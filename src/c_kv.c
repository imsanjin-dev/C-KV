#include<stdio.h>
#include<stdlib.h>
#include"c_kv.h"

struct CKV{
    int count;
};

CKV* kvs_create(){
    CKV* newKVS=(CKV*)malloc(sizeof(CKV));
    if (newKVS==NULL)
    {
        return NULL;
    }
    newKVS->count=0;
    return newKVS;
}
void kvs_destroy(CKV* kvs){
    free(kvs);
}

SYS_STATUS kvs_put(CKV* kvs,const char* key,const char* value){
    printf("输入了key，在kvs中储存了了key对应的value\n");
    return SS_SUCCESS;
}
char* kvs_get(CKV* kvs,const char* key){
    printf("输入了key，在kvs中没找到key对应的value\n");
    return NULL;
};
SYS_STATUS kvs_delete(CKV* kvs,const char* key){
    printf("输入了key，格式错误\n");
    return SS_INVALID_PARAM;
}