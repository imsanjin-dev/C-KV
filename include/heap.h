#ifndef HEAP_H
#define HEAP_H

#include<time.h>
#include"common.h"

typedef struct HeapNode{
    char* key;
    time_t expireTime; //  过期时间
}HeapNode;
typedef struct MinHeap{
    HeapNode* array;
    int currentSize;
    int capacity;
}MinHeap;

MinHeap* heap_create(int capacity);
void heap_destroy(MinHeap* heap);
SYS_STATUS heap_insert(MinHeap* heap,const char* key,time_t expireTime);
SYS_STATUS heap_remove_min(MinHeap* heap,char** out_key);
HeapNode* heap_get_min(MinHeap* heap);

#endif