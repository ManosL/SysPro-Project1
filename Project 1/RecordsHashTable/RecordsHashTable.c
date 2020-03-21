#include <stdio.h>
#include <stdlib.h>

#include "RecordsHashTable.h"
#include "../Misc/MathOps.h"

struct RecordsHashTable{
    BucketListPtr* entries;

    int index_to_split;
    int curr_depth;

    int curr_entries;
    int curr_records;

    int entries_init_num;
    int bucket_capacity;

    double load_factor;

    long (*hash_function)(char*);
};

void RecordsHashTable_Create(RecordsHashTablePtr* recordsHT,int entriesNum, 
                    int bucketCapacity, double loadFactor, long (*hashFn)(char*))
{
    *recordsHT = malloc(sizeof(struct RecordsHashTable));

    (*recordsHT)->entries = malloc(entriesNum * sizeof(BucketListPtr));

    for(int i = 0;i < entriesNum;i++){
        RecordsBucketList_Create(&((*recordsHT)->entries[i]), bucketCapacity);
    }

    (*recordsHT)->index_to_split = 0;
    (*recordsHT)->curr_depth = 0;
    
    (*recordsHT)->curr_entries = entriesNum;
    (*recordsHT)->curr_records = 0;

    (*recordsHT)->entries_init_num = entriesNum;
    (*recordsHT)->bucket_capacity = bucketCapacity;
    (*recordsHT)->load_factor = loadFactor;
    (*recordsHT)->hash_function = hashFn;

    return;
}

Record* RecordsHashTable_Search(RecordsHashTablePtr recordsHT, char* id){
    int index = recordsHT->hash_function(id) % (recordsHT->entries_init_num * power(2, recordsHT->curr_depth));

    if(index < recordsHT->index_to_split){
        index = recordsHT->hash_function(id) % (recordsHT->entries_init_num * (power(2, recordsHT->curr_depth + 1)));
    }

    return RecordsBucketList_Search(recordsHT->entries[index], id);
}

int RecordsHashTable_Insert(RecordsHashTablePtr recordsHT, char* id, Record* record){
    int index = recordsHT->hash_function(id) % (recordsHT->entries_init_num * power(2, recordsHT->curr_depth));

    // if the index was splitted I get the right index
    if(index < recordsHT->index_to_split){
        index = recordsHT->hash_function(id) % (recordsHT->entries_init_num * (power(2, recordsHT->curr_depth + 1)));
    }

    if(RecordsBucketList_Search(recordsHT->entries[index], id) != NULL){
        printf("Record with ID %s already exists\n",id);
        return 1;
    }

    // inserting the record in the bucket
    RecordsBucketList_InsertRecord(recordsHT->entries[index], id, record);
    (recordsHT->curr_records)++;

    double curr_load = ((double) recordsHT->curr_records) / ((double) recordsHT->curr_entries * recordsHT->bucket_capacity);

    // if curr_load > threshold I need to split an entry
    if(curr_load > recordsHT->load_factor){
        int to_split  = recordsHT->index_to_split;
        int to_create = recordsHT->curr_entries;
        int split_depth = recordsHT->curr_depth + 1;

        BucketListPtr* splitted_entry = RecordsBucketList_Split(recordsHT->entries[recordsHT->index_to_split],
                                    to_split, to_create, recordsHT->entries_init_num, split_depth, recordsHT->hash_function);

        recordsHT->entries = realloc(recordsHT->entries,(recordsHT->curr_entries + 1) * sizeof(BucketListPtr));
        
        RecordsBucketList_ShallowDestroy(&(recordsHT->entries[recordsHT->index_to_split]));
        
        recordsHT->entries[recordsHT->index_to_split] = splitted_entry[0];
        recordsHT->entries[recordsHT->curr_entries] = splitted_entry[1];

        free(splitted_entry);
        
        // If I reached the max of current depth the next entry to split
        // is the first one, else its the next
        if(recordsHT->index_to_split == ((recordsHT->entries_init_num * power(2, recordsHT->curr_depth)) - 1)){
            recordsHT->index_to_split = 0;
            (recordsHT->curr_depth)++;
        } else{
            (recordsHT->index_to_split)++;
        }

        (recordsHT->curr_entries)++;
    }

    return 0;
}

// Just finds at which entry is the record that I want to update
// exit date and calls the function of the bucket list to do this
// job.

void RecordsHashTable_UpdateExit(RecordsHashTablePtr recordsHT, char* id, Date new_date){
    int index = recordsHT->hash_function(id) % (recordsHT->entries_init_num * power(2, recordsHT->curr_depth));

    if(index < recordsHT->index_to_split){
        index = recordsHT->hash_function(id) % (recordsHT->entries_init_num * (power(2, recordsHT->curr_depth + 1)));
    }

    RecordsBucketList_UpdateExit(recordsHT->entries[index], id, new_date);

    return;
}

void RecordsHashTable_Destroy(RecordsHashTablePtr* recordsHT){
    for(int i = 0;i < (*recordsHT)->curr_entries; i++){
        RecordsBucketList_Destroy(&((*recordsHT)->entries[i]));
    }

    free((*recordsHT)->entries);

    free(*recordsHT);
    *recordsHT = NULL;
}