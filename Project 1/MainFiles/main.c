#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./DiseaseMonitor/DiseaseMonitor.h"
#include "../Misc/HashFunctions.h"

#define MIN_BUCKET_SIZE 125

int main(int argc,char* argv[]){
    char* initial_file_path = NULL;
    int diseaseNumOfEntries = -1;
    int countryNumOfEntries = -1;

    int bucketSize = -1;
    
    /* Checking the command line arguments */

    if(argc < 9){
        printf("Not enough arguments provided\n");
        return 1;
    }

    if(argc > 9){
        printf("Too many arguments provided\n");
        return 1;
    }

    for(int i = 1;i < argc;i = i + 2){
        if(!strcmp(argv[i],"-p") && initial_file_path == NULL){
            initial_file_path = argv[i+1];
        } else if(!strcmp(argv[i],"-h1") && diseaseNumOfEntries == -1){
            diseaseNumOfEntries = atoi(argv[i+1]);

            if(diseaseNumOfEntries <= 0){
                printf("Num of entries should be greater that 0\n");
                return 1;
            }
        } else if(!strcmp(argv[i], "-h2") && countryNumOfEntries == -1){
            countryNumOfEntries = atoi(argv[i+1]);

            if(countryNumOfEntries <= 0){
                printf("Num of entries should be greater that 0\n");
                return 1;
            }
        } else if(!strcmp(argv[i],"-b") && bucketSize == -1){
            bucketSize = atoi(argv[i+1]);

            if(bucketSize < MIN_BUCKET_SIZE){
                printf("Bucket size should be at least %d bytes\n", MIN_BUCKET_SIZE);
                return 1;
            }
        } else{
            printf("You provided wrong parameter %s\n",argv[i]);
            return 1;
        }
    }

    /* Initializing the data structures that I will use */
    DiseaseMonitorPtr diseaseMonitor;

    int out = DiseaseMonitor_Create(&diseaseMonitor, diseaseNumOfEntries, countryNumOfEntries,
                            bucketSize, Universal_Hash_Function);

    if(out == -1){
        return 1;
    }

    /* Reading the records file and inserting the records */
    FILE* records_file = fopen(initial_file_path, "r");

    if(records_file == NULL){
        printf("Failed to open file %s\n", initial_file_path);
        return 1;
    }

    if(DiseaseMonitor_InitializeData(diseaseMonitor, records_file) == -1){
        fclose(records_file);
        DiseaseMonitor_Exit(&diseaseMonitor);
       return 1;
    }

    // Running the disease monitor
    DiseaseMonitor_Run(diseaseMonitor);
    DiseaseMonitor_Exit(&diseaseMonitor);

    fclose(records_file);
    return 0;
}