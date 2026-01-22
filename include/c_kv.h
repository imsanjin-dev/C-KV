#ifndef C_KV_H
#define C_KV_H

#include"common.h"

typedef struct CKV CKV;

CKV* kvs_create();
void kvs_destroy(CKV* kvs);

SYS_STATUS kvs_put(CKV* kvs,const char* key,const char* value);
char* kvs_get(CKV* kvs,const char* key);
SYS_STATUS kvs_delete(CKV* kvs,const char* key);

#endif