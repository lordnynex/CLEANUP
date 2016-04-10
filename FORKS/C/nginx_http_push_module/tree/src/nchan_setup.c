#include <nchan_websocket_publisher.h>
#include <nchan_types.h>
#include <util/nchan_output.h>
#include <nchan_variables.h>
#include <store/memory/store.h>
#include <store/redis/store.h>

static ngx_str_t      DEFAULT_CHANNEL_EVENT_STRING = ngx_string("$nchan_channel_event $nchan_channel_id");

ngx_module_t     nchan_module;

nchan_store_t   *default_storage_engine = &nchan_store_memory;
ngx_flag_t       global_redis_enabled = 0;

#define MERGE_CONF(cf, prev_cf, name) if((cf)->name == NULL) { (cf)->name = (prev_cf)->name; }

static ngx_int_t nchan_init_module(ngx_cycle_t *cycle) {
  ngx_core_conf_t                *ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);
  nchan_worker_processes = ccf->worker_processes;
  //initialize subscriber queues
  //pool, please
  if((nchan_pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, cycle->log))==NULL) {
  //I trust the cycle pool size to be a well-tuned one.
    return NGX_ERROR; 
  }
  
  //initialize storage engines
  nchan_store_memory.init_module(cycle);
  if(global_redis_enabled) {
    nchan_store_redis.init_module(cycle);
  }
  return NGX_OK;
}

static ngx_int_t nchan_init_worker(ngx_cycle_t *cycle) {
  if (ngx_process != NGX_PROCESS_WORKER && ngx_process != NGX_PROCESS_SINGLE) {
    //not a worker, stop initializing stuff.
    return NGX_OK;
  }
  
  if(nchan_store_memory.init_worker(cycle)!=NGX_OK) {
    return NGX_ERROR;
  }
  
  if(global_redis_enabled && nchan_store_redis.init_worker(cycle)!=NGX_OK) {
    return NGX_ERROR;
  }
  
  nchan_websocket_publisher_llist_init();
  nchan_output_init();
  
  return NGX_OK;
}

static ngx_int_t nchan_postconfig(ngx_conf_t *cf) {
  if(nchan_store_memory.init_postconfig(cf)!=NGX_OK) {
    return NGX_ERROR;
  }
  if(global_redis_enabled && nchan_store_redis.init_postconfig(cf)!=NGX_OK) {
    return NGX_ERROR;
  }
  return NGX_OK;
}

//main config
static void * nchan_create_main_conf(ngx_conf_t *cf) {
  nchan_main_conf_t      *mcf = ngx_pcalloc(cf->pool, sizeof(*mcf));
  if(mcf == NULL) {
    return NGX_CONF_ERROR;
  }
  
  nchan_store_memory.create_main_conf(cf, mcf);
  if(global_redis_enabled) {
    nchan_store_redis.create_main_conf(cf, mcf);
  }
  
  return mcf;
}

//location config stuff
static void *nchan_create_loc_conf(ngx_conf_t *cf) {
  nchan_loc_conf_t       *lcf = ngx_pcalloc(cf->pool, sizeof(*lcf));
  if(lcf == NULL) {
    return NGX_CONF_ERROR;
  }
  
  lcf->pub.http=0;
  lcf->pub.websocket=0;
  
  lcf->sub.poll=0;
  lcf->sub.longpoll=0;
  lcf->sub.eventsource=0;
  lcf->sub.websocket=0;
  lcf->sub.http_chunked=0;
  
  lcf->authorize_request_url = NULL;
  lcf->publisher_upstream_request_url = NULL;
  
  lcf->buffer_timeout=NGX_CONF_UNSET;
  lcf->max_messages=NGX_CONF_UNSET;
  lcf->subscriber_start_at_oldest_message=NGX_CONF_UNSET;
  
  lcf->subscriber_timeout=NGX_CONF_UNSET;
  lcf->subscribe_only_existing_channel=NGX_CONF_UNSET;
  lcf->use_redis=NGX_CONF_UNSET;
  lcf->max_channel_id_length=NGX_CONF_UNSET;
  lcf->max_channel_subscribers=NGX_CONF_UNSET;
  lcf->channel_timeout=NGX_CONF_UNSET;
  lcf->storage_engine=NULL;
  
  lcf->websocket_ping_interval=NGX_CONF_UNSET;
  
  lcf->msg_in_etag_only = NGX_CONF_UNSET;
  
  lcf->channel_events_channel_id = NULL;
  lcf->channel_event_string = NULL;
  
  lcf->longpoll_multimsg=NGX_CONF_UNSET;
  
  ngx_memzero(&lcf->pub_chid, sizeof(nchan_complex_value_arr_t));
  ngx_memzero(&lcf->sub_chid, sizeof(nchan_complex_value_arr_t));
  ngx_memzero(&lcf->pubsub_chid, sizeof(nchan_complex_value_arr_t));
  ngx_memzero(&lcf->last_message_id, sizeof(nchan_complex_value_arr_t));
  return lcf;
}

static ngx_int_t nchan_strmatch(ngx_str_t *val, ngx_int_t n, ...) {
  u_char   *match;
  va_list   args;
  ngx_int_t i;
  va_start(args, n);  
  for(i=0; i<n; i++) {
    match = va_arg(args, u_char *);
    if(ngx_strncasecmp(val->data, match, val->len)==0) {
      return 1;
    }
  }
  va_end(args);
  return 0;
}

static char * create_complex_value_from_ngx_str(ngx_conf_t *cf, ngx_http_complex_value_t **dst_cv, ngx_str_t *str) {
  ngx_http_complex_value_t           *cv;
  ngx_http_compile_complex_value_t    ccv;
  
  cv = ngx_palloc(cf->pool, sizeof(*cv));
  if (cv == NULL) {
    ngx_conf_log_error(NGX_LOG_ERR, cf, 0, "unable to allocate space for complex value");
    return NGX_CONF_ERROR;
  }
  
  ngx_memzero(&ccv, sizeof(ccv));
  
  ccv.cf = cf;
  ccv.value = str;
  ccv.complex_value = cv;
  
  if (ngx_http_compile_complex_value(&ccv) != NGX_OK) {
    return NGX_CONF_ERROR;
  }
  
  *dst_cv = cv;
  return NGX_CONF_OK;
}

static char * nchan_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child) {
  nchan_loc_conf_t       *prev = parent, *conf = child;
  
  //publisher types
  ngx_conf_merge_bitmask_value(conf->pub.http, prev->pub.http, 0);
  ngx_conf_merge_bitmask_value(conf->pub.websocket, prev->pub.websocket, 0);
  
  //subscriber types
  ngx_conf_merge_bitmask_value(conf->sub.poll, prev->sub.poll, 0);
  ngx_conf_merge_bitmask_value(conf->sub.longpoll, prev->sub.longpoll, 0);
  ngx_conf_merge_bitmask_value(conf->sub.eventsource, prev->sub.eventsource, 0);
  ngx_conf_merge_bitmask_value(conf->sub.http_chunked, prev->sub.http_chunked, 0);
  ngx_conf_merge_bitmask_value(conf->sub.websocket, prev->sub.websocket, 0);
  
  ngx_conf_merge_sec_value(conf->buffer_timeout, prev->buffer_timeout, NCHAN_DEFAULT_BUFFER_TIMEOUT);
  ngx_conf_merge_value(conf->max_messages, prev->max_messages, NCHAN_DEFAULT_MAX_MESSAGES);
  ngx_conf_merge_value(conf->subscriber_start_at_oldest_message, prev->subscriber_start_at_oldest_message, 1);
  ngx_conf_merge_sec_value(conf->websocket_ping_interval, prev->websocket_ping_interval, NCHAN_DEFAULT_WEBSOCKET_PING_INTERVAL);
  
  ngx_conf_merge_sec_value(conf->subscriber_timeout, prev->subscriber_timeout, NCHAN_DEFAULT_SUBSCRIBER_TIMEOUT);
  ngx_conf_merge_value(conf->subscribe_only_existing_channel, prev->subscribe_only_existing_channel, 0);
  ngx_conf_merge_value(conf->use_redis, prev->use_redis, 0);
  ngx_conf_merge_value(conf->max_channel_id_length, prev->max_channel_id_length, NCHAN_MAX_CHANNEL_ID_LENGTH);
  ngx_conf_merge_value(conf->max_channel_subscribers, prev->max_channel_subscribers, 0);
  ngx_conf_merge_value(conf->channel_timeout, prev->channel_timeout, NCHAN_DEFAULT_CHANNEL_TIMEOUT);
  ngx_conf_merge_str_value(conf->channel_group, prev->channel_group, "");
  ngx_conf_merge_str_value(conf->allow_origin, prev->allow_origin, "*");
  ngx_conf_merge_str_value(conf->eventsource_event, prev->eventsource_event, "");
  ngx_conf_merge_str_value(conf->custom_msgtag_header, prev->custom_msgtag_header, "");
  ngx_conf_merge_value(conf->msg_in_etag_only, prev->msg_in_etag_only, 0);
  ngx_conf_merge_value(conf->longpoll_multimsg, prev->longpoll_multimsg, 0);
  MERGE_CONF(conf, prev, channel_events_channel_id);
  MERGE_CONF(conf, prev, channel_event_string);
  
  if(conf->channel_event_string == NULL) { //still null? use the default string
    if(create_complex_value_from_ngx_str(cf, &conf->channel_event_string, &DEFAULT_CHANNEL_EVENT_STRING) == NGX_CONF_ERROR) {
      return NGX_CONF_ERROR;
    }
  }

  if(conf->storage_engine == NULL) {
    conf->storage_engine = prev->storage_engine ? prev->storage_engine : default_storage_engine;
  }

  MERGE_CONF(conf, prev, authorize_request_url);
  MERGE_CONF(conf, prev, publisher_upstream_request_url);
  
  if(conf->pub_chid.n == 0) {
    conf->pub_chid = prev->pub_chid;
  }
  if(conf->sub_chid.n == 0) {
    conf->sub_chid = prev->sub_chid;
  }
  if(conf->pubsub_chid.n == 0) {
    conf->pubsub_chid = prev->pubsub_chid;
  }
  if(conf->last_message_id.n == 0) {
    conf->last_message_id = prev->last_message_id;
  }
  if(conf->last_message_id.n == 0) { //if it's still null
    ngx_str_t      first_choice_msgid = ngx_string("$http_last_event_id");
    ngx_str_t      second_choice_msgid = ngx_string("$arg_last_event_id");
    
    if(create_complex_value_from_ngx_str(cf, &conf->last_message_id.cv[0], &first_choice_msgid) == NGX_CONF_ERROR) {
      return NGX_CONF_ERROR;
    }
    if(create_complex_value_from_ngx_str(cf, &conf->last_message_id.cv[1], &second_choice_msgid) == NGX_CONF_ERROR) {
      return NGX_CONF_ERROR;
    }
    conf->last_message_id.n = 2;
  }
  
  
  return NGX_CONF_OK;
}

 //channel id variable
//publisher and subscriber handlers now.
static char *nchan_setup_handler(ngx_conf_t *cf, void * conf, ngx_int_t (*handler)(ngx_http_request_t *)) {
  ngx_http_core_loc_conf_t       *clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
  //nchan_loc_conf_t               *plcf = conf;
  clcf->handler = handler;
  clcf->if_modified_since = NGX_HTTP_IMS_OFF;
  
  return NGX_CONF_OK;
}

static char *nchan_set_storage_engine(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
  nchan_loc_conf_t     *lcf = conf;
  ngx_str_t            *val = &((ngx_str_t *) cf->args->elts)[1];
  
  if(nchan_strmatch(val, 1, "memory")) {
    lcf->storage_engine = &nchan_store_memory;
  }
  else if(nchan_strmatch(val, 1, "redis")) {
    lcf->storage_engine = &nchan_store_redis;
    global_redis_enabled = 1;
  }
  else {
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0, "invalid %V value: %V", &cmd->name, val);
    return NGX_CONF_ERROR;
  }

  return NGX_CONF_OK;
}

#define WEBSOCKET_STRINGS "websocket", "ws", "websockets"
#define WEBSOCKET_STRINGS_N 3

#define EVENTSOURCE_STRINGS "eventsource", "event-source", "es", "sse"
#define EVENTSOURCE_STRINGS_N 4

#define HTTP_CHUNKED_STRINGS "chunked", "http-chunked"
#define HTTP_CHUNKED_STRINGS_N 2

#define HTTP_MULTIPART_STRINGS "multipart", "multipart/mixed", "http-multipart", "multipart-mixed"
#define HTTP_MULTIPART_STRINGS_N 4

#define LONGPOLL_STRINGS "longpoll", "long-poll"
#define LONGPOLL_STRINGS_N 2

#define INTERVALPOLL_STRINGS "poll", "interval-poll", "intervalpoll", "http"
#define INTERVALPOLL_STRINGS_N 4

#define DISABLED_STRINGS "none", "off", "disabled"
#define DISABLED_STRINGS_N 3


static char *nchan_publisher_directive_parse(ngx_conf_t *cf, ngx_command_t *cmd, void *conf, ngx_int_t fail) {
  nchan_loc_conf_t     *lcf = conf;
  ngx_str_t            *val;
  ngx_uint_t            i;
  
  nchan_conf_publisher_types_t *pubt = &lcf->pub;
  
  if(cf->args->nelts == 1){ //no arguments
    pubt->http=1;
    pubt->websocket=1;
  }
  else {
    for(i=1; i < cf->args->nelts; i++) {
      val = &((ngx_str_t *) cf->args->elts)[i];
      if(nchan_strmatch(val, 1, "http")) {
        pubt->http=1;
      }
      else if(nchan_strmatch(val, WEBSOCKET_STRINGS_N, WEBSOCKET_STRINGS)) {
        pubt->websocket=1;
      }
      else{
         if(fail) {
          ngx_conf_log_error(NGX_LOG_ERR, cf, 0, "invalid %V value: %V", &cmd->name, val);
         }
        return NGX_CONF_ERROR;
      }
    }
  }
  
  return nchan_setup_handler(cf, conf, &nchan_pubsub_handler);
}

static char *nchan_publisher_directive(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
  return nchan_publisher_directive_parse(cf, cmd, conf, 1);
}

static char *nchan_subscriber_directive_parse(ngx_conf_t *cf, ngx_command_t *cmd, void *conf, ngx_int_t fail) {
  nchan_loc_conf_t     *lcf = conf;
  ngx_str_t            *val;
  ngx_uint_t            i;
  
  nchan_conf_subscriber_types_t *subt = &lcf->sub;
  
  if(cf->args->nelts == 1){ //no arguments
    subt->poll=0;
    subt->longpoll=1;
    subt->websocket=1;
    subt->eventsource=1;
    subt->http_chunked=1;
    subt->http_multipart=1;
  }
  else {
    for(i=1; i < cf->args->nelts; i++) {
      val = &((ngx_str_t *) cf->args->elts)[i];
      if(nchan_strmatch(val, LONGPOLL_STRINGS_N, LONGPOLL_STRINGS)) {
        subt->longpoll=1;
      }
      else if(nchan_strmatch(val, INTERVALPOLL_STRINGS_N, INTERVALPOLL_STRINGS)) {
        subt->poll=1;
      }
      else if(nchan_strmatch(val, HTTP_CHUNKED_STRINGS_N, HTTP_CHUNKED_STRINGS)) {
        subt->http_chunked=1;
      }
      else if(nchan_strmatch(val, HTTP_MULTIPART_STRINGS_N, HTTP_MULTIPART_STRINGS)) {
        subt->http_multipart=1;
      }
      else if(nchan_strmatch(val, WEBSOCKET_STRINGS_N, WEBSOCKET_STRINGS)) {
        subt->websocket=1;
      }
      else if(nchan_strmatch(val, EVENTSOURCE_STRINGS_N, EVENTSOURCE_STRINGS)) {
        subt->eventsource=1;
      }
      else if(nchan_strmatch(val, DISABLED_STRINGS_N, DISABLED_STRINGS)) {
        subt->poll=0;
        subt->longpoll=0;
        subt->websocket=0;
        subt->eventsource=0;
        subt->http_chunked=0;
        subt->http_multipart=0;
      }
      else {
        if(fail) {
          ngx_conf_log_error(NGX_LOG_ERR, cf, 0, "invalid %V value: %V", &cmd->name, val);
        }
        return NGX_CONF_ERROR;
      }
    }
  }
  return nchan_setup_handler(cf, conf, &nchan_pubsub_handler);
}

static char *nchan_subscriber_directive(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
  return nchan_subscriber_directive_parse(cf, cmd, conf, 1);
}

static char *nchan_pubsub_directive(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
  ngx_str_t            *val;
  ngx_uint_t            i;
  nchan_publisher_directive_parse(cf, cmd, conf, 0);
  nchan_subscriber_directive_parse(cf, cmd, conf, 0);
  for(i=1; i < cf->args->nelts; i++) {
    val = &((ngx_str_t *) cf->args->elts)[i];
    if(! nchan_strmatch(val, 
      WEBSOCKET_STRINGS_N + EVENTSOURCE_STRINGS_N + HTTP_CHUNKED_STRINGS_N + HTTP_MULTIPART_STRINGS_N + LONGPOLL_STRINGS_N + INTERVALPOLL_STRINGS_N + DISABLED_STRINGS_N,
      WEBSOCKET_STRINGS, EVENTSOURCE_STRINGS, HTTP_CHUNKED_STRINGS, HTTP_MULTIPART_STRINGS, LONGPOLL_STRINGS_N, INTERVALPOLL_STRINGS_N, DISABLED_STRINGS_N)) {
      ngx_conf_log_error(NGX_LOG_ERR, cf, 0, "invalid %V value: %V", &cmd->name, val);
      return NGX_CONF_ERROR;
    }
  }
  return nchan_setup_handler(cf, conf, &nchan_pubsub_handler);
}

static char *nchan_subscriber_first_message_directive(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
  nchan_loc_conf_t   *lcf = (nchan_loc_conf_t *)conf;
  ngx_str_t          *val = &((ngx_str_t *) cf->args->elts)[1];
  if(nchan_strmatch(val, 1, "oldest")) {
    lcf->subscriber_start_at_oldest_message = 1;
  }
  else if(nchan_strmatch(val, 1, "newest")) {
    lcf->subscriber_start_at_oldest_message = 0;
  }
  else {
    ngx_conf_log_error(NGX_LOG_ERR, cf, 0, "invalid %V value: %V, must be either 'oldest' or 'newest'", &cmd->name, val);
    return NGX_CONF_ERROR;
  }
  return NGX_CONF_OK;
}

static void nchan_exit_worker(ngx_cycle_t *cycle) {
  nchan_store_memory.exit_worker(cycle);
  if(global_redis_enabled) {
    nchan_store_redis.exit_worker(cycle);
  }
  nchan_output_shutdown();
  ngx_destroy_pool(nchan_pool); // just for this worker
#if NCHAN_SUBSCRIBER_LEAK_DEBUG
  subscriber_debug_assert_isempty();
#endif
}

static void nchan_exit_master(ngx_cycle_t *cycle) {
  nchan_store_memory.exit_master(cycle);
  if(global_redis_enabled) {
    nchan_store_redis.exit_master(cycle);
  }
  ngx_destroy_pool(nchan_pool);
}

static char *nchan_set_complex_value_array(ngx_conf_t *cf, ngx_command_t *cmd, void *conf, nchan_complex_value_arr_t *chid) {
  ngx_uint_t                          i;
  ngx_str_t                          *value;
  ngx_http_complex_value_t          **cv;
  ngx_http_compile_complex_value_t    ccv;  
  
  chid->n = cf->args->nelts - 1;
  for(i=1; i < cf->args->nelts && i <= NCHAN_COMPLEX_VALUE_ARRAY_MAX; i++) {
    value = &((ngx_str_t *) cf->args->elts)[i];
    
    cv = &chid->cv[i-1];
    *cv = ngx_palloc(cf->pool, sizeof(ngx_http_complex_value_t));
    if (*cv == NULL) {
      return NGX_CONF_ERROR;
    }
    
    ngx_memzero(&ccv, sizeof(ngx_http_compile_complex_value_t));
    ccv.cf = cf;
    ccv.value = value;
    ccv.complex_value = *cv;
    
    if (ngx_http_compile_complex_value(&ccv) != NGX_OK) {
      return NGX_CONF_ERROR;
    }
  }
  
  return NGX_CONF_OK;
}

static char *nchan_set_pub_channel_id(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
  return nchan_set_complex_value_array(cf, cmd, conf, &((nchan_loc_conf_t *)conf)->pub_chid);
}

static char *nchan_set_sub_channel_id(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
  return nchan_set_complex_value_array(cf, cmd, conf, &((nchan_loc_conf_t *)conf)->sub_chid);
}

static char *nchan_set_pubsub_channel_id(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
  return nchan_set_complex_value_array(cf, cmd, conf, &((nchan_loc_conf_t *)conf)->pubsub_chid);
}

static char *nchan_subscriber_last_message_id(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
  return nchan_set_complex_value_array(cf, cmd, conf, &((nchan_loc_conf_t *)conf)->last_message_id);
}

static char *nchan_set_channel_events_channel_id(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
  ngx_str_t                          *value = &((ngx_str_t *) cf->args->elts)[1];
  ngx_http_complex_value_t          **cv = &((nchan_loc_conf_t *)conf)->channel_events_channel_id;
  ngx_http_compile_complex_value_t    ccv;  
  
  *cv = ngx_palloc(cf->pool, sizeof(ngx_http_complex_value_t));
  if (*cv == NULL) {
    return NGX_CONF_ERROR;
  }
  
  ngx_memzero(&ccv, sizeof(ngx_http_compile_complex_value_t));
  ccv.cf = cf;
  ccv.value = value;
  ccv.complex_value = *cv;
  
  if (ngx_http_compile_complex_value(&ccv) != NGX_OK) {
    return NGX_CONF_ERROR;
  }
  
  return NGX_CONF_OK;
}

static char *nchan_store_messages_directive(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
  char    *p = conf;
  ngx_str_t *val = cf->args->elts;

  
  if (ngx_strcasecmp(val[1].data, (u_char *) "off") == 0) {
    ngx_int_t *max;
    max = (ngx_int_t *) (p + offsetof(nchan_loc_conf_t, max_messages));
    *max=0;
  }
  return NGX_CONF_OK;
}

static char *nchan_ignore_obsolete_setting(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
  ngx_conf_log_error(NGX_LOG_WARN, cf, 0, "ignoring obsolete nchan config directive '%V'.", &cmd->name);
  return NGX_CONF_OK;
}

static char *nchan_ignore_subscriber_concurrency(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
  ngx_str_t          *val = &((ngx_str_t *) cf->args->elts)[1];
  if(!nchan_strmatch(val, 1, "broadcast")) {
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0, "ignoring obsolete nchan config directive '%V %V;'. Only 'broadcast' is currently supported.", &cmd->name, val);
  }
  return NGX_CONF_OK;
}

static char *ngx_conf_enable_redis(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
  char                *rc;
  ngx_flag_t          *fp;
  char                *p = conf;
  
  rc = ngx_conf_set_flag_slot(cf, cmd, conf);
  if(rc == NGX_CONF_ERROR) {
    return rc;
  }
  fp = (ngx_flag_t *) (p + cmd->offset);
  if(fp) {
    global_redis_enabled = 1;
  }
  
  return rc;
}

#include "nchan_config_commands.c" //hideous but hey, it works

static ngx_http_module_t  nchan_module_ctx = {
    nchan_add_variables,           /* preconfiguration */
    nchan_postconfig,              /* postconfiguration */
    nchan_create_main_conf,        /* create main configuration */
    NULL,                          /* init main configuration */
    NULL,                          /* create server configuration */
    NULL,                          /* merge server configuration */
    nchan_create_loc_conf,         /* create location configuration */
    nchan_merge_loc_conf,          /* merge location configuration */
};

ngx_module_t  nchan_module = {
    NGX_MODULE_V1,
    &nchan_module_ctx,             /* module context */
    nchan_commands,                /* module directives */
    NGX_HTTP_MODULE,               /* module type */
    NULL,                          /* init master */
    nchan_init_module,             /* init module */
    nchan_init_worker,             /* init process */
    NULL,                          /* init thread */
    NULL,                          /* exit thread */
    nchan_exit_worker,             /* exit process */
    nchan_exit_master,             /* exit master */
    NGX_MODULE_V1_PADDING
};
