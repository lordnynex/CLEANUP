// Taken from agenzh's echo nginx module
// https://github.com/openresty/echo-nginx-module
// Thanks, agenzh!

/*
Copyright (C) 2009-2014, Yichun "agentzh" Zhang <agentzh@gmail.com>.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <nchan_module.h>

ngx_str_t  nchan_content_length_header_key = ngx_string("Content-Length");

static ngx_inline ngx_uint_t nchan_hash_str(u_char *src, size_t n) {
  ngx_uint_t  key;
  key = 0;
  while (n--) {
    key = ngx_hash(key, *src);
    src++;
  }
  return key;
}
#define nchan_hash_literal(s)                                        \
  nchan_hash_str((u_char *) s, sizeof(s) - 1)

static ngx_int_t nchan_set_content_length_header(ngx_http_request_t *r, off_t len, u_char *p) {
  ngx_table_elt_t                 *h, *header;
  ngx_list_part_t                 *part;
  ngx_http_request_t              *pr;
  ngx_uint_t                       i;
  static ngx_uint_t                nchan_content_length_hash = 0;
  if(nchan_content_length_hash == 0) {
    nchan_content_length_hash = nchan_hash_literal("content-length");
  }
  
  r->headers_in.content_length_n = len;
  
  if (ngx_list_init(&r->headers_in.headers, r->pool, 20, sizeof(ngx_table_elt_t)) != NGX_OK) {
    return NGX_ERROR;
  }

  h = ngx_list_push(&r->headers_in.headers);
  if (h == NULL) {
    return NGX_ERROR;
  }

  h->key = nchan_content_length_header_key;
  h->lowcase_key= (u_char *)"content-length";

  r->headers_in.content_length = h;
  if(p == NULL) {
    p = ngx_palloc(r->pool, NGX_OFF_T_LEN);
    if (p == NULL) {
      return NGX_ERROR;
    }
  }

  h->value.data = p;
  h->value.len = ngx_sprintf(h->value.data, "%O", len) - h->value.data;

  h->hash = nchan_content_length_hash;

  pr = r->parent;

  if (pr == NULL) {
    return NGX_OK;
  }

  /* forward the parent request's all other request headers */

  part = &pr->headers_in.headers.part;
  header = part->elts;

  for (i = 0; /* void */; i++) {
    
    if (i >= part->nelts) {
      if (part->next == NULL) {
        break;
      }
      part = part->next;
      header = part->elts;
      i = 0;
    }
    if (header[i].key.len == sizeof("Content-Length") - 1 
        && ngx_strncasecmp(header[i].key.data, (u_char *) "Content-Length", sizeof("Content-Length") - 1) == 0) {
      continue;
    }
    h = ngx_list_push(&r->headers_in.headers);
    if (h == NULL) {
      return NGX_ERROR;
    }
    *h = header[i];
  }

  /* XXX maybe we should set those built-in header slot in
    * ngx_http_headers_in_t too? */

  return NGX_OK;
}

ngx_int_t nchan_adjust_subrequest(ngx_http_request_t *sr, ngx_uint_t method, ngx_str_t *method_name, ngx_http_request_body_t *request_body, size_t content_length_n, u_char *content_len_str) {
  //ngx_http_core_main_conf_t  *cmcf;
  ngx_http_request_t         *r;
  ngx_http_request_body_t    *body;
  ngx_int_t                   rc;

  sr->method = method;
  sr->method_name = *method_name;

  if (sr->method == NGX_HTTP_HEAD) {
    sr->header_only = 1;
  }
  r = sr->parent;

  sr->header_in = r->header_in;

  /* XXX work-around a bug in ngx_http_subrequest */
  if (r->headers_in.headers.last == &r->headers_in.headers.part) {
    sr->headers_in.headers.last = &sr->headers_in.headers.part;
  }

  
  /* we do not inherit the parent request's variables */
  //scratch that, let's inherit those vars
  //cmcf = ngx_http_get_module_main_conf(sr, ngx_http_core_module);
  //sr->variables = ngx_pcalloc(sr->pool, cmcf->variables.nelts * sizeof(ngx_http_variable_value_t));

  if (sr->variables == NULL) {
    return NGX_ERROR;
  }

  if ((body = request_body)!=NULL) {
    sr->request_body = body;

    rc = nchan_set_content_length_header(sr, content_length_n, content_len_str);
    
    if (rc != NGX_OK) {
      return NGX_ERROR;
    }
  }

  //dd("subrequest body: %p", sr->request_body);

  return NGX_OK;
}