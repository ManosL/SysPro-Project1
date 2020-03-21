#ifndef __AVLTREE_H__
#define __AVLTREE_H__

#include "../Record/Record.h"

typedef struct AVLTree* AVLTreePtr;

void AVLTree_Create(AVLTreePtr*);
int AVLTree_Height(AVLTreePtr);
void AVLTree_Insert(AVLTreePtr,Record*); // I dont need to write delete function
int AVLTree_diseaseFrequency(AVLTreePtr,char*,Date,Date);
void AVLTree_setDiseaseHeap(AVLTreePtr,Date,Date,MaxHeapPtr);
void AVLTree_setCountryHeap(AVLTreePtr,Date,Date,MaxHeapPtr);
void AVLTree_Destroy(AVLTreePtr*);

#endif