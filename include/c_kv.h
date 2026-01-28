#ifndef C_KV_H
#define C_KV_H

#include"common.h"

#define TABLE_SIZE 128
#define DEFAULT_CAPACITY 100000

typedef struct KVS KVS;
typedef struct KVSNode KVSNode;
struct KVSNode
{
    char* key;
    char* value;
    KVSNode* next;
    KVSNode* lru_prev;  // LRU 双向链表的前驱
    KVSNode* lru_next;  // LRU 双向链表的后继 
    KVSNode* bst_left;  // BST 左子树
    KVSNode* bst_right; // BST 右子树
};
struct KVS{
    KVSNode** buckets;
    int itemCount;
    int bucketCount;
    KVSNode* lruHead;   // 指向最新鲜的数据 
    KVSNode* lruTail;   // 指向最陈旧的数据 
    int maxCapacity;    // 允许存储的最大节点数
    KVSNode* bstRoot;   // 二叉搜索树根节点
};

/*  保存格式
    [ 4字节整数 ][  N字节字符串  ][ 4字节整数 ][  M字节字符串  ]
    |  Key长度  |   Key的内容    |  Value长度 |  Value的内容  | */

KVS* kvs_create(int maxCapacity);
void kvs_save(KVS* kvs);
void kvs_destroy(KVS* kvs);

SYS_STATUS kvs_put(KVS* kvs,const char* key,const char* value);
char* kvs_get(KVS* kvs,const char* key);
SYS_STATUS kvs_delete(KVS* kvs,const char* key);

#endif