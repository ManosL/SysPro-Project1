#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Misc/MathOps.h"
#include "RecordsBucketList.h"

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
    Record* record;
};

void RecordsBucketList_Create(BucketListPtr* bucketList, int bucketCapacity){
    *bucketList = malloc(sizeof(struct BucketList));

    (*bucketList)->head = NULL;
    (*bucketList)->bucket_capacity = bucketCapacity;

    (*bucketList)->size = 0;

    return;
}

// a simple search function
Record* RecordsBucketList_Search(BucketListPtr bucketList, char* id){
    Bucket* curr_bucket = bucketList->head;

    while(curr_bucket != NULL){
        for(int i = 0; i < curr_bucket->curr_size; i++){
            if(strcmp(curr_bucket->bucket_records[i].ID, id) == 0){
                return curr_bucket->bucket_records[i].record;
            }
        }

        curr_bucket = curr_bucket->next;
    }

    return NULL;
}

// insertion to the hash table entry
void RecordsBucketList_InsertRecord(BucketListPtr bucketList,char* id, Record* record){
    Bucket* curr_bucket = bucketList->head;
    Bucket* prev_bucket = NULL;

    int inserted = 0;

    while(curr_bucket != NULL){
        for(int i = 0; i < curr_bucket->curr_size && inserted == 0; i++){
            // if ID already exists its an error because This hash table represents a set
            if(strcmp(curr_bucket->bucket_records[i].ID, id) == 0){
                printf("Record already exists\n");
                return;
            }
        }
        
        // If I find a bucket with a free space, I insert the record in this bucket
        if(curr_bucket->curr_size < curr_bucket->bucket_capacity){
            curr_bucket->bucket_records[curr_bucket->curr_size].ID = malloc((strlen(id) + 1)*sizeof(char));
            strcpy(curr_bucket->bucket_records[curr_bucket->curr_size].ID, id);

            curr_bucket->bucket_records[curr_bucket->curr_size].record = record;

            (curr_bucket->curr_size)++;
            inserted = 1;
        }

        prev_bucket = curr_bucket;
        curr_bucket = curr_bucket->next;
    }

    // If I traversed all the bucket list and didn't found free space on a bucket
    // I create a new bucket and insert that record here
    if(inserted == 0){
        if(prev_bucket == NULL){        // just checking if the list was empty or not
            bucketList->head = malloc(sizeof(struct Bucket));

            bucketList->head->bucket_capacity = bucketList->bucket_capacity;
            bucketList->head->bucket_records = malloc(bucketList->bucket_capacity * sizeof(struct BucketRecord));
            bucketList->head->curr_size = 1;
            
            bucketList->head->next = NULL;

            bucketList->head->bucket_records[0].ID = malloc((strlen(id) + 1)*sizeof(char));
            strcpy(bucketList->head->bucket_records[0].ID, id);

            bucketList->head->bucket_records[0].record = record;
        }
        else{
            prev_bucket->next = malloc(sizeof(struct Bucket));

            prev_bucket->next->bucket_capacity = bucketList->bucket_capacity;
            prev_bucket->next->bucket_records = malloc(bucketList->bucket_capacity * sizeof(struct BucketRecord));
            prev_bucket->next->curr_size = 1;
            
            prev_bucket->next->next = NULL;

            prev_bucket->next->bucket_records[0].ID = malloc((strlen(id) + 1)*sizeof(char));
            strcpy(prev_bucket->next->bucket_records[0].ID, id);

            prev_bucket->next->bucket_records[0].record = record;
        }

        (bucketList->size)++;
    }
}

// Because the records hash table applies Linear hashing
// I need a function that splits the current bucket list
BucketListPtr* RecordsBucketList_Split(BucketListPtr bucketList,int to_split_1,
                    int to_split_2, int entries_init_num, int curr_depth, 
                    long (*hashFn)(char*))
{
    BucketListPtr* buckets = malloc(2 * sizeof(BucketListPtr));  // Creating the 2 new buckets

    RecordsBucketList_Create(&buckets[0], bucketList->bucket_capacity);
    RecordsBucketList_Create(&buckets[1], bucketList->bucket_capacity);

    Bucket* curr_node = bucketList->head;

    while(curr_node != NULL){
        
        for(int i = 0;i < curr_node->curr_size; i++){
            int index = hashFn(curr_node->bucket_records[i].ID) % (entries_init_num * (power(2, curr_depth))); // determining in which
                                                                                                               // bucket the record will be

            if(index == to_split_1){
                RecordsBucketList_InsertRecord(buckets[0], curr_node->bucket_records[i].ID, curr_node->bucket_records[i].record);
            }
            else if(index == to_split_2){
                RecordsBucketList_InsertRecord(buckets[1], curr_node->bucket_records[i].ID, curr_node->bucket_records[i].record);
            } else{
                printf("HUGE FAULT AT SPLIT\n");
            }
        }
        curr_node = curr_node->next;
    }

    return buckets;  // returning the 2 new buckets
}

// Changing record's exit date to new_date 
void RecordsBucketList_UpdateExit(BucketListPtr bucketList, char* id, Date new_date){
    Bucket* curr_bucket = bucketList->head;

    while(curr_bucket != NULL){
        for(int i = 0; i < curr_bucket->curr_size; i++){
            if(!strcmp(curr_bucket->bucket_records[i].record->recordID, id)){
                Date prev_exit_date;
                Date_Copy(curr_bucket->bucket_records[i].record->exit_date, &prev_exit_date);
                Date_Copy(new_date, &(curr_bucket->bucket_records[i].record->exit_date));

                // printing the confirmation message
                /*printf("Changed succesfully exit date of record %s from ",id);
                if(prev_exit_date.day == -1){
                    printf("- to ");
                } else{
                    printf("%d-%d-%d to ",prev_exit_date.day, 
                                        prev_exit_date.month,
                                        prev_exit_date.year);
                }

                if(new_date.day == -1){
                    printf("-.\n");
                } else{
                    printf("%d-%d-%d.\n",curr_bucket->bucket_records[i].record->exit_date.day, 
                                        curr_bucket->bucket_records[i].record->exit_date.month,
                                        curr_bucket->bucket_records[i].record->exit_date.year);
                }*/

                printf("Record updated\n");

                return;
            }
        }

        curr_bucket = curr_bucket->next;
    }
}

// I need that function when splitting to destroy the old list
// because I don't want to free the memory that was allocated for
// each record
void RecordsBucketList_ShallowDestroy(BucketListPtr* bucketList){
    Bucket* curr_node = (*bucketList)->head;

    while(curr_node != NULL){
        Bucket* temp = curr_node;
        
        for(int i = 0; i < curr_node->curr_size;i++){
            free(curr_node->bucket_records[i].ID);
        }

        free(curr_node->bucket_records);
        
        temp = curr_node;
        curr_node = curr_node->next;

        free(temp);
    }

    free(*bucketList);
    
    *bucketList = NULL;
}

void RecordsBucketList_Destroy(BucketListPtr* bucketList){
    Bucket* curr_node = (*bucketList)->head;

    while(curr_node != NULL){
        Bucket* temp = curr_node;

        for(int i = 0;i < curr_node->curr_size;i++){
            free(curr_node->bucket_records[i].ID);
            Record_Destroy(&(curr_node->bucket_records[i].record)); // I delete that because this 
                                                                    // will be the data structure 
                                                                    // that will hold the records
        }

        free(curr_node->bucket_records);
        
        temp = curr_node;
        curr_node = curr_node->next;

        free(temp);
    }

    free(*bucketList);
    
    *bucketList = NULL;
}