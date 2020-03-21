#ifndef __RECORDS_HT__
#define __RECORDS_HT__

#include "./RecordsBucketList/RecordsBucketList.h"

// It will be a linear hash table
typedef struct RecordsHashTable* RecordsHashTablePtr;

void RecordsHashTable_Create(RecordsHashTablePtr* recordsHT,int entriesNum, 
                    int bucketCapacity, double loadFactor, long (*hashFn)(char*));

Record* RecordsHashTable_Search(RecordsHashTablePtr, char*);
int RecordsHashTable_Insert(RecordsHashTablePtr,char*,Record*);
void RecordsHashTable_UpdateExit(RecordsHashTablePtr,char*,Date);
void RecordsHashTable_Destroy(RecordsHashTablePtr*);

#endif