#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BucketList.h"
#include "../../AVLTree/AVLTree.h"

typedef struct Bucket Bucket;
typedef struct BucketRecord BucketRecord;

struct BucketList{
    Bucket* head;
    //Bucket* tail;

    int bucket_capacity; // This does not change, 
                         //shows the number of records that a bucket can hold
    int size;
};

struct Bucket{
    BucketRecord* bucket_records;

    int bucket_capacity;
    int curr_size;

    Bucket* next;
};

struct BucketRecord{
    char* ID;
    AVLTreePtr patient_records;
};

void BucketList_Create(BucketListPtr* bucketList, int bucketSize){
    *bucketList = malloc(sizeof(struct BucketList));

    (*bucketList)->head = NULL;
    
    (*bucketList)->bucket_capacity = (bucketSize - sizeof(Bucket*) - (2 * sizeof(int)) - sizeof(BucketRecord*)) / sizeof(struct BucketRecord);

    (*bucketList)->size = 0;

    return;
}


// inserting a record into a hash table entry
void BucketList_InsertRecord(BucketListPtr bucketList,char* id, Record* record){
    Bucket* curr_bucket = bucketList->head;
    Bucket* prev_bucket = NULL;

    int inserted = 0;

    while(curr_bucket != NULL){
        for(int i = 0; i < curr_bucket->curr_size && inserted == 0; i++){
            if(strcmp(curr_bucket->bucket_records[i].ID, id) == 0){         // if the id already has itwown record in the hash table
                                                                            // I insert that record in the tree
                AVLTree_Insert(curr_bucket->bucket_records[i].patient_records, record);
                inserted = 1;
            }
        }

        if(inserted == 1) break;

        // if i did not insert the record and the last bucket(because only the last bucket
        // can have free space) has free space I create the record here and insert the
        // patient record provided, in the tree
        if(inserted == 0 && curr_bucket->curr_size < curr_bucket->bucket_capacity){
            curr_bucket->bucket_records[curr_bucket->curr_size].ID = malloc((strlen(id) + 1)*sizeof(char));
            strcpy(curr_bucket->bucket_records[curr_bucket->curr_size].ID, id);

            AVLTree_Create(&(curr_bucket->bucket_records[curr_bucket->curr_size].patient_records));
            AVLTree_Insert(curr_bucket->bucket_records[curr_bucket->curr_size].patient_records, record);

            (curr_bucket->curr_size)++;
            inserted = 1;
        }

        prev_bucket = curr_bucket;
        curr_bucket = curr_bucket->next;
    }

    // If the patient's record did not got into a tree I just create a new bucket where 
    // a new record will be created and in that record's tree the patient's 
    // record will be added
    if(inserted == 0){
        if(prev_bucket == NULL){
            bucketList->head = malloc(sizeof(struct Bucket));

            bucketList->head->bucket_capacity = bucketList->bucket_capacity;
            bucketList->head->bucket_records = malloc(bucketList->bucket_capacity * sizeof(struct BucketRecord));

            bucketList->head->curr_size = 1;
            
            bucketList->head->next = NULL;

            bucketList->head->bucket_records[0].ID = malloc((strlen(id) + 1)*sizeof(char));
            strcpy(bucketList->head->bucket_records[0].ID, id);

            AVLTree_Create(&(bucketList->head->bucket_records[0].patient_records));
            AVLTree_Insert(bucketList->head->bucket_records[0].patient_records, record);
        }
        else{
            prev_bucket->next = malloc(sizeof(struct Bucket));

            prev_bucket->next->bucket_capacity = bucketList->bucket_capacity;
            prev_bucket->next->bucket_records = malloc(bucketList->bucket_capacity * sizeof(struct BucketRecord));
            prev_bucket->next->curr_size = 1;
            
            prev_bucket->next->next = NULL;

            prev_bucket->next->bucket_records[0].ID = malloc((strlen(id) + 1)*sizeof(char));
            strcpy(prev_bucket->next->bucket_records[0].ID, id);

            AVLTree_Create(&(prev_bucket->next->bucket_records[0].patient_records));
            AVLTree_Insert(prev_bucket->next->bucket_records[0].patient_records, record);
        }

        (bucketList->size)++;
    }
}

// just searches for an ID in the list
int BucketList_Exists(BucketListPtr bucketList, char* id){
    Bucket* curr_bucket = bucketList->head;

    while(curr_bucket != NULL){
        for(int i = 0; i < curr_bucket->curr_size; i++){
            if(!strcmp(curr_bucket->bucket_records[i].ID, id))
                return 1;
        }
        
        curr_bucket = curr_bucket->next;
    }

    return 0;
}

void BucketList_diseaseFrequency(BucketListPtr bucketList,char* disease, char* country, Date start, Date end){
    Bucket* curr_bucket = bucketList->head;
    int curr_result;

    // calls the AVL Tree function that will give us the disease frequency in a specific
    //country and date(if these are provided, if not country == NULL and the day of date
    // structures will be -1) of a disease or for all diseases(if disease == NULL).
    while(curr_bucket != NULL){
        for(int i = 0; i < curr_bucket->curr_size; i++){
            if(disease == NULL){
                curr_result = AVLTree_diseaseFrequency(curr_bucket->bucket_records[i].patient_records, 
                                                    NULL, start, end);
                
                printf("%s: %d outbreaks\n",curr_bucket->bucket_records[i].ID,curr_result);
            }
            else if(!strcmp(curr_bucket->bucket_records[i].ID, disease)){
                curr_result = AVLTree_diseaseFrequency(curr_bucket->bucket_records[i].patient_records, 
                                                    country, start, end);
                
                printf("%s: %d outbreaks\n",disease,curr_result);
            }
        }

        curr_bucket = curr_bucket->next;
    }
}

// This function and the next one have the purpose to set their heaps to service the topK queries.
// This will be done by searching the ID on the list and let the AVLTree_set{name}Heap do the job.
void BucketList_setDiseaseHeap(BucketListPtr bucketList, char* country, Date start, Date end, MaxHeapPtr heap){
    Bucket* curr_node = bucketList->head;

    while(curr_node != NULL){
        for(int i = 0; i < curr_node->curr_size; i++){
            if(!strcmp(curr_node->bucket_records[i].ID, country)){
                AVLTree_setDiseaseHeap(curr_node->bucket_records[i].patient_records, start, end, heap);
                return;
            }
        }

        curr_node = curr_node->next;
    }
}

void BucketList_setCountryHeap(BucketListPtr bucketList, char* disease, Date start, Date end, MaxHeapPtr heap){
    Bucket* curr_node = bucketList->head;

    while(curr_node != NULL){
        for(int i = 0; i < curr_node->curr_size; i++){
            if(!strcmp(curr_node->bucket_records[i].ID, disease)){
                AVLTree_setCountryHeap(curr_node->bucket_records[i].patient_records, start, end, heap);
                return;
            }
        }

        curr_node = curr_node->next;
    }
}

void BucketList_Destroy(BucketListPtr* bucketList){
    Bucket* curr_node = (*bucketList)->head;

    while(curr_node != NULL){
        Bucket* temp = curr_node;

        for(int i = 0;i < curr_node->curr_size;i++){
            free(curr_node->bucket_records[i].ID);
            AVLTree_Destroy(&(curr_node->bucket_records[i].patient_records));
        }

        free(curr_node->bucket_records);
        
        temp = curr_node;
        curr_node = curr_node->next;

        free(temp);
    }

    free(*bucketList);
    
    *bucketList = NULL;
}