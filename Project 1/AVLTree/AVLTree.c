#include <stdio.h>
#include <stdlib.h>

#include "../Date/Date.h"
#include "RecPtrLinkedList/RecPtrLinkedList.h"

#include "AVLTree.h"

typedef struct AVLTreeNode AVLTreeNode;

struct AVLTree{
    AVLTreeNode* root;
};

struct AVLTreeNode{
    Date entry_date;
    RecPtrLinkedListPtr record_list;   // I keep in the tree the records
                                       // with the same entry date on a list

    AVLTreeNode* left_child;
    AVLTreeNode* right_child;
};

void AVLTree_Create(AVLTreePtr* avlPtr){

    (*avlPtr) = malloc(sizeof(struct AVLTree));

    if(*avlPtr == NULL){
        printf("Memory Error in AVLTree_Create.\n");
        return;
    }
    
    (*avlPtr)->root = NULL;

    return;
}

// Helper method in order to get the result recursively
// I did it in other recursive functions, too.
int tree_height(AVLTreeNode* curr_node){
    if(curr_node == NULL){
        return -1;
    }

    int left_tree_height = tree_height(curr_node->left_child);
    int right_tree_height = tree_height(curr_node->right_child);

    return 1 + (left_tree_height > right_tree_height ? left_tree_height : right_tree_height);
}

int AVLTree_Height(AVLTreePtr avlPtr){
    return tree_height(avlPtr->root);
}

// Tree rotations
// at the rotations I don't need to think any 
// more complicated cases because the rotations
// are getting done whenever should be and I dont have a
// ready BST that needs rotations

AVLTreeNode* AVLTree_LeftRotate(AVLTreeNode* curr_root){
    AVLTreeNode* new_root = curr_root->right_child;
    AVLTreeNode* temp = new_root->left_child;       // The "node" that should be moved

    new_root->left_child = curr_root;
    curr_root->right_child = temp;

    return new_root;
}

AVLTreeNode* AVLTree_RightRotate(AVLTreeNode* curr_root){
    AVLTreeNode* new_root = curr_root->left_child;
    AVLTreeNode* temp = new_root->right_child;      // The "node" that should be moved

    new_root->right_child = curr_root;
    curr_root->left_child = temp;

    return new_root;
}

// Classic AVL Tree insertion.

AVLTreeNode* tree_insert(AVLTreeNode* curr_node, Record* rec){
    // If I reached the bottom of the tree I just create the node 
    // and return its address
    if(curr_node == NULL){
        curr_node = malloc(sizeof(AVLTreeNode));

        Date_Copy(rec->entry_date,&(curr_node->entry_date));

        RecPtrLinkedList_Create(&(curr_node->record_list));
        RecPtrLinkedList_Insert(curr_node->record_list, rec);

        curr_node->right_child = curr_node->left_child = NULL;

        return curr_node;
    }
    else{                                                                   // else I go to left or right child and in
                                                                            // case of equality I add it into the records list
                                                                            // and return the current node because there won't 
                                                                            // be any need of rotation
        if(Date_isGreater(rec->entry_date, curr_node->entry_date)){
            curr_node->right_child = tree_insert(curr_node->right_child,rec);
        }
        else if(Date_isLess(rec->entry_date, curr_node->entry_date)){
            curr_node->left_child = tree_insert(curr_node->left_child,rec);
        } else {
            RecPtrLinkedList_Insert(curr_node->record_list,rec);
            
            return curr_node;
        }
    }

    // Now I determine if there is a need to rotate the tree
    // and because I apply recursion this will be checked all
    // the way up to the root and at each parent will get the
    // new root of the subtree

    int left_subtree_height = tree_height(curr_node->left_child);
    int right_subtree_height = tree_height(curr_node->right_child);
    int node_balance = left_subtree_height - right_subtree_height;

    // The inbalance came from left subtree, so I got the first "step"
    if(node_balance > 1){
        AVLTreeNode* left_subtree = curr_node->left_child;
        int left_subtree_balance  = tree_height(left_subtree->left_child) - tree_height(left_subtree->right_child);
        
        // determining the second step to new node, so determining the
        // second rotation(if it need to get done)
        if(left_subtree_balance == 1){              // LL case
            curr_node = AVLTree_RightRotate(curr_node);
        }else if(left_subtree_balance == -1){       // LR case
            curr_node->left_child = AVLTree_LeftRotate(curr_node->left_child);
            curr_node = AVLTree_RightRotate(curr_node);
        }else{
            printf("WRONG\n");
        }
    }

    // The inbalance came from right subtree, so I got the first "step"
    if(node_balance < -1){
        AVLTreeNode* right_subtree = curr_node->right_child;
        int right_subtree_balance  = tree_height(right_subtree->left_child) - tree_height(right_subtree->right_child);

        // determining the second step to new node, so determining the
        // second rotation(if it need to get done)
        if(right_subtree_balance == 1){              // RL case
            curr_node->right_child = AVLTree_RightRotate(curr_node->right_child);
            curr_node = AVLTree_LeftRotate(curr_node);
        }else if(right_subtree_balance == -1){       // RR case
            curr_node = AVLTree_LeftRotate(curr_node);
        }else{
            printf("WRONG\n");
        }
    }

    return curr_node;
}

void AVLTree_Insert(AVLTreePtr avlPtr,Record* rec){
    if(avlPtr->root == NULL){       // If the tree is empty I create the root node
        avlPtr->root = malloc(sizeof(AVLTreeNode));

        Date_Copy(rec->entry_date,&(avlPtr->root->entry_date));
        
        RecPtrLinkedList_Create(&(avlPtr->root->record_list));
        RecPtrLinkedList_Insert(avlPtr->root->record_list, rec);

        avlPtr->root->right_child = avlPtr->root->left_child = NULL;
    }
    else{
        avlPtr->root = tree_insert(avlPtr->root,rec);
    }
}

// helper method
// Im gonna make an inorder traversal because inorder 
// traverses the tree with sorted order
int tree_diseaseFrequency(AVLTreeNode* curr_node, char* country, Date start, Date end){
    int result_to_add;

    if(curr_node == NULL) return 0;

    result_to_add = RecPtrLinkedList_countryDateFrequency(curr_node->record_list, country, start, end);

    return result_to_add + tree_diseaseFrequency(curr_node->right_child, country, start, end) +
            tree_diseaseFrequency(curr_node->left_child, country, start, end);
}

int AVLTree_diseaseFrequency(AVLTreePtr avlPtr, char* country, Date start, Date end){
    return tree_diseaseFrequency(avlPtr->root, country, start, end);
}

// Just traverses the nodes and letting the set Heap function
// of the list do the job
void tree_setDiseaseHeap(AVLTreeNode* curr_node, Date start, Date end, MaxHeapPtr heap){
    if(curr_node == NULL) return;

    RecPtrLinkedList_setDiseaseHeap(curr_node->record_list, start, end, heap);
    tree_setDiseaseHeap(curr_node->left_child, start, end, heap);
    tree_setDiseaseHeap(curr_node->right_child, start, end, heap);
}

void AVLTree_setDiseaseHeap(AVLTreePtr avlPtr, Date start, Date end, MaxHeapPtr heap){
    tree_setDiseaseHeap(avlPtr->root,start,end,heap);
}

// Just traverses the nodes and letting the set Heap function
// of the list do the job
void tree_setCountryHeap(AVLTreeNode* curr_node, Date start, Date end, MaxHeapPtr heap){
    if(curr_node == NULL) return;

    RecPtrLinkedList_setCountryHeap(curr_node->record_list, start, end, heap);
    tree_setCountryHeap(curr_node->left_child, start, end, heap);
    tree_setCountryHeap(curr_node->right_child, start, end, heap);
}

void AVLTree_setCountryHeap(AVLTreePtr avlPtr, Date start, Date end, MaxHeapPtr heap){
    tree_setCountryHeap(avlPtr->root,start,end,heap);
}

// Destroying the tree with post-order traversal
void tree_destroy(AVLTreeNode* curr_node){
    if(curr_node == NULL)
        return;
    
    tree_destroy(curr_node->right_child);
    tree_destroy(curr_node->left_child);

    RecPtrLinkedList_Destroy(&(curr_node->record_list));
    free(curr_node);
}

void AVLTree_Destroy(AVLTreePtr* avlPtr){
    tree_destroy((*avlPtr)->root);

    free(*avlPtr);
    *avlPtr = NULL;
}