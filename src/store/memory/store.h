#ifndef NCHAN_MEMSTORE_H
#define NCHAN_MEMSTORE_H
#include "uthash.h"

extern nchan_store_t  nchan_store_memory;

ngx_int_t msg_reserve(nchan_msg_t *msg, char *lbl);
ngx_int_t msg_release(nchan_msg_t *msg, char *lbl);

#endif //NCHAN_MEMSTORE_H
