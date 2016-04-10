#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "mongoc.h"

typedef struct {
  ngx_str_t     mongo_server_addr;
  ngx_str_t     mongo_port;
  ngx_str_t     mongo_database;
  ngx_str_t     mongo_collection;
  ngx_str_t     mongo_user;
  ngx_str_t     mongo_passw;
  bson_t        unreadable_fields;
} ngx_http_mg_conf_t;

typedef struct {
  ngx_str_t     error_msg;
  ngx_int_t     error_code;
} error_s;

static ngx_int_t ngx_http_mg_handler(ngx_http_request_t *);
static char* ngx_http_mg(ngx_conf_t *, ngx_command_t *, void *);
static void* ngx_http_mg_create_loc_conf(ngx_conf_t *);
static char* ngx_http_mg_merge_loc_conf(ngx_conf_t *, void *, void *);
static ngx_int_t ngx_http_mg_handle_get_request(ngx_http_request_t*, ngx_http_mg_conf_t*, ngx_http_complex_value_t*, error_s*);
void ngx_http_mg_handle_post_request(ngx_http_request_t*);
static mongoc_uri_t* ngx_http_create_mongo_conn_uri(char*, char*, char*, char*);
static char* ngx_http_mg_unreadable_fields(ngx_conf_t*, ngx_command_t*, void*);


static ngx_str_t application_type = ngx_string("application/json");

static ngx_command_t ngx_http_mg_commands[] = {
  {
    ngx_string("mongo"),
    NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS|NGX_CONF_TAKE1,
    ngx_http_mg,
    NGX_HTTP_LOC_CONF_OFFSET,
    offsetof(ngx_http_mg_conf_t, mongo_server_addr),
    NULL
  },
  {
    ngx_string("mongo_port"),
    NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
    ngx_conf_set_str_slot,
    NGX_HTTP_LOC_CONF_OFFSET,
    offsetof(ngx_http_mg_conf_t, mongo_port),
    NULL
  },
  {
    ngx_string("mongo_database"),
    NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
    ngx_conf_set_str_slot,
    NGX_HTTP_LOC_CONF_OFFSET,
    offsetof(ngx_http_mg_conf_t, mongo_database),
    NULL
  },
  {
    ngx_string("mongo_collection"),
    NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
    ngx_conf_set_str_slot,
    NGX_HTTP_LOC_CONF_OFFSET,
    offsetof(ngx_http_mg_conf_t, mongo_collection),
    NULL
  },
  { ngx_string("mongo_user"),
    NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
    ngx_conf_set_str_slot,
    NGX_HTTP_LOC_CONF_OFFSET,
    offsetof(ngx_http_mg_conf_t, mongo_user),
    NULL
  },

  { ngx_string("mongo_passw"),
    NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
    ngx_conf_set_str_slot,
    NGX_HTTP_LOC_CONF_OFFSET,
    offsetof(ngx_http_mg_conf_t, mongo_passw),
    NULL
  },
  { ngx_string("mongo_unreadable_field"),
    NGX_HTTP_LOC_CONF|NGX_CONF_TAKE12, //Up to 12 unreadable fields
    ngx_http_mg_unreadable_fields,
    NGX_HTTP_LOC_CONF_OFFSET,
    0,
    NULL
  },

  ngx_null_command
};

static ngx_http_module_t ngx_http_mg_module_ctx = {
  NULL,
  NULL, //postconfiguration

  NULL,
  NULL,

  NULL,
  NULL,

  ngx_http_mg_create_loc_conf,
  ngx_http_mg_merge_loc_conf
};

ngx_module_t ngx_http_mg_module = {
  NGX_MODULE_V1,
  &ngx_http_mg_module_ctx,
  ngx_http_mg_commands,
  NGX_HTTP_MODULE,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NGX_MODULE_V1_PADDING
};

static ngx_int_t
ngx_http_mg_handler(ngx_http_request_t *r)
{
    ngx_int_t    rc;
    ngx_http_mg_conf_t  *mgcf;
    ngx_http_complex_value_t cv;
    error_s* error = ngx_palloc(r->pool, sizeof(error_s));

    //Let's assume we can handle the following.
    //GET will get
    //POST will create/update
    //Todo: What about PUT?
    if (!(r->method & (NGX_HTTP_GET|NGX_HTTP_POST))) {
        return NGX_HTTP_NOT_ALLOWED;
    }

    if (r->headers_in.if_modified_since) {
        return NGX_HTTP_NOT_MODIFIED;
    }

    mgcf = ngx_http_get_module_loc_conf(r, ngx_http_mg_module);

    //If we've got a GET request, process it
    if (r->method == NGX_HTTP_GET) {
      if(ngx_http_mg_handle_get_request(r, mgcf, &cv, error) == NGX_ERROR) {
        ngx_memzero(&cv, sizeof(ngx_http_complex_value_t));
        cv.value.len = error->error_msg.len;
        cv.value.data = error->error_msg.data;

        return ngx_http_send_response(r, error->error_code, &application_type, &cv);
      }
    }

    if (r->method == NGX_HTTP_POST) {
      //Figure out if we want to handle this post request..
      if(strcmp(r->headers_in.content_type->value.data, application_type.data) != 0) {
        cv.value.data = "{ \"ok\" : false, \"reason\" : \"Incorrect Media Type.\" }";
        cv.value.len = strlen(cv.value.data);
        return ngx_http_send_response(r, NGX_HTTP_UNSUPPORTED_MEDIA_TYPE, &application_type, &cv);
      }

      //This will process as many times as it needs to.
      //If it's not done, rc will return NGX_AGAIN. In that case,
      //let's come back to this at a later date.
      //Else, our post handler will execute
      rc = ngx_http_read_client_request_body(r, ngx_http_mg_handle_post_request);
      if (rc == NGX_AGAIN) {
        return NGX_AGAIN;
      }
    }

    return ngx_http_send_response(r, NGX_HTTP_OK, &application_type, &cv);
}

//Build out the complex value from our query and modify our
//request appropriately (handle the application type, etc).
static ngx_int_t
ngx_http_mg_handle_get_request(ngx_http_request_t *r, ngx_http_mg_conf_t* mgcf, ngx_http_complex_value_t* cv, error_s* ngx_mg_req_error) {
    //Mongoc related vars
    mongoc_client_t *client;
    mongoc_collection_t *collection;
    mongoc_cursor_t *cursor;

    //Dealing with bson here.
    bson_error_t error;
    const bson_t *doc;
    bson_t doc_response = BSON_INITIALIZER;
    bson_t query;
    bson_t find;

    mongoc_uri_t* uri;
    int count;
    ngx_str_t q;
    char* response;

    //Query vars.
    char* field;
    char* value;
    ngx_str_t offset = ngx_string("0");
    int offsetI;
    char* offsetA;
    ngx_str_t limit = ngx_string("0");
    int limitI;
    char* limitA;

    uri = ngx_http_create_mongo_conn_uri(mgcf->mongo_server_addr.data, mgcf->mongo_port.data, mgcf->mongo_user.data, mgcf->mongo_passw.data);

    //Init our client and connect
    mongoc_init();
    client = mongoc_client_new_from_uri(uri); //Connect to db. Let's assume for the moment all actions require connection.

    bson_init( &query ); //init bson

    //https://github.com/mongodb/mongo-c-driver/blob/master/doc/mongoc_collection_find.txt
    collection = mongoc_client_get_collection(client, mgcf->mongo_database.data, mgcf->mongo_collection.data); //Get our requested collection

    //Also a 500
    if(!collection) {
      ngx_mg_req_error->error_msg = (ngx_str_t)ngx_string("{ \"ok\" : false, \"reason\" : \"Unable to get collection.\" }");
      ngx_mg_req_error->error_code = NGX_HTTP_INTERNAL_SERVER_ERROR;
      return NGX_ERROR;
    }

    //Before we find, let's see if we're querying for anything specific.
    //The query will come in the form of ?q=field:value
    if(ngx_http_arg(r, (u_char *) "q", 1, &q) == NGX_OK) {
      u_char* ptr = strchr(q.data, ':' ); //Find our dividing character.
      int index = ptr - q.data; //The index of :
      field = ngx_palloc(r->pool, index);
      value = ngx_palloc(r->pool, q.len - index - 1);
      strncpy(field, q.data, index);
      strncpy(value, q.data + index + 1, q.len - index - 1);
      bson_append_regex(&query, field, index, value, "i");
    }

    //Do we need an offset?
    ngx_http_arg(r, (u_char *) "offset", 6, &offset);

    //What about a limit?
    ngx_http_arg(r, (u_char *) "limit",  5, &limit);

    offsetI = ngx_atoi(offset.data, offset.len); //Convert offset to int
    limitI = ngx_atoi(limit.data, limit.len); //Convert limit to int

    //Todo: This needs a check to determine if there's memory left?
    limitA = ngx_palloc(r->pool, limit.len);
    offsetA = ngx_palloc(r->pool, offset.len);
    //Format our offset and limit.
    snprintf(offsetA, offset.len + 1, "%s", offset.data); //Offset as string + null char
    snprintf(limitA,  limit.len + 1, "%s", limit.data); //Offset as string + null char
    //Before we query, append our document of unreadable fields (if there are any)

    //If we don't have any unreadable fields, make this null.
    if(mgcf->unreadable_fields.len > 0) {
      cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, offsetI, limitI, 0, &query, &mgcf->unreadable_fields, NULL);
    } else {
      cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, offsetI, limitI, 0, &query, NULL, NULL);
    }

    if(mongoc_cursor_error(cursor, &error)) {
      ngx_mg_req_error->error_msg = (ngx_str_t)ngx_string("{ \"ok\" : false, \"reason\" : \"There was a problem connecting to the mongo cluster.\" }");
      ngx_mg_req_error->error_code = NGX_HTTP_INTERNAL_SERVER_ERROR;
      return NGX_ERROR;
    }

    while(!mongoc_cursor_error(cursor, &error) && mongoc_cursor_more(cursor)) {
      if(mongoc_cursor_next(cursor, &doc)) {
        bson_concat(&doc_response, doc); //Concatenate the bson documents into one large document.
      }
    }

    if(mongoc_cursor_error(cursor, &error)) {
      ngx_mg_req_error->error_msg = (ngx_str_t)ngx_string("{ \"ok\" : false, \"reason\" : \"There was a problem with the Mongo Cursor.\" }");
      ngx_mg_req_error->error_code = NGX_HTTP_INTERNAL_SERVER_ERROR;
      return NGX_ERROR;
    }

    char* str = bson_as_json(&doc_response, NULL);
    count = mongoc_collection_count(collection, MONGOC_QUERY_NONE, &query, offsetI, limitI, NULL, &error);
    //Ok, need a large enough string to contain our docs + extra info.
    response = (char*)ngx_palloc(r->pool, strlen(str) + 49 + limit.len + offset.len + sizeof(count));

    //If we couldn't allocate, fail.
    if( !response ) {
      ngx_mg_req_error->error_msg = (ngx_str_t)ngx_string("{ \"ok\" : false, \"reason\" : \"Unable to allocate enough memory for a response.\" }");
      ngx_mg_req_error->error_code = NGX_HTTP_INTERNAL_SERVER_ERROR;
      return NGX_ERROR;
    }

    //Get rid of strncpy, we're well aware of the size being allocated.
    sprintf(response, "{\"q_results\" : [ %s ], count: %d, limit: %s, offset: %s }", str, count, limitA, offsetA);

    bson_free(str); //Free up str.

    bson_destroy(&query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);

    ngx_memzero(cv, sizeof(ngx_http_complex_value_t));

    cv->value.len = strlen(response);
    cv->value.data = response;

    return NGX_OK;
}

//Handle posting to Mongo.
void
ngx_http_mg_handle_post_request(ngx_http_request_t* r)  {
    //Since this will just be called as handler, get our conf information.
    ngx_http_mg_conf_t* mgcf = ngx_http_get_module_loc_conf(r, ngx_http_mg_module);
    ngx_http_complex_value_t cv;

    //Mongoc related vars
    mongoc_client_t *client;
    mongoc_collection_t *collection;

    ngx_int_t rc;
    bool inserted;

    //Dealing with bson here.
    char* response;
    mongoc_uri_t* uri;
    bson_error_t error;
    bson_t* post_data;

    //Init our client and connect
    mongoc_init();

    uri = ngx_http_create_mongo_conn_uri(mgcf->mongo_server_addr.data, mgcf->mongo_port.data, mgcf->mongo_user.data, mgcf->mongo_passw.data);
    client = mongoc_client_new_from_uri(uri); //Connect to db. Let's assume for the moment all actions require connection.

    if(!client) {
      ngx_memzero(&cv, sizeof(ngx_http_complex_value_t));
      cv.value.data = "{ \"ok\" : false, \"reason\" : \"Could not authenticate.\" }";
      cv.value.len = strlen(cv.value.data);
      ngx_http_send_response(r, NGX_HTTP_UNAUTHORIZED, &application_type, &cv);
    }

    collection = mongoc_client_get_collection(client, mgcf->mongo_database.data, mgcf->mongo_collection.data); //Get our requested collection

    if(!collection) {
      ngx_memzero(&cv, sizeof(ngx_http_complex_value_t));
      cv.value.data = "{ \"ok\" : false, \"reason\" : \"Unable to get collection.\" }";
      cv.value.len = strlen(cv.value.data);
      ngx_http_send_response(r, NGX_HTTP_INTERNAL_SERVER_ERROR, &application_type, &cv);
    }

    //Fetch posted data and digest it, then attempt to upsert the data.
    post_data = bson_new_from_json(r->request_body->bufs->buf->pos, strlen(r->request_body->bufs->buf->pos), &error);
    //If not able to create bson from our json
    if(!post_data) {
      ngx_memzero(&cv, sizeof(ngx_http_complex_value_t));
      cv.value.data = "{ \"ok\" : false, \"reason\" : \"Parsing error. Unable to create bson from json.\" }";
      cv.value.len = strlen(cv.value.data);
      ngx_http_send_response(r, NGX_HTTP_INTERNAL_SERVER_ERROR, &application_type, &cv);
    }

    inserted = mongoc_collection_update(collection, MONGOC_UPDATE_UPSERT, post_data, post_data, NULL, &error);

    //Unable to do upsert.
    if(!inserted) {
      ngx_memzero(&cv, sizeof(ngx_http_complex_value_t));
      cv.value.data = "{ \"ok\" : false, \"reason\" : \"Unable to upsert document.\" }";
      cv.value.len = strlen(cv.value.data);
      ngx_http_send_response(r, NGX_HTTP_INTERNAL_SERVER_ERROR, &application_type, &cv);
    }

    //Sweet. Updated correctly.
    ngx_memzero(&cv, sizeof(ngx_http_complex_value_t));
    cv.value.data = "{ \"ok\" true, \"reason\" : \"Insert/Update successful.\" }";
    cv.value.len = strlen(cv.value.data);
    ngx_http_send_response(r, NGX_HTTP_OK, &application_type, &cv);

    //Is this necessary?
    ngx_http_finalize_request(r, NGX_OK);
}

static char*
ngx_http_mg_unreadable_fields(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
  ngx_http_mg_conf_t *mgcf = conf;
  ngx_str_t *value;

  value = cf->args->elts;

  //Start our bson doc, this will be used for our unreadable fields.
  bson_init(&mgcf->unreadable_fields);

  //Per the docs, explicitly exclude certain fields by zeroin'g their value out.
  int i = 1;
  for(i; i < cf->args->nelts; i++) {
    if(!bson_append_int32(&mgcf->unreadable_fields, value[i].data, value[i].len, 0)){
      return NGX_CONF_ERROR;
    }
  }

  return NGX_CONF_OK;
}

static mongoc_uri_t*
ngx_http_create_mongo_conn_uri(char* addr, char* port, char* username, char* pass)
{
    char* uri_string;
    if(strlen(username) > 0 && strlen(pass) > 0) {
      uri_string = bson_strdup_printf("mongodb://%s:%s@%s:%s",
                                      username,
                                      pass,
                                      addr,
                                      port);
    } else if(strlen(username) > 0) {
      uri_string = bson_strdup_printf("mongodb://%s@%s:%s",
                                      username,
                                      addr,
                                      port);
    } else {
      uri_string = bson_strdup_printf("mongodb://%s:%s",
                                      addr,
                                      port);
    }

    return mongoc_uri_new(uri_string);
}


static char *
ngx_http_mg(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
  ngx_http_core_loc_conf_t *clcf;

  clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
  clcf->handler = ngx_http_mg_handler;

  return NGX_CONF_OK;
}


static void *
ngx_http_mg_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_mg_conf_t  *conf;

    conf = (ngx_http_mg_conf_t *)ngx_pcalloc(cf->pool, sizeof(ngx_http_mg_conf_t));

    if (conf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     * conf->mongo_server_addr = { 0, NULL }
     * conf->mongo_database = { 0, NULL }
     * conf->mongo_collection = { 0, NULL }
     * conf->mongo_user = { 0, NULL }
     * conf->mongo_passw = { 0, NULL }
     */

    return conf;
}

static char *
ngx_http_mg_merge_loc_conf(ngx_conf_t *cf, void * parent, void * child)
{
    ngx_http_mg_conf_t *prev = (ngx_http_mg_conf_t *) parent;
    ngx_http_mg_conf_t *conf = (ngx_http_mg_conf_t *) child;

    //Merge up all our defaults.
    ngx_conf_merge_str_value(conf->mongo_server_addr, prev->mongo_server_addr, "127.0.0.1");
    ngx_conf_merge_str_value(conf->mongo_port, prev->mongo_port, "27017");
    ngx_conf_merge_str_value(conf->mongo_database, prev->mongo_database, "test");
    ngx_conf_merge_str_value(conf->mongo_collection, prev->mongo_collection, "test");
    ngx_conf_merge_str_value(conf->mongo_user, prev->mongo_user, "");
    ngx_conf_merge_str_value(conf->mongo_passw, prev->mongo_passw, "");

    return NGX_CONF_OK;

}
