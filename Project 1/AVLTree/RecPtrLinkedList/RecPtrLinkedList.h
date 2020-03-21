#ifndef __RECORD_PTR_LINKED_LIST__
#define __RECORD_PTR_LINKED_LIST__

#include "../../Record/Record.h"
#include "../../MaxHeap/MaxHeap.h"

typedef struct RecPtrLinkedList* RecPtrLinkedListPtr; 

void RecPtrLinkedList_Create(RecPtrLinkedListPtr*);
int RecPtrLinkedList_Length(RecPtrLinkedListPtr);
void RecPtrLinkedList_Insert(RecPtrLinkedListPtr,Record*);
int RecPtrLinkedList_countryDateFrequency(RecPtrLinkedListPtr,char*,Date,Date);
void RecPtrLinkedList_setDiseaseHeap(RecPtrLinkedListPtr,Date,Date,MaxHeapPtr);
void RecPtrLinkedList_setCountryHeap(RecPtrLinkedListPtr,Date,Date,MaxHeapPtr);
void RecPtrLinkedList_Destroy(RecPtrLinkedListPtr*);

#endif