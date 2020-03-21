#ifndef __STATIC_HT_H__
#define __STATIC_HT_H__

#include "../Record/Record.h"

// its the structure used for diseaseHashTable
// and countryHashTable

typedef struct StaticHashTable* StaticHashTablePtr;

void StaticHashTable_Create(StaticHashTablePtr*,int,int,long (*hashFn)(char*));
void StaticHashTable_Insert(StaticHashTablePtr,char*, Record*);
int StaticHashTable_Exists(StaticHashTablePtr,char*);
void StaticHashTable_diseaseFrequency(StaticHashTablePtr,char*,char*,Date,Date);
void StaticHashTable_printTopKDiseases(StaticHashTablePtr,int,char*,Date,Date);
void StaticHashTable_printTopKCountries(StaticHashTablePtr,int,char*,Date,Date);
void StaticHashTable_Destroy(StaticHashTablePtr*);

#endif