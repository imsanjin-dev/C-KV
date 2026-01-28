#include"../include/bst.h"
#include"../include/c_kv.h"
#include<string.h>

static KVSNode* bst_insert_recursive(KVSNode* currentNode,KVSNode* theNode){
    if (currentNode)
    {
        int cmp=strcmp(theNode->key,currentNode->key);
        if (cmp>0)
        {
            currentNode->bst_right=bst_insert_recursive(currentNode->bst_right,theNode);
        }else if (cmp<0)
        {
            currentNode->bst_left=bst_insert_recursive(currentNode->bst_left,theNode);
        }
        return currentNode;
    }else{
        return theNode;
    }
}
void bst_insert(KVS* kvs, KVSNode* node){
    kvs->bstRoot=bst_insert_recursive(kvs->bstRoot,node);
}

static KVSNode* bst_delete_removeMin(KVSNode* currentNode,KVSNode** outMin){
    if (currentNode->bst_left)
    {
        currentNode->bst_left=bst_delete_removeMin(currentNode->bst_left,outMin);
        return currentNode;
    }else{
        *outMin=currentNode;
        return currentNode->bst_right;
    }
}
static KVSNode* bst_delete_recursive(KVSNode* currentNode,KVSNode* theNode){
    if (currentNode)
    {
        int cmp=strcmp(theNode->key,currentNode->key);
        if (cmp>0)
        {
            currentNode->bst_right=bst_delete_recursive(currentNode->bst_right,theNode);
            return currentNode;
        }else if (cmp<0)
        {
            currentNode->bst_left=bst_delete_recursive(currentNode->bst_left,theNode);
            return currentNode;
        }else{
            if (currentNode->bst_left==NULL&&currentNode->bst_right==NULL)
            {
                return NULL;
            }else if (currentNode->bst_left==NULL&&currentNode->bst_right!=NULL)
            {
                return currentNode->bst_right;
            }else if (currentNode->bst_left!=NULL&&currentNode->bst_right==NULL)
            {
                return currentNode->bst_left;
            }else{
                KVSNode* newCurrentNode=NULL;
                currentNode->bst_right=bst_delete_removeMin(currentNode->bst_right,&newCurrentNode);
                newCurrentNode->bst_left=currentNode->bst_left;
                newCurrentNode->bst_right=currentNode->bst_right;
                return newCurrentNode;
            }
        }   
    }
    return NULL;
}
void bst_delete(KVS* kvs, KVSNode* node){
    kvs->bstRoot=bst_delete_recursive(kvs->bstRoot,node);
}


void bst_print_keys(KVSNode* root) {
    if (root == NULL) {
        return;
    }
    bst_print_keys(root->bst_left);
    
    printf("%s\n", root->key);
    
    bst_print_keys(root->bst_right);
}