#include <nchan_module.h>
#include <subscribers/common.h>
#include "internal.h"
#include <assert.h>

//#define DEBUG_LEVEL NGX_LOG_WARN
#define DEBUG_LEVEL NGX_LOG_DEBUG

#define DBG(fmt, arg...) ngx_log_error(DEBUG_LEVEL, ngx_cycle->log, 0, "SUB:INTERNAL:" fmt, ##arg)
#define ERR(fmt, arg...) ngx_log_error(NGX_LOG_ERR, ngx_cycle->log, 0, "SUB:INTERNAL:" fmt, ##arg)

static const subscriber_t new_internal_sub;

static ngx_int_t empty_callback(ngx_int_t code, void *ptr, void *d) {
  return NGX_OK;
}

ngx_int_t internal_subscriber_set_enqueue_handler(subscriber_t *sub, callback_pt handler) {
  ((internal_subscriber_t *)sub)->enqueue = handler;
  return NGX_OK;
}
ngx_int_t internal_subscriber_set_dequeue_handler(subscriber_t *sub, callback_pt handler) {
  ((internal_subscriber_t *)sub)->dequeue = handler;
  return NGX_OK;
}
ngx_int_t internal_subscriber_set_notify_handler(subscriber_t *sub, callback_pt handler) {
  ((internal_subscriber_t *)sub)->notify = handler;
  return NGX_OK;
}
ngx_int_t internal_subscriber_set_respond_message_handler(subscriber_t *sub, callback_pt handler) {
  ((internal_subscriber_t *)sub)->respond_message = handler;
  return NGX_OK;
}
ngx_int_t internal_subscriber_set_respond_status_handler(subscriber_t *sub, callback_pt handler) {
  ((internal_subscriber_t *)sub)->respond_status = handler;
  return NGX_OK;
}

static ngx_str_t     subscriber_name = ngx_string("internal");
static nchan_loc_conf_t              dummy_config;
static nchan_loc_conf_t             *dummy_config_ptr = NULL;

subscriber_t *internal_subscriber_create(ngx_str_t *name, void *privdata) {
  internal_subscriber_t               *fsub;
  if(dummy_config_ptr == NULL) {
    ngx_memzero(&dummy_config, sizeof(dummy_config));
    dummy_config.buffer_timeout = 0;
    dummy_config.max_messages = -1;
    dummy_config_ptr = &dummy_config;
  }
  
  if((fsub = ngx_alloc(sizeof(*fsub), ngx_cycle->log)) == NULL) {
    ERR("Unable to allocate");
    return NULL;
  }
  
  nchan_subscriber_init(&fsub->sub, &new_internal_sub, NULL, NULL);
  nchan_subscriber_init_timeout_timer(&fsub->sub, &fsub->timeout_ev);
  fsub->sub.cf = &dummy_config;
  fsub->sub.name= (name == NULL ? &subscriber_name : name);
  
  fsub->enqueue = empty_callback;
  fsub->dequeue = empty_callback;
  fsub->respond_message = empty_callback;
  fsub->respond_status = empty_callback;
  fsub->notify = empty_callback;
  
  DBG("%p create %V with privdata %p", fsub, fsub->sub.name, privdata);
  fsub->privdata = privdata;
  
  fsub->already_dequeued = 0;
  fsub->awaiting_destruction = 0;
  
  fsub->dequeue_handler_data = NULL;
  
#if NCHAN_SUBSCRIBER_LEAK_DEBUG
  subscriber_debug_add(&fsub->sub);
#endif
  
  return &fsub->sub;
}

subscriber_t *internal_subscriber_create_init(ngx_str_t *sub_name, void *privdata, callback_pt enqueue, callback_pt dequeue, callback_pt respond_message, callback_pt respond_status, callback_pt notify_handler) {
  subscriber_t          *sub;
  
  if(privdata == NULL) {
    ERR("couldn't allocate %V subscriber data", sub_name);
    return NULL;
  }
  
  sub = internal_subscriber_create(sub_name, privdata);
  if(enqueue)
    internal_subscriber_set_enqueue_handler(sub, enqueue);
  if(dequeue)
    internal_subscriber_set_dequeue_handler(sub, dequeue);
  if(respond_message)
    internal_subscriber_set_respond_message_handler(sub, respond_message);
  if(respond_status)
    internal_subscriber_set_respond_status_handler(sub, respond_status);
  if(notify_handler)
    internal_subscriber_set_notify_handler(sub, notify_handler);
  return sub;
}

ngx_int_t internal_subscriber_destroy(subscriber_t *sub) {
  internal_subscriber_t  *fsub = (internal_subscriber_t  *)sub;
  if(sub->reserved > 0) {
    DBG("%p not ready to destroy (reserved for %i)", sub, sub->reserved);
    fsub->awaiting_destruction = 1;
  }
  else {
    DBG("%p (%V) destroy", sub, fsub->sub.name);
#if NCHAN_SUBSCRIBER_LEAK_DEBUG
    subscriber_debug_remove(sub);
#endif
    nchan_free_msg_id(&sub->last_msgid);
    ngx_free(fsub);
  }
  return NGX_OK;
}

static ngx_int_t internal_reserve(subscriber_t *self) {
  internal_subscriber_t  *fsub = (internal_subscriber_t  *)self;
  DBG("%p ) (%V) reserve", self, fsub->sub.name);
  self->reserved++;
  return NGX_OK;
}
static ngx_int_t internal_release(subscriber_t *self, uint8_t nodestroy) {
  internal_subscriber_t  *fsub = (internal_subscriber_t  *)self;
  DBG("%p (%V) release", self, fsub->sub.name);
  self->reserved--;
  if(nodestroy == 0 && fsub->awaiting_destruction == 1 && self->reserved == 0) {
    DBG("%p (%V) free", self, fsub->sub.name);
    ngx_free(fsub);
    return NGX_ABORT;
  }
  else {
    return NGX_OK;
  }
}

void *internal_subscriber_get_privdata(subscriber_t *sub) {
  internal_subscriber_t               *fsub = (internal_subscriber_t *)sub;
  return fsub->privdata;
}

static void reset_timer(internal_subscriber_t *f) {
  if(f->sub.cf->subscriber_timeout > 0) {
    if(f->timeout_ev.timer_set) {
      ngx_del_timer(&f->timeout_ev);
    }
    ngx_add_timer(&f->timeout_ev, f->sub.cf->subscriber_timeout * 1000);
  }
}

static ngx_int_t internal_enqueue(subscriber_t *self) {
  internal_subscriber_t   *fsub = (internal_subscriber_t *)self;
  DBG("%p (%V) enqueue", self, fsub->sub.name);
  if(self->cf->subscriber_timeout > 0 && !fsub->timeout_ev.timer_set) {
    //add timeout timer
    reset_timer(fsub);
  }
  fsub->enqueue(fsub->sub.cf->buffer_timeout, NULL, fsub->privdata);
  self->enqueued = 1;
  return NGX_OK;
}

static ngx_int_t internal_dequeue(subscriber_t *self) {
  internal_subscriber_t   *f = (internal_subscriber_t *)self;
  assert(!f->already_dequeued);
  f->already_dequeued = 1;
  DBG("%p (%V) dequeue sub", self, f->sub.name);
  f->dequeue(NGX_OK, NULL, f->privdata);
  f->dequeue_handler(self, f->dequeue_handler_data);
  if(self->cf->subscriber_timeout > 0 && f->timeout_ev.timer_set) {
    ngx_del_timer(&f->timeout_ev);
  }
  self->enqueued = 0;
  if(self->destroy_after_dequeue) {
    internal_subscriber_destroy(self);
  }
  return NGX_OK;
}

static ngx_int_t dequeue_maybe(subscriber_t *self) {
  if(self->dequeue_after_response) {
    self->fn->dequeue(self);
  }
  return NGX_OK;
}

static ngx_int_t internal_respond_message(subscriber_t *self, nchan_msg_t *msg) {
  internal_subscriber_t   *f = (internal_subscriber_t *)self;
  
  update_subscriber_last_msg_id(self, msg);
  
  DBG("%p (%V) respond msg %p", self, f->sub.name, msg);
  f->respond_message(NGX_OK, msg, f->privdata);
  reset_timer(f);
  return dequeue_maybe(self);
}

static ngx_int_t internal_respond_status(subscriber_t *self, ngx_int_t status_code, const ngx_str_t *status_line) {
  internal_subscriber_t   *f = (internal_subscriber_t *)self;
  DBG("%p status %i", self, status_code);
  if(status_code == NGX_HTTP_GONE) {
    self->dequeue_after_response = 1;
  }
  f->respond_status(status_code, (void *)status_line, f->privdata);
  reset_timer(f);
  return dequeue_maybe(self);
}

static ngx_int_t internal_set_dequeue_callback(subscriber_t *self, subscriber_callback_pt cb, void *privdata) {
  internal_subscriber_t   *f = (internal_subscriber_t *)self;
  if(cb != NULL) {
    DBG("%p set dequeue handler to %p", self, cb);
    f->dequeue_handler = cb;
  }
  if(privdata != NULL) {
    DBG("%p set dequeue handler data to %p", self, cb);
    f->dequeue_handler_data = privdata;
  }
  return NGX_OK;
}

ngx_int_t internal_subscriber_set_name(subscriber_t *self, ngx_str_t *name) {
  self->name = name;
  return NGX_OK;
}

static ngx_int_t internal_notify(subscriber_t *self, ngx_int_t code, void *data) {
  internal_subscriber_t   *fsub = (internal_subscriber_t *)self;
  return fsub->notify(code, data, fsub->privdata);
}

static const subscriber_fn_t internal_sub_fn = {
  &internal_enqueue,
  &internal_dequeue,
  &internal_respond_message,
  &internal_respond_status,
  &internal_set_dequeue_callback,
  &internal_reserve,
  &internal_release,
  &internal_notify,
  &nchan_subscriber_subscribe
};

static const subscriber_t new_internal_sub = {
  &subscriber_name,
  INTERNAL,
  &internal_sub_fn,
  UNKNOWN,
  NCHAN_ZERO_MSGID,
  NULL,
  NULL,
  0, //reserved
  0, //stick around after response
  1, //destroy after dequeue
  0, //enqueued

};
