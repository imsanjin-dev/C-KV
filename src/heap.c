#include"../include/heap.h"
#include"../include/utils.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

MinHeap* heap_create(int capacity){
    MinHeap* newHeap=(MinHeap*)malloc(sizeof(MinHeap));
    if (!newHeap)
    {
        return NULL;
    }
    HeapNode* newArray=(HeapNode*)calloc(capacity,sizeof(HeapNode));
    if (!newArray)
    {
        free(newHeap);
        return NULL;
    }
    newHeap->array=newArray;
    newHeap->currentSize=0;
    newHeap->capacity=capacity;
    return newHeap;
}
void heap_destroy(MinHeap* heap){
    if (!heap)
    {
        return;
    }
    for (int i = 0; i < heap->currentSize; i++)
    {
        free(heap->array[i].key);
    }
    free(heap->array);
    free(heap);
    return;
}

static void swap_nodes(HeapNode* node1,HeapNode* node2){
    HeapNode tmp=*node1;
    *node1 = *node2;
    *node2 = tmp;
    return;
}
static void move_up(HeapNode* array,int index){
    if (!array||index<=0)
    {
        return;
    }
    int parentIndex=(index-1)/2;
    if (array[index].expireTime>=array[parentIndex].expireTime)
    {
        return;
    }else{
        swap_nodes(&array[parentIndex],&array[index]);
        move_up(array,parentIndex);
    }
}
SYS_STATUS heap_insert(MinHeap* heap,const char* key,time_t expireTime){
    if (!heap||!key){
        return SS_INVALID_PARAM;
    }
    if (heap->currentSize >= heap->capacity) {
        int newCap = heap->capacity>0?heap->capacity*2:128;
        HeapNode* newArr = (HeapNode*)realloc(heap->array, newCap * sizeof(HeapNode));
        if (!newArr){
            return SS_OUT_OF_MEMORY;
        }
        heap->array = newArr;
        heap->capacity = newCap;
    }
    char* newKey=str_copy(key);
    if (!newKey)
    {
        return SS_OUT_OF_MEMORY;
    }
    HeapNode newNode;
    newNode.key=newKey;
    newNode.expireTime=expireTime;
    heap->array[heap->currentSize]=newNode;
    move_up(heap->array,heap->currentSize);
    heap->currentSize++;
    return SS_SUCCESS;
}
static void move_down(MinHeap* heap,int index){
    if (!heap||index>heap->currentSize-1)
    {
        return;
    }
    int smallest=index;
    int left=2*index+1;
    int right=left+1;
    if (left<=heap->currentSize-1)
    {
        smallest=(heap->array[smallest].expireTime<heap->array[left].expireTime)?smallest:left;
    }
    if (right<=heap->currentSize-1)
    {
        smallest=(heap->array[smallest].expireTime<heap->array[right].expireTime)?smallest:right;
    }
    if (smallest==index)
    {
        return;
    }else{
        swap_nodes(&heap->array[index],&heap->array[smallest]);
        move_down(heap,smallest);
    }
}
SYS_STATUS heap_remove_min(MinHeap* heap,char** out_key){
    if (!heap||heap->currentSize==0||!out_key)
    {
        return SS_INVALID_PARAM;
    }
    *out_key=heap->array[0].key;
    swap_nodes(&heap->array[0],&heap->array[heap->currentSize-1]);
    heap->currentSize--;
    move_down(heap,0);
    return SS_SUCCESS;
}
HeapNode* heap_get_min(MinHeap* heap){
    if (!heap||heap->currentSize==0)
    {
        return NULL;
    }
    return &heap->array[0];
}
