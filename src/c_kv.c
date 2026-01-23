#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "c_kv.h"
#include "utils.h"

struct KVSNode
{
    char* key;
    char* value;
    KVSNode* next;
};

struct KVS{
    KVSNode* head;
    int count;
};

int kvs_get_count(KVS* kvs){
    return kvs->count;
}

KVS* kvs_create(){
    KVS* newKVS=(KVS*)malloc(sizeof(KVS));
    if (newKVS==NULL)
    {
        return NULL;
    }
    newKVS->count=0;
    newKVS->head=NULL;
    return newKVS;
}
void kvs_destroy(KVS* kvs){
    if (kvs==NULL)
    {
        return;
    }
    KVSNode* current=kvs->head;
    while (current!=NULL)
    {
        kvs->head=kvs->head->next;
        free(current->key);
        free(current->value);
        free(current);
        current=kvs->head;
    }
    free(kvs);
}

SYS_STATUS kvs_put(KVS* kvs, const char* key, const char* value) {
    if (kvs == NULL || key == NULL || value == NULL) {
        return SS_INVALID_PARAM;
    }

    char* newValue = str_copy(value);
    if (newValue == NULL) {
        return SS_OUT_OF_MEMORY;
    }

    KVSNode* p = kvs->head;
    while (p != NULL) {
        if (strcmp(p->key, key) == 0) {


            free(p->value); 

            p->value = newValue; 

            return SS_SUCCESS; 
        }
        p = p->next;
    }

    KVSNode* newNode = (KVSNode*)malloc(sizeof(KVSNode));
    if (newNode == NULL) {
        free(newValue);
        return SS_OUT_OF_MEMORY;
    }

    char* newKey = str_copy(key);
    if (newKey == NULL) {
        free(newNode);
        free(newValue); 
        return SS_OUT_OF_MEMORY;
    }

    
    newNode->key = newKey;
    newNode->value = newValue;
    newNode->next = kvs->head; 
    kvs->head = newNode;       
    kvs->count++;

    return SS_SUCCESS;
}
char* kvs_get(KVS* kvs,const char* key){
    if (kvs==NULL)
    {
        return NULL;
    }
    KVSNode* p=kvs->head;
    while (p!=NULL)
    {
        if (strcmp(p->key,key)==0)
        {
            return p->value;
        }
        p=p->next;
    }
    return NULL;
};
SYS_STATUS kvs_delete(KVS* kvs,const char* key){
    if (kvs==NULL||key==NULL)
    {
        return SS_INVALID_PARAM;
    }
    KVSNode* pre=kvs->head;
    KVSNode* current=kvs->head;
    while (current!=NULL)
    {
        if (strcmp(current->key,key)==0)
        {
            if (current==kvs->head)
            {
                kvs->head=kvs->head->next;
            }else{
                pre->next=current->next;
            }
            free(current->key);
            free(current->value);
            free(current);
            kvs->count--;
            return SS_SUCCESS;
        }
        pre=current;
        current=current->next; 
    }
    return SS_KEY_NOT_FOUND;
}