#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MaxHeap.h"
#include "../Misc/MathOps.h"

typedef struct MaxHeapNode MaxHeapNode;

struct MaxHeap{
    MaxHeapNode* root;

    int size;
};

struct MaxHeapNode{
    HeapRecord record;

    MaxHeapNode* left_child;
    MaxHeapNode* right_child;
    MaxHeapNode* parent;
};

void MaxHeap_Create(MaxHeapPtr* heapPtr){
    *heapPtr = malloc(sizeof(struct MaxHeap));

    (*heapPtr)->root = NULL;

    (*heapPtr)->size = 0;
}

bool MaxHeap_isEmpty(MaxHeapPtr heapPtr){
    return heapPtr->size == 0;
}

// Searching a name in the heap by traversing the tree 
MaxHeapNode* heap_search(MaxHeapNode* curr_node, char* name){       // helper function
    if(curr_node == NULL)
        return NULL;
    
    if(strcmp(curr_node->record.name, name) == 0){
        return curr_node;
    }

    MaxHeapNode* search_result1 = heap_search(curr_node->left_child, name);
    MaxHeapNode* search_result2 = heap_search(curr_node->right_child, name);

    if(search_result1 == NULL && search_result2 == NULL) return NULL;

    if(search_result1 != NULL && search_result2 == NULL) return search_result1;

    if(search_result1 == NULL && search_result2 != NULL) 
        return search_result2;
    else{
        printf("Cannot have duplicate values in heap\n");
        return NULL;
    }
}

MaxHeapNode* MaxHeap_Search(MaxHeapPtr heapPtr, char* name){      //returns address of element's node
    return heap_search(heapPtr->root, name);
}

// Counting nodes on a specific level
int tree_count_nodes_on_level(MaxHeapNode* curr_node,int curr_level){
    if(curr_node == NULL){
        return 0;
    }

    // if the node belongs to the level we wanted we return that
    // we found one node on this level
    if(curr_level == 0){
        return 1;
    }

    // if the node is not on that level we ask the left and right subtree
    // about howmany nodes they have on level -1
    int left_subtree_nodes = tree_count_nodes_on_level(curr_node->left_child, curr_level-1);
    int right_subtree_nodes = tree_count_nodes_on_level(curr_node->right_child, curr_level-1);
    
    return left_subtree_nodes + right_subtree_nodes;
}

int heap_tree_height(MaxHeapNode* curr_node){
    if(curr_node == NULL){
        return -1;
    }

    int left_tree_height = heap_tree_height(curr_node->left_child);
    int right_tree_height = heap_tree_height(curr_node->right_child);

    return 1 + (left_tree_height > right_tree_height ? left_tree_height : right_tree_height);
}

// finding the node that the new value will be his child
MaxHeapNode* MaxHeap_toAddNode(MaxHeapNode* curr_node, int curr_height){    // it's a helper
    if(curr_node == NULL){
        return NULL;
    }

    if(curr_node->left_child == NULL && curr_node->right_child == NULL){
        return curr_node;
    }

    if(curr_node->left_child != NULL && curr_node->right_child == NULL){
        return curr_node;
    }

    // if the left subtree has greater height than the right subtree then
    if(heap_tree_height(curr_node->left_child) > heap_tree_height(curr_node->right_child)){
        // if the left subtree is complete then I should add it in the right subtree
        //  because it does not have space on  the last level to put the new node
        if(tree_count_nodes_on_level(curr_node->left_child, curr_height - 1) == power(2, curr_height - 1)){
            return MaxHeap_toAddNode(curr_node->right_child, curr_height - 1);
        }
        else{               // else we go to the left subtree
            return MaxHeap_toAddNode(curr_node->left_child, curr_height - 1);
        }
    }

    // if both subtrees have the same height then I check if the tree
    // is complete in order to determine if the last level have space
    // for an extra node. If it doesn't I just put the new node on
    // another level 
    if(heap_tree_height(curr_node->left_child) == heap_tree_height(curr_node->right_child)){
        if(tree_count_nodes_on_level(curr_node, curr_height) == power(2, curr_height)){
            return MaxHeap_toAddNode(curr_node->left_child, curr_height - 1);
        }
        else{
            return MaxHeap_toAddNode(curr_node->right_child, curr_height - 1);
        }        
    } 
    else{
        printf("Should not have happened\n");
    }

    return NULL;   
}

// Finding the last node on the heap(the outmost right on the last level)
MaxHeapNode* MaxHeap_findLastNode(MaxHeapNode* curr_node, int curr_height){    // it's a helper
    if(curr_node == NULL){
        return NULL;
    }

    if(curr_node->left_child == NULL && curr_node->right_child == NULL){
        return curr_node;
    }

    if(heap_tree_height(curr_node->left_child) > heap_tree_height(curr_node->right_child)){
        return MaxHeap_findLastNode(curr_node->left_child, curr_height - 1);
    }

    if(heap_tree_height(curr_node->left_child) == heap_tree_height(curr_node->right_child)){
        return MaxHeap_findLastNode(curr_node->right_child, curr_height - 1);    
    } 
    else{
        printf("Should not have happened\n");
    }

    return NULL;   
}

// In this program insert is used as a helper function.
// The main interface functions are MaxHeap_addOne and
// MaxHeap_printTopK
void MaxHeap_Insert(MaxHeapPtr heapPtr, char* name, int val){
    // If the heap is empty I just put a new node
    if(heapPtr->root == NULL){
        heapPtr->size = 1;

        heapPtr->root = malloc(sizeof(MaxHeapNode));

        heapPtr->root->record.name = malloc((strlen(name) + 1) * sizeof(char));
        strcpy(heapPtr->root->record.name, name);

        heapPtr->root->record.value = val;

        heapPtr->root->left_child = heapPtr->root->right_child = NULL;
        heapPtr->root->parent = NULL;

        return;
    }

    heapPtr->size += 1;

    // I find the parent of the new node
    MaxHeapNode* curr_node = MaxHeap_toAddNode(heapPtr->root, heap_tree_height(heapPtr->root));
    MaxHeapNode* new_child = NULL;

    // I determine which child of that node will be
    if(curr_node->left_child == NULL){
        curr_node->left_child = malloc(sizeof(MaxHeapNode));
        new_child = curr_node->left_child;
    }
    else{
        curr_node->right_child = malloc(sizeof(MaxHeapNode));
        new_child = curr_node->right_child;
    }

    new_child->record.name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(new_child->record.name, name);

    new_child->record.value = val;
    new_child->left_child = new_child->right_child = NULL;
    new_child->parent = curr_node;

    curr_node = new_child;

    // I determining by going all the way up to the root(if needed)
    // the right position of the new node
    while(curr_node->parent != NULL){
        MaxHeapNode* next_node = curr_node->parent;

        if(next_node->record.value < curr_node->record.value){
            HeapRecord temp = next_node->record;
            next_node->record = curr_node->record;
            next_node->record = temp;
        }
        else{
            break;
        }

        curr_node = next_node;
    }
}

// It's a helper function
// This function just adds 1 the value of the wanted node
// and finds the right position of that element
void addOne(MaxHeapPtr heapPtr, MaxHeapNode* heapElem){
    (heapElem->record.value)++;

    MaxHeapNode* curr_node = heapElem;

    while(curr_node->parent != NULL){
        MaxHeapNode* next_node = curr_node->parent;

        if(next_node->record.value < curr_node->record.value){
            HeapRecord temp = next_node->record;
            next_node->record = curr_node->record;
            curr_node->record = temp;
        }
        else{
            break;
        }

        curr_node = next_node;
    }
}

void MaxHeap_addOne(MaxHeapPtr heapPtr, char* name){
    MaxHeapNode* search_result = MaxHeap_Search(heapPtr,name);

    // If the node that I want to addOne does not exists
    // I insert it to the heap, else I just add one and heapify it
    if(search_result == NULL){
        MaxHeap_Insert(heapPtr,name,1);
    }
    else{
        addOne(heapPtr,search_result);
    }
}

HeapRecord MaxHeap_popMax(MaxHeapPtr heapPtr){
    HeapRecord max = heapPtr->root->record;

    // Because I delete the root I need to replace the root value
    // with the value of the last node
    MaxHeapNode* last_node = MaxHeap_findLastNode(heapPtr->root, heap_tree_height(heapPtr->root));

    if(heapPtr->root == last_node){
        free(heapPtr->root);
        heapPtr->root = NULL;
        
        (heapPtr->size)--;
        return max;
    }

    heapPtr->root->record = last_node->record;

    /* Delete last node */
    if(last_node == last_node->parent->left_child){
        last_node->parent->left_child = NULL;
    }
    else{
        last_node->parent->right_child = NULL;
    }

    free(last_node);
    
    (heapPtr->size)--;

    /* ---------------- */

    MaxHeapNode* curr_node = heapPtr->root;

    // Heapifying and inserting the last node to its right position
    while (1)
    {
        MaxHeapNode* next_node_1 = curr_node->left_child;
        MaxHeapNode* next_node_2 = curr_node->right_child;

        MaxHeapNode* next_node = NULL;

        if(next_node_1 == NULL && next_node_2 == NULL){
            break;
        } else if(next_node_1 != NULL && next_node_2 == NULL){ 
            next_node = next_node_1;
        } else if(next_node_1 == NULL && next_node_2 != NULL){
            next_node = next_node_2;
        }
        else{
            next_node = next_node_1->record.value > next_node_2->record.value
                         ? next_node_1 : next_node_2 ;
        }

        // if the value of next node is greater than the value
        // of last node I swap them
        if(next_node->record.value > curr_node->record.value){
            HeapRecord temp = next_node->record;
            next_node->record = curr_node->record;
            curr_node->record = temp;
        }
        else{
            break;
        }

        curr_node = next_node;        
    }

    return max;  
}

// Just removing the root of the heap and printing the result
void MaxHeap_printTopK(MaxHeapPtr heapPtr, int k){
    for(int i = 1; i <= k; i++){
        if(MaxHeap_isEmpty(heapPtr)) break;

        HeapRecord curr = MaxHeap_popMax(heapPtr);

        printf("%d->%s->%d\n", i, curr.name, curr.value);

        free(curr.name);
    }

    return;
}

// helper for MaxHeap_Destroy
void heap_destroy(MaxHeapNode* curr_node){
    if(curr_node == NULL) return;

    heap_destroy(curr_node->left_child);
    heap_destroy(curr_node->right_child);

    free(curr_node->record.name);
    free(curr_node);

    return;
}

void MaxHeap_Destroy(MaxHeapPtr* heapPtr){
    heap_destroy((*heapPtr)->root);

    free(*heapPtr);

    *heapPtr = NULL;
}