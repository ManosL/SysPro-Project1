
#include <stdio.h>
#include <stdlib.h>

#include "BucketList/BucketList.h"

#include "StaticHashTable.h"

struct StaticHashTable{
    BucketListPtr* entries;

    int entriesNum;
    int bucketSize;         // how many bytes will be each bucket

    long (*hashFunction)(char*);
};

void StaticHashTable_Create(StaticHashTablePtr* staticHT,int entriesNum,int bucketSize,long (*hashFn)(char*)){
    *staticHT = malloc(sizeof(struct StaticHashTable));

    (*staticHT)->entriesNum = entriesNum;
    (*staticHT)->bucketSize = bucketSize;

    (*staticHT)->hashFunction = hashFn;

    (*staticHT)->entries = malloc(entriesNum * sizeof(BucketListPtr));

    for(int i = 0; i < entriesNum; i++){
        BucketList_Create(&((*staticHT)->entries[i]), bucketSize);
    }

    return;
}

// The following functions just determine the index of the given ID on the hash
// table and call the proper function of the BucketList structure in order to do
// all the job
void StaticHashTable_Insert(StaticHashTablePtr staticHT,char* id, Record* record){
    int index = staticHT->hashFunction(id) % staticHT->entriesNum;

    BucketList_InsertRecord(staticHT->entries[index],id,record);
}

int StaticHashTable_Exists(StaticHashTablePtr staticHT, char* id){
    int index = staticHT->hashFunction(id) % staticHT->entriesNum;

    return BucketList_Exists(staticHT->entries[index], id);
}

// I will apply this function to disease hash table
void StaticHashTable_diseaseFrequency(StaticHashTablePtr staticHT,char* disease, char* country, Date start, Date end){
    // Starting printing the results of /diseaseFrequency or /globalDiseaseStats command
    if(disease == NULL){
        printf("Global outbreaks for all countries ");
    } else{
        printf("%s's outbreaks for ", disease);

        if(country == NULL){
            printf("all countries ");
        } else{
            printf("%s ", country);
        }
    }

    if(start.day == -1){
        printf("(all time)\n");
    }else{
        printf("(from %d-%d-%d to %d-%d-%d)\n", start.day, start.month, start.year,
                                            end.day, end.month, end.year);
    }

    if(disease == NULL){
        for(int i = 0; i < staticHT->entriesNum; i++){
            BucketList_diseaseFrequency(staticHT->entries[i], NULL, NULL, start, end);
        }
    } else{
        int index = staticHT->hashFunction(disease) % staticHT->entriesNum;

        BucketList_diseaseFrequency(staticHT->entries[index], disease, country, start, end);
    }
}

void StaticHashTable_printTopKDiseases(StaticHashTablePtr staticHT, int K, char* country, Date start, Date end){
    int index = staticHT->hashFunction(country) % staticHT->entriesNum;

    MaxHeapPtr topKHeap = NULL;

    MaxHeap_Create(&topKHeap);
    
    BucketList_setDiseaseHeap(staticHT->entries[index], country, start, end, topKHeap);  // getting the configured heap

    printf("Top (at most) %d diseases",K);

    if(start.day == -1){
        printf("(all time)\n\n");
    }else{
        printf("(from %d-%d-%d to %d-%d-%d)\n\n", start.day, start.month, start.year,
                                            end.day, end.month, end.year);
    }

    MaxHeap_printTopK(topKHeap,K);      // printing top k diseases

    MaxHeap_Destroy(&topKHeap);

    return;
}

void StaticHashTable_printTopKCountries(StaticHashTablePtr staticHT, int K, char* disease, Date start, Date end){
    int index = staticHT->hashFunction(disease) % staticHT->entriesNum;

    MaxHeapPtr topKHeap = NULL;

    MaxHeap_Create(&topKHeap);
    
    BucketList_setCountryHeap(staticHT->entries[index], disease, start, end, topKHeap); // getting the configured heap

    printf("Top (at most) %d diseases",K);

    if(start.day == -1){
        printf("(all time)\n\n");
    }else{
        printf("(from %d-%d-%d to %d-%d-%d)\n\n", start.day, start.month, start.year,
                                            end.day, end.month, end.year);
    }

    MaxHeap_printTopK(topKHeap,K);  // printing top k countries

    MaxHeap_Destroy(&topKHeap);

    return;
}

void StaticHashTable_Destroy(StaticHashTablePtr* staticHT){
    for(int i = 0;i < (*staticHT)->entriesNum; i++){
        BucketList_Destroy(&((*staticHT)->entries[i]));
    }

    free((*staticHT)->entries);

    free(*staticHT);
    *staticHT = NULL;
}