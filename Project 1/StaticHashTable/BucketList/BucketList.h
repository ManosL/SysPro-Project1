#ifndef __BUCKET_LIST_H__
#define __BUCKET_LIST_H__

#include "../../Record/Record.h"
#include "../../MaxHeap/MaxHeap.h"

typedef struct BucketList* BucketListPtr;

void BucketList_Create(BucketListPtr*,int);
void BucketList_InsertRecord(BucketListPtr,char*,Record*);
int BucketList_Exists(BucketListPtr,char*);
void BucketList_diseaseFrequency(BucketListPtr,char*,char*,Date,Date);
void BucketList_currentPatients(BucketListPtr,char*,Date);
void BucketList_setDiseaseHeap(BucketListPtr,char*,Date,Date,MaxHeapPtr);
void BucketList_setCountryHeap(BucketListPtr,char*,Date,Date,MaxHeapPtr);
void BucketList_Destroy(BucketListPtr*);

#endif