#ifndef C_KV_H
#define C_KV_H

#include"common.h"

typedef struct KVS KVS;
typedef struct KVSNode KVSNode;

int kvs_get_count(KVS* kvs);

KVS* kvs_create();
void kvs_destroy(KVS* kvs);

SYS_STATUS kvs_put(KVS* kvs,const char* key,const char* value);
char* kvs_get(KVS* kvs,const char* key);
SYS_STATUS kvs_delete(KVS* kvs,const char* key);

#endif