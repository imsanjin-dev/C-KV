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
    KVSNode** buckets;
    int itemCount;
    int bucketCount;
};

int kvs_get_count(KVS* kvs){
    return kvs->itemCount;
}

KVS* kvs_create(){
    KVS* newKVS=(KVS*)malloc(sizeof(KVS));
    if (newKVS==NULL)
    {
        return NULL;
    }
    KVSNode** newBuckets=(KVSNode**)calloc(TABLE_SIZE,sizeof(KVSNode*));
    if (newBuckets==NULL)
    {
        free(newKVS);
        return NULL;
    }
    newKVS->itemCount=0;
    newKVS->bucketCount=TABLE_SIZE;
    newKVS->buckets=newBuckets;
    return newKVS;
}
void kvs_destroy(KVS* kvs){
    if (kvs==NULL)
    {
        return;
    }
    KVSNode** currentBucket=kvs->buckets;
    for (int i = 0; i < kvs->bucketCount; i++)
    {
            KVSNode* currentNode=currentBucket[i];
            while (currentNode!=NULL)
            {
                KVSNode* next=currentNode->next;
                free(currentNode->key);
                free(currentNode->value);
                free(currentNode);
                currentNode=next;
            }
    }
    free(kvs->buckets);
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

    unsigned long hKey=hash(key)%kvs->bucketCount;

    KVSNode* p = kvs->buckets[hKey];
    KVSNode* prev = kvs->buckets[hKey];
    
    while (p!=NULL)
    {
        if (strcmp(p->key,key)==0)
        {
            free(p->value);
            p->value=newValue;
            return SS_SUCCESS;
        }else{
            prev=p;
            p=p->next;
        }  
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
    newNode->next = NULL; 
    if (kvs->buckets[hKey]==NULL)
    {
        kvs->buckets[hKey]=newNode;
    }else{
        prev->next=newNode;
    } 
    kvs->itemCount++;

    return SS_SUCCESS;
}
char* kvs_get(KVS* kvs,const char* key){
    if (kvs==NULL||key==NULL)
    {
        return NULL;
    }

    unsigned long hKey=hash(key)%kvs->bucketCount;

    KVSNode* p=kvs->buckets[hKey];
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

    unsigned long hKey=hash(key)%kvs->bucketCount;

    KVSNode* pre=kvs->buckets[hKey];
    KVSNode* current=kvs->buckets[hKey];
    while (current!=NULL)
    {
        if (strcmp(current->key,key)==0)
        {
            if (current==kvs->buckets[hKey])
            {
                kvs->buckets[hKey]=kvs->buckets[hKey]->next;
            }else{
                pre->next=current->next;
            }
            free(current->key);
            free(current->value);
            free(current);
            kvs->itemCount--;
            return SS_SUCCESS;
        }
        pre=current;
        current=current->next; 
    }
    return SS_KEY_NOT_FOUND;
}