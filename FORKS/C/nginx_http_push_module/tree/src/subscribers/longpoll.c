#include <nchan_module.h>
#include <subscribers/common.h>
//#define DEBUG_LEVEL NGX_LOG_WARN
#define DEBUG_LEVEL NGX_LOG_DEBUG
#define DBG(fmt, arg...) ngx_log_error(DEBUG_LEVEL, ngx_cycle->log, 0, "SUB:LONGPOLL:" fmt, ##arg)
#define ERR(fmt, arg...) ngx_log_error(NGX_LOG_ERR, ngx_cycle->log, 0, "SUB:LONGPOLL:" fmt, ##arg)
#include <assert.h>
#include "longpoll-private.h"

#include <store/memory/store.h>

void memstore_fakeprocess_push(ngx_int_t slot);
void memstore_fakeprocess_push_random(void);
void memstore_fakeprocess_pop(void);
ngx_int_t memstore_slot(void);

static const subscriber_t new_longpoll_sub;

static void empty_handler() { }

static void sudden_abort_handler(subscriber_t *sub) {
#if FAKESHARD
  full_subscriber_t  *fsub = (full_subscriber_t  *)sub;
  memstore_fakeprocess_push(fsub->sub.owner);
#endif
  sub->status = DEAD;
  sub->fn->dequeue(sub);
#if FAKESHARD
  memstore_fakeprocess_pop();
#endif
}

//void verify_unique_response(ngx_str_t *uri, nchan_msg_id_t *msgid, nchan_msg_t *msg, subscriber_t *sub);

subscriber_t *longpoll_subscriber_create(ngx_http_request_t *r, nchan_msg_id_t *msg_id) {
  DBG("create for req %p", r);
  full_subscriber_t      *fsub;

  //TODO: allocate from pool (but not the request's pool)
  if((fsub = ngx_alloc(sizeof(*fsub), ngx_cycle->log)) == NULL) {
    ERR("Unable to allocate");
    assert(0);
    return NULL;
  }
  
  nchan_subscriber_init(&fsub->sub, &new_longpoll_sub, r, msg_id);
  fsub->privdata = NULL;
  fsub->data.cln = NULL;
  fsub->data.finalize_request = 1;
  fsub->data.holding = 0;
  fsub->data.act_as_intervalpoll = 0;
  
  nchan_subscriber_init_timeout_timer(&fsub->sub, &fsub->data.timeout_ev);
  
  fsub->data.dequeue_handler = empty_handler;
  fsub->data.dequeue_handler_data = NULL;
  fsub->data.already_responded = 0;
  fsub->data.awaiting_destruction = 0;
  
  if(fsub->sub.cf->longpoll_multimsg) {
    fsub->sub.dequeue_after_response = 0;
  }
  
  fsub->data.multimsg_first = NULL;
  fsub->data.multimsg_last = NULL;
  
#if NCHAN_SUBSCRIBER_LEAK_DEBUG
  subscriber_debug_add(&fsub->sub);
  //set debug label
  fsub->sub.lbl = ngx_calloc(r->uri.len+1, ngx_cycle->log);
  ngx_memcpy(fsub->sub.lbl, r->uri.data, r->uri.len);
#endif
  
  //http request sudden close cleanup
  if((fsub->data.cln = ngx_http_cleanup_add(r, 0)) == NULL) {
    ERR("Unable to add request cleanup for longpoll subscriber");
    assert(0);
    return NULL;
  }
  fsub->data.cln->data = fsub;
  fsub->data.cln->handler = (ngx_http_cleanup_pt )sudden_abort_handler;
  DBG("%p created for request %p", &fsub->sub, r);
  
  
  return &fsub->sub;
}

ngx_int_t longpoll_subscriber_destroy(subscriber_t *sub) {
  full_subscriber_t   *fsub = (full_subscriber_t  *)sub;
  
  if(sub->reserved > 0) {
    DBG("%p not ready to destroy (reserved for %i) for req %p", sub, sub->reserved, fsub->sub.request);
    fsub->data.awaiting_destruction = 1;
  }
  else {
    DBG("%p destroy for req %p", sub, fsub->sub.request);
    nchan_free_msg_id(&fsub->sub.last_msgid);
    assert(sub->status == DEAD);
#if NCHAN_SUBSCRIBER_LEAK_DEBUG
    subscriber_debug_remove(sub);
    ngx_free(sub->lbl);
    ngx_memset(fsub, 0xB9, sizeof(*fsub)); //debug
#endif
    ngx_free(fsub);
  }
  return NGX_OK;
}

static void ensure_request_hold(full_subscriber_t *fsub) {
  if(fsub->data.holding == 0) {
    DBG("hodl request %p", fsub->sub.request);
    fsub->data.holding = 1;
    fsub->sub.request->read_event_handler = ngx_http_test_reading;
    fsub->sub.request->write_event_handler = ngx_http_request_empty_handler;
    fsub->sub.request->main->count++; //this is the right way to hold and finalize the request... maybe
  }
}

static ngx_int_t longpoll_reserve(subscriber_t *self) {
  full_subscriber_t  *fsub = (full_subscriber_t  *)self;
  ensure_request_hold(fsub);
  self->reserved++;
  DBG("%p reserve for req %p, reservations: %i", self, fsub->sub.request, self->reserved);
  return NGX_OK;
}
static ngx_int_t longpoll_release(subscriber_t *self, uint8_t nodestroy) {
  full_subscriber_t  *fsub = (full_subscriber_t  *)self;
  assert(self->reserved > 0);
  self->reserved--;
  DBG("%p release for req %p. reservations: %i", self, fsub->sub.request, self->reserved);
  if(nodestroy == 0 && fsub->data.awaiting_destruction == 1 && self->reserved == 0) {
    longpoll_subscriber_destroy(self);
    return NGX_ABORT;
  }
  else {
    return NGX_OK;
  }
}

ngx_int_t longpoll_enqueue(subscriber_t *self) {
  full_subscriber_t  *fsub = (full_subscriber_t  *)self;
  assert(fsub->sub.enqueued == 0);
  DBG("%p enqueue", self);
  
  fsub->data.finalize_request = 1;
  
  fsub->sub.enqueued = 1;
  ensure_request_hold(fsub);
  if(self->cf->subscriber_timeout > 0) {
    //add timeout timer
    ngx_add_timer(&fsub->data.timeout_ev, self->cf->subscriber_timeout * 1000);
  }
  
  return NGX_OK;
}

static ngx_int_t longpoll_dequeue(subscriber_t *self) {
  full_subscriber_t    *fsub = (full_subscriber_t  *)self;
  nchan_request_ctx_t  *ctx = ngx_http_get_module_ctx(fsub->sub.request, nchan_module);
  if(fsub->data.timeout_ev.timer_set) {
    ngx_del_timer(&fsub->data.timeout_ev);
  }
  DBG("%p dequeue", self);
  fsub->data.dequeue_handler(self, fsub->data.dequeue_handler_data);
  self->enqueued = 0;
  
  ctx->sub = NULL;
  
  if(fsub->data.finalize_request) {
    DBG("finalize request %p", fsub->sub.request);
    ngx_http_finalize_request(fsub->sub.request, NGX_OK);
    self->status = DEAD;
  }
  
  if(self->destroy_after_dequeue) {
    longpoll_subscriber_destroy(self);
  }
  return NGX_OK;
}

static ngx_int_t dequeue_maybe(subscriber_t *self) {
  if(self->dequeue_after_response) {
    self->fn->dequeue(self);
  }
  return NGX_OK;
}

static ngx_int_t abort_response(subscriber_t *sub, char *errmsg) {
  ERR("abort! %s", errmsg ? errmsg : "unknown error");
  dequeue_maybe(sub);
  return NGX_ERROR;
}

static ngx_int_t longpoll_multipart_add(full_subscriber_t *fsub, nchan_msg_t *msg, char **err) {
  
  nchan_longpoll_multimsg_t     *mmsg;
  
  if((mmsg = ngx_palloc(fsub->sub.request->pool, sizeof(*mmsg))) == NULL) {
    *err = "can't allocate multipart msg link";
    return NGX_ERROR;
  }
  
  if(msg->shared) {
    msg_reserve(msg, "longpoll multipart");
  }
  else if(msg->id.tagcount > 1) {
    //msg from a multiplexed channel
    assert(!msg->shared && !msg->temp_allocd);
    nchan_msg_copy_t *cmsg;
    if((cmsg = ngx_palloc(fsub->sub.request->pool, sizeof(*cmsg))) == NULL) {
      *err = "can't allocate msgcopy for message from multiplexed channel";
      return NGX_ERROR;
      
    }
    //  multiplexed channel message should have been created as a nchan_msg_copy_t
    *cmsg = *(nchan_msg_copy_t *)msg;
    
    cmsg->copy.temp_allocd = 1;
    
    assert(cmsg->original->shared);
    msg_reserve(cmsg->original, "longpoll multipart for multiplexed channel");
    msg = &cmsg->copy;
  }
  else {
    assert(0); //this is not yet an expected scenario;
  }
  
  mmsg->msg = msg;
  mmsg->next = NULL;
  if(fsub->data.multimsg_first == NULL) {
    fsub->data.multimsg_first = mmsg;
  }
  if(fsub->data.multimsg_last) {
    fsub->data.multimsg_last->next = mmsg;
  }
  fsub->data.multimsg_last = mmsg;
  
  return NGX_OK;
}

static ngx_int_t longpoll_respond_message(subscriber_t *self, nchan_msg_t *msg) {
  full_subscriber_t  *fsub = (full_subscriber_t  *)self;
  ngx_int_t                  rc;
  char                      *err = NULL;
  ngx_http_request_t        *r = fsub->sub.request;
  nchan_request_ctx_t       *ctx = ngx_http_get_module_ctx(r, nchan_module);
  nchan_loc_conf_t          *cf = fsub->sub.cf;
  
  DBG("%p respond req %p msg %p", self, r, msg);
  
  ctx->prev_msg_id = self->last_msgid;
  update_subscriber_last_msg_id(self, msg);
  ctx->msg_id = self->last_msgid;

  //verify_unique_response(&fsub->data.request->uri, &self->last_msgid, msg, self);
  if(fsub->data.timeout_ev.timer_set) {
    ngx_del_timer(&fsub->data.timeout_ev);
  }
  if(!cf->longpoll_multimsg) {
    //disable abort handler
    fsub->data.cln->handler = empty_handler;
    
    assert(fsub->data.already_responded != 1);
    fsub->data.already_responded = 1;
    if((rc = nchan_respond_msg(r, msg, &self->last_msgid, 0, &err)) != NGX_OK) {
      return abort_response(self, err);
    }
  }
  else {
    if((rc = longpoll_multipart_add(fsub, msg, &err)) != NGX_OK) {
      return abort_response(self, err);
    }
  }
  dequeue_maybe(self);
  return rc;
}

static void multipart_request_cleanup_handler(nchan_longpoll_multimsg_t *first) {
  nchan_longpoll_multimsg_t    *cur;
  nchan_msg_copy_t             *cmsg;
  for(cur = first; cur != NULL; cur = cur->next) {
    if(cur->msg->shared) {
      msg_release(cur->msg, "longpoll multipart");
    }
    else if(cur->msg->id.tagcount > 1) {
      assert(!cur->msg->shared && cur->msg->temp_allocd);
      // multiplexed channel message should have been created as a nchan_msg_copy_t
      cmsg = (nchan_msg_copy_t *)cur->msg;
      
      assert(cmsg->original->shared);
      msg_release(cmsg->original, "longpoll multipart for multiplexed channel");
    }
    else {
      assert(0);
    }
  }
}

static ngx_int_t longpoll_multipart_respond(full_subscriber_t *fsub) {
  ngx_http_request_t    *r = fsub->sub.request;
  nchan_request_ctx_t   *ctx = ngx_http_get_module_ctx(r, nchan_module);
  char                  *err;
  ngx_int_t              rc;
  u_char                 char_boundary[50];
  u_char                *char_boundary_last;
  
  ngx_int_t              i;
  ngx_buf_t              boundary[3]; //first, mid, and last boundary
  ngx_chain_t           *chains, *first_chain = NULL, *last_chain = NULL;
  ngx_buf_t             *buf;
  ngx_buf_t              double_newline_buf;
  ngx_str_t             *content_type;
  size_t                 size = 0;
  nchan_longpoll_multimsg_t *first, *cur;
  
  //disable abort handler
  fsub->data.cln->handler = empty_handler;
  
  first = fsub->data.multimsg_first;
  
  
  fsub->sub.dequeue_after_response = 1;
  
  //cleanup to release msgs
  fsub->data.cln = ngx_http_cleanup_add(fsub->sub.request, 0);
  fsub->data.cln->data = first;
  fsub->data.cln->handler = (ngx_http_cleanup_pt )multipart_request_cleanup_handler;
  
  if(fsub->data.multimsg_first == fsub->data.multimsg_last) {
    //just one message.
    if((rc = nchan_respond_msg(r, fsub->data.multimsg_first->msg, &fsub->sub.last_msgid, 0, &err)) != NGX_OK) {
      return abort_response(&fsub->sub, err);
    }
    return NGX_OK;
  }
  
  //multi messages
  nchan_request_set_content_type_multipart_boundary_header(r, ctx);
  
  char_boundary_last = ngx_snprintf(char_boundary, 50, ("\r\n--%V--\r\n"), nchan_request_multipart_boundary(r, ctx));
  
  ngx_memzero(&double_newline_buf, sizeof(double_newline_buf));
  double_newline_buf.start = (u_char *)"\r\n\r\n";
  double_newline_buf.end = double_newline_buf.start + 4;
  double_newline_buf.pos = double_newline_buf.start;
  double_newline_buf.last = double_newline_buf.end;
  double_newline_buf.memory = 1;
  
  //set up the boundaries
  for(i=0; i<3; i++) {
    ngx_memzero(&boundary[i], sizeof(ngx_buf_t));
    boundary[i].memory = 1;
    if(i==0) {
      boundary[i].start = &char_boundary[2];
      boundary[i].end = &char_boundary_last[-4];
    }
    else if(i==1) {
      boundary[i].start = &char_boundary[0];
      boundary[i].end = &char_boundary_last[-4];
    }
    else if(i==2) {
      boundary[i].start = &char_boundary[0];
      boundary[i].end = char_boundary_last;
      boundary[i].last_buf = 1;
      boundary[i].last_in_chain = 1;
      boundary[i].flush = 1;
    }
    boundary[i].pos = boundary[i].start;
    boundary[i].last = boundary[i].end;
  }
  
  int n=0;
  
  for(cur = first; cur != NULL; cur = cur->next) {
    chains = ngx_palloc(r->pool, sizeof(*chains)*4);
    n++;
    
    if(last_chain) {
      last_chain->next = &chains[0];
    }
    if(!first_chain) {
      first_chain = &chains[0];
    }
    
    // each buffer needs to be unique for the purpose of dealing with nginx output guts
    // (something about max. 64 iovecs per write call and counting the number of bytes already sent)
    buf = ngx_pcalloc(r->pool, sizeof(*buf));
    *buf = cur == first ? boundary[0] : boundary[1];
    chains[0].buf = buf;
    chains[0].next = &chains[1];
    
    size += ngx_buf_size(chains[0].buf);
    
    content_type = &cur->msg->content_type;
    if (content_type->data != NULL) {
      buf = ngx_pcalloc(r->pool, sizeof(*buf) + content_type->len + 25);
      buf->memory = 1;
      buf->start = (u_char *)&buf[1];
      buf->end = ngx_snprintf(buf->start, content_type->len + 25, "\r\nContent-Type: %V\r\n\r\n", content_type);
      buf->pos = buf->start;
      buf->last = buf->end;
      chains[1].buf = buf;
    }
    else {

      buf = ngx_palloc(r->pool, sizeof(*buf));
      chains[1].buf = buf;
      *buf = double_newline_buf;
    }
    size += ngx_buf_size(chains[1].buf);
    
    if(ngx_buf_size(cur->msg->buf) > 0) {
      chains[1].next = &chains[2];
      
      buf = ngx_palloc(r->pool, sizeof(*buf));
      *buf = *cur->msg->buf;
      nchan_msg_buf_open_fd_if_needed(buf, NULL, r);
      buf->last_buf = 0;
      chains[2].buf = buf;
      size += ngx_buf_size(chains[2].buf);
      
      last_chain = &chains[2];  
    }
    else {
      last_chain = &chains[1];
    }
    
    if(cur->next == NULL) {
      last_chain->next = &chains[3];
      
      chains[3].buf = &boundary[2];
      chains[3].next = NULL;
      last_chain = &chains[3];
      size += ngx_buf_size(chains[3].buf);
    }
  }
  
  r->headers_out.status = NGX_HTTP_OK;
  r->headers_out.content_length_n = size;
  nchan_set_msgid_http_response_headers(r, ctx, &fsub->data.multimsg_last->msg->id);
  nchan_include_access_control_if_needed(r, ctx);
  ngx_http_send_header(r);
  nchan_output_filter(r, first_chain);
  
  
  return NGX_OK;
}

static ngx_int_t longpoll_respond_status(subscriber_t *self, ngx_int_t status_code, const ngx_str_t *status_line) {
  
  full_subscriber_t     *fsub = (full_subscriber_t *)self;
  ngx_http_request_t    *r = fsub->sub.request;
  nchan_loc_conf_t      *cf = fsub->sub.cf;
  
  //DBG("%p got status %i", self, status_code);
  
  if(fsub->data.act_as_intervalpoll) {
    if(status_code == NGX_HTTP_NO_CONTENT || status_code == NGX_HTTP_NOT_MODIFIED || status_code == NGX_HTTP_NOT_FOUND ) {
      status_code = NGX_HTTP_NOT_MODIFIED;
    }
  }
  else if(status_code == NGX_HTTP_NO_CONTENT || (status_code == NGX_HTTP_NOT_MODIFIED && !status_line)) {
    if(cf->longpoll_multimsg) {
      if(fsub->data.multimsg_first != NULL) {
        longpoll_multipart_respond(fsub);
        dequeue_maybe(self);
      }
      return NGX_OK;
    }
    else { 
      //don't care, ignore
      return NGX_OK;
    }
  }
  
  DBG("%p respond req %p status %i", self, r, status_code);
  
  nchan_set_msgid_http_response_headers(r, NULL, &self->last_msgid);
  
  //disable abort handler
  fsub->data.cln->handler = empty_handler;
  
  nchan_respond_status(r, status_code, status_line, 0);

  dequeue_maybe(self);
  return NGX_OK;
}

void subscriber_maybe_dequeue_after_status_response(full_subscriber_t *fsub, ngx_int_t status_code) {
  if((status_code >=400 && status_code < 600) || status_code == NGX_HTTP_NOT_MODIFIED) {
    fsub->data.cln->handler = (ngx_http_cleanup_pt )empty_handler;
    fsub->sub.request->keepalive=0;
    fsub->data.finalize_request=1;
    fsub->sub.fn->dequeue(&fsub->sub);
  }
}

static void request_cleanup_handler(subscriber_t *sub) {

}


static ngx_int_t longpoll_set_dequeue_callback(subscriber_t *self, subscriber_callback_pt cb, void *privdata) {
  full_subscriber_t  *fsub = (full_subscriber_t  *)self;
  if(fsub->data.cln == NULL) {
    fsub->data.cln = ngx_http_cleanup_add(fsub->sub.request, 0);
    fsub->data.cln->data = self;
    fsub->data.cln->handler = (ngx_http_cleanup_pt )request_cleanup_handler;
  }
  fsub->data.dequeue_handler = cb;
  fsub->data.dequeue_handler_data = privdata;
  return NGX_OK;
}

static const subscriber_fn_t longpoll_fn = {
  &longpoll_enqueue,
  &longpoll_dequeue,
  &longpoll_respond_message,
  &longpoll_respond_status,
  &longpoll_set_dequeue_callback,
  &longpoll_reserve,
  &longpoll_release,
  NULL,
  &nchan_subscriber_authorize_subscribe
};

static ngx_str_t  sub_name = ngx_string("longpoll");

static const subscriber_t new_longpoll_sub = {
  &sub_name,
  LONGPOLL,
  &longpoll_fn,
  UNKNOWN,
  NCHAN_ZERO_MSGID,
  NULL,
  NULL,
  0, //reservations
  1, //deque after response
  1, //destroy after dequeue
  0, //enqueued
};
