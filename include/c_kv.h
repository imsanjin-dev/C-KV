#ifndef C_KV_H
#define C_KV_H

#include"common.h"

#define TABLE_SIZE 128
#define MAXCAPACITY 100000

typedef struct KVS KVS;
typedef struct KVSNode KVSNode;

int kvs_get_count(KVS* kvs);
int kvs_get_bucket_count(KVS* kvs);

KVS* kvs_create();
void kvs_destroy(KVS* kvs);

SYS_STATUS kvs_put(KVS* kvs,const char* key,const char* value);
char* kvs_get(KVS* kvs,const char* key);
SYS_STATUS kvs_delete(KVS* kvs,const char* key);

#endif