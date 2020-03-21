#ifndef __RECORDS_BUCKET_LIST_H__
#define __RECORDS_BUCKET_LIST_H__

#include "../../Record/Record.h"

typedef struct BucketList* BucketListPtr;

void RecordsBucketList_Create(BucketListPtr*,int);
Record* RecordsBucketList_Search(BucketListPtr,char*);
void RecordsBucketList_InsertRecord(BucketListPtr,char*,Record*);
BucketListPtr* RecordsBucketList_Split(BucketListPtr,int,int,int,int,long (*hashFn)(char*));
void RecordsBucketList_UpdateExit(BucketListPtr,char*,Date);
void RecordsBucketList_ShallowDestroy(BucketListPtr*);
void RecordsBucketList_Destroy(BucketListPtr*);

#endif