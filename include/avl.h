#ifndef AVL_H
#define AVL_H

#include"c_kv.h"

void avl_insert(KVS* kvs,KVSNode* node);
void avl_delete(KVS* kvs, KVSNode* node);

void avl_print_keys(KVSNode* root);

#endif