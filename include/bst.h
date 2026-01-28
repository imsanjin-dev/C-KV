#ifndef BST_H
#define BST_H

#include "c_kv.h" 

void bst_insert(KVS* kvs, KVSNode* node);
void bst_delete(KVS* kvs, KVSNode* node);

void bst_print_keys(KVSNode* root);

#endif