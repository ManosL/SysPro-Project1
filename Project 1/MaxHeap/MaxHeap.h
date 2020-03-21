#ifndef __MAX_HEAP_H__
#define __MAX_HEAP_H__

#include <stdbool.h>

#include "HeapRecord.h"

typedef struct MaxHeap* MaxHeapPtr;

void MaxHeap_Create(MaxHeapPtr*);
void MaxHeap_addOne(MaxHeapPtr,char*);
void MaxHeap_printTopK(MaxHeapPtr,int);
void MaxHeap_Destroy(MaxHeapPtr*);

#endif