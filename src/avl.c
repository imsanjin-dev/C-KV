#include"../include/avl.h"
#include"../include/c_kv.h"
#include<string.h>
#include<stdio.h>

static int node_height(KVSNode* node){
    if (!node)
    {
        return 0;
    }
    return node->avl_height;
}
static void update_height(KVSNode* node){
    if (!node)
    {
        return;
    } 
    int lh=node_height(node->avl_left);
    int rh=node_height(node->avl_right);
    node->avl_height=1+(lh>rh?lh:rh);
}
static int balance_factor(KVSNode* node){
    if (!node)
    {
        return 0;
    }
    return node_height(node->avl_left)-node_height(node->avl_right);
}
//右旋
static KVSNode* rotate_right(KVSNode* node){
    if (!node||!node->avl_left)
    {
        return node;
    }
    KVSNode* newRoot=node->avl_left;
    node->avl_left=newRoot->avl_right;
    newRoot->avl_right=node;
    update_height(node);
    update_height(newRoot);
    return newRoot;
}
//左旋
static KVSNode* rotate_left(KVSNode* node){
    if (!node||!node->avl_right)
    {
        return node;
    }
    KVSNode* newRoot=node->avl_right;
    node->avl_right=newRoot->avl_left;
    newRoot->avl_left=node;
    update_height(node);
    update_height(newRoot);
    return newRoot;
}
static KVSNode* avl_insert_recursive(KVSNode* currentNode,KVSNode* theNode){
    if (currentNode)
    {
        int cmp=strcmp(theNode->key,currentNode->key);
        if (cmp>0)
        {
            currentNode->avl_right=avl_insert_recursive(currentNode->avl_right,theNode);
        }else if (cmp<0)
        {
            currentNode->avl_left=avl_insert_recursive(currentNode->avl_left,theNode);
        }
        update_height(currentNode);
        int bf=balance_factor(currentNode);
        if (bf>1)
        {
            cmp=strcmp(theNode->key,currentNode->avl_left->key);
            if (cmp<0)
            {
                return rotate_right(currentNode);
                //LL
            }else{
                currentNode->avl_left=rotate_left(currentNode->avl_left);
                return rotate_right(currentNode);
                //LR
            }  
        }else if (bf<-1)
        {
            cmp=strcmp(theNode->key,currentNode->avl_right->key);
            if (cmp>0)
            {
                return rotate_left(currentNode);
                //RR
            }else{
                currentNode->avl_right=rotate_right(currentNode->avl_right);
                return rotate_left(currentNode);
                //RL
            }  
        }
        return currentNode;
    }else{
        return theNode;
    }
}
void avl_insert(KVS* kvs, KVSNode* node){
    kvs->avlRoot=avl_insert_recursive(kvs->avlRoot,node);
}

static KVSNode* avl_delete_removeMin(KVSNode* currentNode,KVSNode** outMin){
    if (currentNode->avl_left)
    {
        currentNode->avl_left=avl_delete_removeMin(currentNode->avl_left,outMin);
        update_height(currentNode);
        int bf=balance_factor(currentNode);
        if (bf>1)
        {
            int cmp=balance_factor(currentNode->avl_left);
            if (cmp>=0)
            {
                return rotate_right(currentNode);
                //LL
            }else{
                currentNode->avl_left=rotate_left(currentNode->avl_left);
                return rotate_right(currentNode);
                //LR
            }  
        }else if (bf<-1)
        {
            int cmp=balance_factor(currentNode->avl_right);
            if (cmp<=0)
            {
                return rotate_left(currentNode);
                //RR
            }else{
                currentNode->avl_right=rotate_right(currentNode->avl_right);
                return rotate_left(currentNode);
                //RL
            }  
        }
        return currentNode;
    }else{
        *outMin=currentNode;
        return currentNode->avl_right;
    }
}
static KVSNode* avl_delete_recursive(KVSNode* currentNode,KVSNode* theNode){
    if (currentNode)
    {
        int cmp=strcmp(theNode->key,currentNode->key);
        if (cmp>0)
        {
            currentNode->avl_right=avl_delete_recursive(currentNode->avl_right,theNode);
            update_height(currentNode);
            int bf=balance_factor(currentNode);
            if (bf>1)
            {
                int left_bf=balance_factor(currentNode->avl_left);
                if (left_bf>=0)
                {
                    return rotate_right(currentNode);
                    //LL
                }else{
                    currentNode->avl_left=rotate_left(currentNode->avl_left);
                    return rotate_right(currentNode);
                    //LR
                }  
            }else if (bf<-1)
            {
                int right_bf=balance_factor(currentNode->avl_right);
                if (right_bf<=0)
                {
                    return rotate_left(currentNode);
                    //RR
                }else{
                    currentNode->avl_right=rotate_right(currentNode->avl_right);
                    return rotate_left(currentNode);
                    //RL
                }  
            }
            return currentNode;
        }else if (cmp<0)
        {
            currentNode->avl_left=avl_delete_recursive(currentNode->avl_left,theNode);
            update_height(currentNode);
            int bf=balance_factor(currentNode);
            if (bf>1)
            {
                int left_bf=balance_factor(currentNode->avl_left);
                if (left_bf>=0)
                {
                    return rotate_right(currentNode);
                    //LL
                }else{
                    currentNode->avl_left=rotate_left(currentNode->avl_left);
                    return rotate_right(currentNode);
                    //LR
                }  
            }else if (bf<-1)
            {
                int right_bf=balance_factor(currentNode->avl_right);
                if (right_bf<=0)
                {
                    return rotate_left(currentNode);
                    //RR
                }else{
                    currentNode->avl_right=rotate_right(currentNode->avl_right);
                    return rotate_left(currentNode);
                    //RL
                }  
            }
            return currentNode;
        }else{
            if (currentNode->avl_left==NULL&&currentNode->avl_right==NULL)
            {
                return NULL;
            }else if (currentNode->avl_left==NULL&&currentNode->avl_right!=NULL)
            {
                return currentNode->avl_right;
            }else if (currentNode->avl_left!=NULL&&currentNode->avl_right==NULL)
            {
                return currentNode->avl_left;
            }else{
                KVSNode* newCurrentNode=NULL;
                currentNode->avl_right=avl_delete_removeMin(currentNode->avl_right,&newCurrentNode);
                newCurrentNode->avl_left=currentNode->avl_left;
                newCurrentNode->avl_right=currentNode->avl_right;
                update_height(newCurrentNode);
                int bf=balance_factor(newCurrentNode);
                if (bf>1)
                {
                    int left_bf=balance_factor(newCurrentNode->avl_left);
                    if (left_bf>=0)
                    {
                        return rotate_right(newCurrentNode);
                        //LL
                    }else{
                        newCurrentNode->avl_left=rotate_left(newCurrentNode->avl_left);
                        return rotate_right(newCurrentNode);
                        //LR
                    }  
                }else if (bf<-1)
                {
                    int right_bf=balance_factor(newCurrentNode->avl_right);
                    if (right_bf<=0)
                    {
                        return rotate_left(newCurrentNode);
                        //RR
                    }else{
                        newCurrentNode->avl_right=rotate_right(newCurrentNode->avl_right);
                        return rotate_left(newCurrentNode);
                        //RL
                    }  
                }
                return newCurrentNode;
            }
        }   
    }
    return NULL;
}
void avl_delete(KVS* kvs, KVSNode* node){
    kvs->avlRoot=avl_delete_recursive(kvs->avlRoot,node);
}


void avl_print_keys(KVSNode* root) {
    if (root == NULL) {
        return;
    }
    avl_print_keys(root->avl_left);
    
    printf("%s\n", root->key);
    
    avl_print_keys(root->avl_right);
}