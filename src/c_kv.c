#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "c_kv.h"
#include "utils.h"
#include "bst.h"

KVS* kvs_create(int maxCapacity){
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

    newKVS->lruHead = NULL;
    newKVS->lruTail = NULL;

    newKVS->bstRoot = NULL;

    newKVS->maxCapacity = maxCapacity>0?maxCapacity:DEFAULT_CAPACITY; 

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
static void kvs_save_recursive(KVSNode* root, FILE* fp){
    if (!root)
    {
        return;
    }
    int keyLen=strlen(root->key);
    int valLen=strlen(root->value);
    fwrite(&keyLen,sizeof(int),1,fp);
    fwrite(root->key,sizeof(char),keyLen,fp);
    fwrite(&valLen,sizeof(int),1,fp);
    fwrite(root->value,sizeof(char),valLen,fp);
    kvs_save_recursive(root->bst_left,fp);
    kvs_save_recursive(root->bst_right,fp);
}
void kvs_save(KVS* kvs, const char* filename){
    if (kvs==NULL||filename==NULL)
    {
        return;
    }
    FILE* fp=fopen(filename,"wb");
    if (fp == NULL) {
        perror("Failed to open file for save");
        return;
    }
    char magic[4]="CKV1";
    fwrite(magic,sizeof(char),4,fp);
    kvs_save_recursive(kvs->bstRoot, fp);
    fclose(fp);
    printf(">> DB Saved to '%s' (Format: CKV1)\n", filename);
}
void kvs_load(KVS* kvs, const char* filename){
    if (kvs==NULL||filename==NULL)
    {
        return;
    }
    FILE* fp=fopen(filename,"rb");
    if (fp == NULL) {
        perror("Failed to open file for load");
        return;
    }
    char magic[4];
    if (fread(magic, sizeof(char), 4, fp) != 4) {
        printf("File is too short or empty.\n");
        fclose(fp);
        return;
    }
    if (memcmp(magic, "CKV1", 4) != 0) {
        printf("Invalid file format. Magic number mismatch.\n");
        fclose(fp); 
        return;
    }
    while (1)
    {
        int keyLen=0;
        if (fread(&keyLen,sizeof(int),1,fp)!=1)
        {
            break;
        }
        if (keyLen<0)
        {
            break;
        }
        char* key=(char*)malloc((keyLen+1)*sizeof(char));
        if (fread(key,sizeof(char),keyLen,fp)!=keyLen)
        {
            free(key);
            break;
        }
        key[keyLen]='\0';
        int valLen=0;
        if (fread(&valLen,sizeof(int),1,fp)!=1)
        {
            free(key);
            break;
        }
        if (valLen<0)
        {
            free(key);
            break;
        }
        char* value=(char*)malloc((valLen+1)*sizeof(char));
        if (fread(value,sizeof(char),valLen,fp)!=valLen)
        {
            free(key);
            free(value);
            break;
        }
        value[valLen]='\0';
        kvs_put(kvs,key,value);
        free(key);
        free(value);
    }
    fclose(fp);
    return; 
}

static void lru_unlink_node(KVS* kvs, KVSNode* node){
    if (node->lru_prev)
    {
        node->lru_prev->lru_next=node->lru_next;
    }else{
        kvs->lruHead=node->lru_next;
        if (kvs->lruHead)
        {
            kvs->lruHead->lru_prev=NULL;
        } 
    }
    if (node->lru_next)
    {
        node->lru_next->lru_prev=node->lru_prev;
    }else{
        kvs->lruTail=node->lru_prev;
        if (kvs->lruTail)
        {
            kvs->lruTail->lru_next=NULL;
        }
    }
    node->lru_prev=NULL;
    node->lru_next=NULL;
}
static void lru_link_node(KVS* kvs, KVSNode* node){
    node->lru_prev=NULL;
    node->lru_next=NULL;
    if (kvs->lruHead)
    {
        node->lru_next=kvs->lruHead;
        kvs->lruHead->lru_prev=node;
        kvs->lruHead=node;
    }else{
        kvs->lruHead=node;
        kvs->lruTail=node;
    }
}

static void kvs_resize(KVS* kvs,int newSize){
    KVSNode** newBucket=(KVSNode**)calloc(newSize,sizeof(KVSNode*));
    if (!newBucket)
    {
        return;
    }
    //currentBucket,当前正在复制的Bucket
    KVSNode** currentBucket=kvs->buckets;
    for (int i = 0; i < kvs->bucketCount; i++)
    {
        while (currentBucket[i])
        {
            int newI=hash(currentBucket[i]->key)%newSize;
            KVSNode* theNext=currentBucket[i]->next;
            currentBucket[i]->next=newBucket[newI];
            newBucket[newI]=currentBucket[i];
            currentBucket[i]=theNext;
        }
    }
    free(kvs->buckets);
    kvs->buckets=newBucket;
    kvs->bucketCount=newSize;
    return;
}

//头插法
SYS_STATUS kvs_put(KVS* kvs, const char* key, const char* value) {
    if (kvs == NULL || key == NULL || value == NULL) {
        return SS_INVALID_PARAM;
    }

    char* newValue = str_copy(value);
    if (newValue == NULL) {
        return SS_OUT_OF_MEMORY;
    }
    
    unsigned long hKey=hash(key)%kvs->bucketCount;

    KVSNode* current = kvs->buckets[hKey];
    
    while (current!=NULL)
    {
        if (strcmp(current->key,key)==0)
        {
            free(current->value);
            current->value=newValue;
            lru_unlink_node(kvs,current);
            lru_link_node(kvs,current);
            return SS_SUCCESS;
        }else{
            current=current->next;
        }  
    }

    if ((double)kvs->itemCount / kvs->bucketCount > 0.75)
    {
        kvs_resize(kvs,kvs->bucketCount*2);
        hKey=hash(key)%kvs->bucketCount;
    }

    if (kvs->itemCount>=kvs->maxCapacity)
    {
        kvs_delete(kvs,kvs->lruTail->key);
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
    newNode->lru_next=NULL;
    newNode->lru_prev=NULL;
    newNode->bst_left=NULL;
    newNode->bst_right=NULL;
    newNode->next = kvs->buckets[hKey]; 
    kvs->buckets[hKey]=newNode;
    kvs->itemCount++;
    bst_insert(kvs,newNode);
    lru_link_node(kvs,newNode);

    return SS_SUCCESS;
}
char* kvs_get(KVS* kvs,const char* key){
    if (kvs==NULL||key==NULL)
    {
        return NULL;
    }

    unsigned long hKey=hash(key)%kvs->bucketCount;

    KVSNode* current=kvs->buckets[hKey];
    while (current!=NULL)
    {
        if (strcmp(current->key,key)==0)
        {
            lru_unlink_node(kvs,current);
            lru_link_node(kvs,current);
            return current->value;
        }
        current=current->next; 
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
            lru_unlink_node(kvs,current);
            bst_delete(kvs,current);
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