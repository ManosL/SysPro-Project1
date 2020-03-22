#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "DiseaseMonitor.h"

#include "../../Date/Date.h"

#include "../../Misc/StringUtils.h"
#include "../../RecordsHashTable/RecordsHashTable.h"
#include "../../StaticHashTable/StaticHashTable.h"

struct DiseaseMonitor{
    StaticHashTablePtr diseaseHashTable;
    StaticHashTablePtr countryHashTable;

    RecordsHashTablePtr recordsSet;
};

/* Forward declaration for helper functions */
void DiseaseMonitor_globalDiseaseStats(DiseaseMonitorPtr,char*);
void DiseaseMonitor_diseaseFrequency(DiseaseMonitorPtr,char*);
void DiseaseMonitor_topKDiseases(DiseaseMonitorPtr,char*);
void DiseaseMonitor_topKCountries(DiseaseMonitorPtr,char*);
void DiseaseMonitor_InsertPatient(DiseaseMonitorPtr,char*);
void DiseaseMonitor_RecordPatientExit(DiseaseMonitorPtr,char*);
void DiseaseMonitor_numCurrentPatients(DiseaseMonitorPtr,char*);

// Creating the disease monitor by initializing all
// the necessary data structures.

int DiseaseMonitor_Create(DiseaseMonitorPtr* diseaseMonitor, int diseaseNumOfEntries,
                        int countryNumOfEntries, int bucketSize, long (*hashFn)(char*))
{
    *diseaseMonitor = malloc(sizeof(struct DiseaseMonitor));

    if(*diseaseMonitor == NULL){
        printf("Memory error at DiseaseMonitor_Create\n");
        return -1;
    }

    StaticHashTable_Create(&((*diseaseMonitor)->diseaseHashTable),diseaseNumOfEntries,bucketSize, hashFn);
    StaticHashTable_Create(&((*diseaseMonitor)->countryHashTable),countryNumOfEntries,bucketSize, hashFn);

    RecordsHashTable_Create(&((*diseaseMonitor)->recordsSet), 1, 5, 0.75, hashFn);  //The parameters there are hard coded

    return 0;
}

/* Inserts file's records */
/* I suppose that the stream is already open */
int  DiseaseMonitor_InitializeData(DiseaseMonitorPtr diseaseMonitor, FILE* records_file){
    int record_counter = 1;

    while(1){
        char* line = NULL;
        size_t line_len = 0;

        getline(&line, &line_len, records_file);    // reading the file line

        if(feof(records_file)){         // If I reach the end of file probably I read all the lines
            free(line);
            break;
        }
        
        line = String_removeWhitespaceSuffix(line);     // removing the whitespaces at the end

        // tokenizing the line (I suppose that each column is seperated with
        // exactly one space)
        char* recordID  = strtok(line, " ");
        char* firstName = strtok(NULL, " ");
        char* lastName  = strtok(NULL, " ");
        char* diseaseID = String_lowercase(strtok(NULL, " "));
        char* country   = String_lowercase(strtok(NULL, " "));
        char* entryDate_str = strtok(NULL, " ");
        char* exitDate_str  = strtok(NULL, " ");

        Date entryDate;
        Date exitDate;
        
        // Checking the numbers of the arguments provided at each line
        if(strtok(NULL," ") != NULL){
            printf("Line %d: Too many arguments(line will be ignored)\n", record_counter);
        }
        else if(exitDate_str == NULL){
            printf("Line %d: Not enough arguments(line will be ignored)\n",record_counter);
        } else{
            // If record with that ID already exists I stop the program
            if(RecordsHashTable_Search(diseaseMonitor->recordsSet, recordID) != NULL){
                printf("Record ID %s already exists. The program will stop...\n", recordID);
                free(line);
                return -1;
            }
            
            // Checking if the strings are ok
            if(!String_onlyLetters(firstName, -1)){
                printf("Line %d: First Name should contain only letters(line will be ignored)\n", record_counter);
            }
            else if(!String_onlyLetters(lastName, -1)){
                printf("Line %d: Last Name should contain only letters(line will be ignored)\n", record_counter);
            }
            else if((!String_onlyLettersAndNums(diseaseID,'-')) || (String_howManyOf(diseaseID,'-') > 1)){
                printf("Line %d: Disease ID should contain only letters, numbers and at least\none dash(line will be ignored)\n",record_counter);
            }
            else if(!String_onlyLetters(country, -1)){
                printf("Line %d: Country should contain only letters(line will be ignored)\n", record_counter);
            } 
            else if((entryDate = Date_fromStr(entryDate_str)).day == -1){
                printf("Line %d: Invalid Date or invalid date format(line will be ignored)\n",record_counter);
            } 
            else if((exitDate = Date_fromStr(exitDate_str)).day == -1 && (strcmp(exitDate_str,"-") != 0)){
                printf("Line %d: Invalid Date or invalid date format(line will be ignored)\n",record_counter);
            }
            else if(Date_isGreater(entryDate, exitDate) && (strcmp(exitDate_str,"-") != 0)){
                printf("Line %d: Entry date should be before exit date(line will be ignored)\n", record_counter);
            }
            else{   // If everything is ok
                Record* record = NULL;

                // Record creation
                Record_Initialize(&record, recordID, firstName, lastName, diseaseID, country, entryDate, exitDate);
                
                // I insert the necessary information in disease monitor's hash tables
                RecordsHashTable_Insert(diseaseMonitor->recordsSet, recordID, record);
                StaticHashTable_Insert(diseaseMonitor->diseaseHashTable, record->diseaseID, record);
                StaticHashTable_Insert(diseaseMonitor->countryHashTable, record->country, record);
            }


        }

        free(line);

        record_counter++;
    }
}

// The main function of disease monitor
void DiseaseMonitor_Run(DiseaseMonitorPtr diseaseMonitor){
    char* line = NULL;
    size_t line_len = 0;
    Date d1 = {-1,-1,-1};
    Date d2 = {-1, -1, -1};

    while(1){
        line = NULL;
        line_len = 0;

        // reading user's command
        printf(">");
        getline(&line,&line_len,stdin);

        line = String_removeWhitespaceSuffix(line);

        char* temp_line = malloc((strlen(line) + 1) * sizeof(char));
        strcpy(temp_line,line);
        
        char* command = strtok(line, " ");
        
        // Checking the first token that is the command and I call the
        // necessary helper function

        if(!strcmp(command, "/globalDiseaseStats")){
            DiseaseMonitor_globalDiseaseStats(diseaseMonitor, temp_line);
        }else if(!strcmp(command, "/diseaseFrequency")){
            DiseaseMonitor_diseaseFrequency(diseaseMonitor, temp_line);
        }else if(!strcmp(command,"/topk-Diseases")){
            DiseaseMonitor_topKDiseases(diseaseMonitor, temp_line);
        }else if(!strcmp(command,"/topk-Countries")){
            DiseaseMonitor_topKCountries(diseaseMonitor, temp_line);
        }else if(!strcmp(command,"/insertPatientRecord")){
            DiseaseMonitor_InsertPatient(diseaseMonitor, temp_line);
        }else if(!strcmp(command,"/recordPatientExit")){
            DiseaseMonitor_RecordPatientExit(diseaseMonitor, temp_line);
        }else if(!strcmp(command,"/numCurrentPatients")){
            DiseaseMonitor_numCurrentPatients(diseaseMonitor, temp_line);
        }else if(strcmp(line,"/exit") == 0){
            free(line);
            free(temp_line);
            return;
        }else{
            printf("Unknown command %s\n",command);
        }

        free(line);
        free(temp_line);
    }
}

// Function for /globalDiseaseStats command
void DiseaseMonitor_globalDiseaseStats(DiseaseMonitorPtr diseaseMonitor, char* cmd_line){
    char* cmd = strtok(cmd_line, " ");
    char* date1_str = strtok(NULL, " ");
    char* date2_str = strtok(NULL, " ");

    assert(!strcmp(cmd,"/globalDiseaseStats"));

    // Checking the num of arguments given
    if(strtok(NULL," ") != NULL){
        printf("Usage: /globalDiseaseStats [date1 date2]\n");
        return;
    }

    if(date1_str != NULL && date2_str == NULL){
        printf("globalDiseaseStats: When you are giving starting date you should give end date.\nToo many arguments.\n");
        return;
    }

    // If no dates are given
    if(date1_str == NULL && date2_str == NULL){
        StaticHashTable_diseaseFrequency(diseaseMonitor->diseaseHashTable,
                                        NULL,NULL, (Date) {-1,-1,-1}, (Date) {-1,-1,-1});
    } else{
        Date date1 = Date_fromStr(date1_str);
        Date date2 = Date_fromStr(date2_str);

        // Checking if the user gave valid dates and start day is indeed before end date
        // and if it is it calls the necessary function
        if(date1.day == -1){
            printf("globalDiseaseStats: Start date has invalid date or format(should be DD-MM-YYYY)\n");
            return;
        }

        if(date2.day == -1){
            printf("globalDiseaseStats: End date has invalid date or format(should be DD-MM-YYYY)\n");
            return;
        }

        if(Date_isGreater(date1, date2)){
            printf("globalDiseaseStats: Start date should be before end date\n");
            return;
        }

        StaticHashTable_diseaseFrequency(diseaseMonitor->diseaseHashTable,
                                    NULL, NULL, date1, date2);
    }

    return;
}

// Function for /diseaseFrequency command
void DiseaseMonitor_diseaseFrequency(DiseaseMonitorPtr diseaseMonitor, char* cmd_line){
    char* command   = strtok(cmd_line, " ");
    char* disease   = strtok(NULL, " ");
    char* date1_str = strtok(NULL, " ");
    char* date2_str = strtok(NULL, " ");
    char* country   = strtok(NULL, " ");

    assert(!strcmp(command, "/diseaseFrequency"));

    // Checking the number of arguments provided
    if(date2_str == NULL){
        printf("Usage: /diseaseFrequency disease date1 date2 [country]\n");
        return;
    }

    if(strtok(NULL, " ") != NULL){
        printf("Usage: /diseaseFrequency disease date1 date2 [country]\nToo many arguments.\n");
        return;
    }

    // Checking that the disease that the user gave exists in the diseaseHashTable
    if(!StaticHashTable_Exists(diseaseMonitor->diseaseHashTable, String_lowercase(disease))){
        printf("Disease %s does not have any outbreaks.\n", disease);
        return;
    }

    // Checking the validity of the dates provided as the previous function
    Date start = Date_fromStr(date1_str);
    Date end   = Date_fromStr(date2_str);

    if(start.day == -1){
        printf("diseaseFrequency: Start date has invalid date or format(should be DD-MM-YYYY)\n");
        return;
    }

    if(end.day == -1){
        printf("diseaseFrequency: End date has invalid date or format(should be DD-MM-YYYY)\n");
        return;
    }

    if(Date_isGreater(start, end)){
        printf("diseaseFrequency: Start date should be before end date\n");
        return;
    }

    // Calling the necessary function to print the result.
    if(country == NULL){
        StaticHashTable_diseaseFrequency(diseaseMonitor->diseaseHashTable,
                                String_lowercase(disease), NULL, start, end);
    }else{
        StaticHashTable_diseaseFrequency(diseaseMonitor->diseaseHashTable,
                    String_lowercase(disease), String_lowercase(country), start, end);
    }

    return;
}

// Function for /topk-Diseases command
void DiseaseMonitor_topKDiseases(DiseaseMonitorPtr diseaseMonitor, char* cmd_line){
    char* command   = strtok(cmd_line, " ");
    char* k_str     = strtok(NULL, " ");
    char* country   = strtok(NULL, " ");
    char* date1_str = strtok(NULL, " ");
    char* date2_str = strtok(NULL, " ");

    assert(!strcmp(command,"/topk-Diseases"));

    // Checking the number of arguments provided
    if(country == NULL){
        printf("Usage: /topk-Diseases k country [date1 date2]\n");
        return;
    }

    if(strtok(NULL, " ") != NULL){
        printf("Usage: /topk-Diseases k country [date1 date2]\nToo many arguments.\n");
        return;
    }

    // Checking if the arguments satisfy the requirements
    if(date1_str != NULL && date2_str == NULL){
        printf("topk-Diseases: when you provide start date you should provide end date.\n");
        return;
    }

    int k = atoi(k_str);

    if(k <= 0){
        printf("topk-Diseases: k is invalid or you should give a number greater than zero.\n");
        return;
    }

    // Checking if the country provided has any records.
    if(!StaticHashTable_Exists(diseaseMonitor->countryHashTable, String_lowercase(country))){
        printf("Country %s does not have any outbreaks.\n", country);
        return;
    }

    Date start = {-1,-1,-1};
    Date end = {-1,-1,-1};

    if(date2_str != NULL){
        assert(date1_str != NULL);

        // Checking the validity of the dates provided
        start = Date_fromStr(date1_str);
        end   = Date_fromStr(date2_str);

        if(start.day == -1){
            printf("topk-Diseases: Start date has invalid date or format(should be DD-MM-YYYY)\n");
            return;
        }

        if(end.day == -1){
            printf("topk-Diseases: End date has invalid date or format(should be DD-MM-YYYY)\n");
            return;
        }

        if(Date_isGreater(start,end)){
            printf("topk-Diseases: Start date should be before end date.\n");
            return;
        }
    }

    // Calling the necessary function
    StaticHashTable_printTopKDiseases(diseaseMonitor->countryHashTable, k,
                                String_lowercase(country), start, end);
}

// Function for /topk-Countries command
// I do the same things as above function
void DiseaseMonitor_topKCountries(DiseaseMonitorPtr diseaseMonitor, char* cmd_line){
    char* command   = strtok(cmd_line, " ");
    char* k_str     = strtok(NULL, " ");
    char* disease   = strtok(NULL, " ");
    char* date1_str = strtok(NULL, " ");
    char* date2_str = strtok(NULL, " ");

    assert(!strcmp(command,"/topk-Countries"));

    if(disease == NULL){
        printf("Usage: /topk-Countries k disease [date1 date2]\n");
        return;
    }

    if(strtok(NULL, " ") != NULL){
        printf("Usage: /topk-Countries k disease [date1 date2]\nToo many arguments.\n");
        return;
    }

    if(date1_str != NULL && date2_str == NULL){
        printf("topk-Countries: when you provide start date you should provide end date.\n");
        return;
    }

    int k = atoi(k_str);

    if(k <= 0){
        printf("topk-Countries: k is invalid or you should give a number greater than zero.\n");
        return;
    }

    if(!StaticHashTable_Exists(diseaseMonitor->diseaseHashTable, String_lowercase(disease))){
        printf("Disease %s does not have any outbreaks.\n", disease);
        return;
    }

    Date start = {-1,-1,-1};
    Date end = {-1,-1,-1};

    if(date2_str != NULL){
        assert(date1_str != NULL);

        start = Date_fromStr(date1_str);
        end   = Date_fromStr(date2_str);

        if(start.day == -1){
            printf("topk-Countries: Start date has invalid date or format(should be DD-MM-YYYY)\n");
            return;
        }

        if(end.day == -1){
            printf("topk-Countries: End date has invalid date or format(should be DD-MM-YYYY)\n");
            return;
        }

        if(Date_isGreater(start,end)){
            printf("topk-Countries: Start date should be before end date.\n");
            return;
        }
    }

    StaticHashTable_printTopKCountries(diseaseMonitor->diseaseHashTable, k,
                               String_lowercase(disease), start, end);
}

// Function for /insertPatientRecord command
void DiseaseMonitor_InsertPatient(DiseaseMonitorPtr diseaseMonitor, char* cmd_line){
    char* command   = strtok(cmd_line, " ");
    char* recordID  = strtok(NULL, " ");
    char* firstName = strtok(NULL, " ");
    char* lastName  = strtok(NULL, " ");
    char* diseaseID = String_lowercase(strtok(NULL, " "));
    char* country   = String_lowercase(strtok(NULL, " "));
    char* entryDate_str = strtok(NULL, " ");
    char* exitDate_str  = strtok(NULL, " ");

    Date entryDate = {-1,-1,-1};
    Date exitDate  = {-1,-1,-1};

    assert(!strcmp(command, "/insertPatientRecord"));

    // Checking the number of the arguments provided
    if(strtok(NULL," ") != NULL){
        printf("Usage: /insertPatientRecord recordID patientFirstName patientLastName diseaseID entryDate [exitDate]\n");
    }
    else if(entryDate_str == NULL){
        printf("Usage: /insertPatientRecord recordID patientFirstName patientLastName diseaseID entryDate [exitDate]\n");
    } else{
        // Checking if the input record's ID already exists.
        if(RecordsHashTable_Search(diseaseMonitor->recordsSet, recordID) != NULL){
            printf("insertPatientRecord: Record ID %s already exists.\n", recordID);
            return;
        }

        // Checking if the strings provided are valid and if the date strings
        // are valid.
        if(!String_onlyLetters(firstName, -1)){
            printf("insertPatientRecord: First Name should contain only letters\n");
        }
        else if(!String_onlyLetters(lastName, -1)){
            printf("insertPatientRecord: Last Name should contain only letters\n");
        }
        else if((!String_onlyLettersAndNums(diseaseID,'-')) || (String_howManyOf(diseaseID,'-') > 1)){
            printf("insertPatientRecord: Disease ID should contain only letters, numbers and at least\none dash\n");
        }
        else if(!String_onlyLetters(country, -1)){
            printf("insertPatientRecord: Country should contain only letters\n");
        } 
        else if((entryDate = Date_fromStr(entryDate_str)).day == -1){
            printf("insertPatientRecord: Invalid Date or invalid date format\n");
        } 
        else if((exitDate = Date_fromStr(exitDate_str)).day == -1 && (exitDate_str != NULL)){
            printf("insertPatientRecord: Invalid Date or invalid date format\n");
        }
        else if(Date_isGreater(entryDate, exitDate) && (exitDate_str != NULL)){
            printf("insertPatientRecord: Entry date should be before exit date\n");
        }
        else{   // If everything is ok I insert the record in the records hash table and I configure properly
                // the other disease monitor's data structures.
            Record* record = NULL;

            Record_Initialize(&record, recordID, firstName, lastName, diseaseID, country, entryDate, exitDate);
            
            RecordsHashTable_Insert(diseaseMonitor->recordsSet, recordID, record);
            StaticHashTable_Insert(diseaseMonitor->diseaseHashTable, record->diseaseID, record);
            StaticHashTable_Insert(diseaseMonitor->countryHashTable, record->country, record);
        }
    }

    return;
}

// Function for /recordPatientExit command
void DiseaseMonitor_RecordPatientExit(DiseaseMonitorPtr diseaseMonitor, char* cmd_line){
    char* command  = strtok(cmd_line, " ");
    char* recordID = strtok(NULL, " ");
    char* newDate_str = strtok(NULL, " ");

    Record* curr_rec;

    assert(!strcmp(command, "/recordPatientExit"));

    // Checking the number of the arguments provided.
    if(newDate_str == NULL){
        printf("Usage: /recordPatientExit recordID exitDate\n");
        return;
    }

    if(strtok(NULL, " ") != NULL){
        printf("Usage: /recordPatientExit recordID exitDate\n");
        return;        
    }

    // Checking if the record with that ID exists
    if((curr_rec = RecordsHashTable_Search(diseaseMonitor->recordsSet, recordID)) == NULL){
        printf("recordPatientExit: Record ID %s does not exist.\n", recordID);
        return;
    }

    Date new_date = Date_fromStr(newDate_str);

    // checking the validity of the date given in order to convert the string
    if(new_date.day == -1){
        printf("recordPatientExit: Invalid Date or invalid date format\n");
        return;
    }

    // checking if new exit date is greater than the record's entry date.
    if(Date_isGreater(curr_rec->entry_date, new_date)){
        printf("recordPatientExit: New exit date cannot be before than record's entry date\n");
        return;
    }

    // If everything is ok I do the change
    RecordsHashTable_UpdateExit(diseaseMonitor->recordsSet, recordID, new_date);

    return;
}

void DiseaseMonitor_numCurrentPatients(DiseaseMonitorPtr diseaseMonitor, char* cmd_line){
    char* command = strtok(cmd_line," ");
    char* disease = strtok(NULL," ");

    assert(!strcmp(command,"/numCurrentPatients"));

    // Checking the number of the arguments provided
    if(strtok(NULL," ") != NULL){
        printf("Usage: /numCurrentPatients [disease]\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // getting system's current date
    Date curr_date;
    curr_date.day   = tm.tm_mday;
    curr_date.month = tm.tm_mon + 1;
    curr_date.year  = tm.tm_year + 1900;

    if(disease != NULL){
        disease = String_lowercase(disease);

        // Checking that the disease that the user gave(if he gave on)
        // exists in the diseaseHashTable
        if(!StaticHashTable_Exists(diseaseMonitor->diseaseHashTable, disease)){
            printf("Disease %s does not have any outbreaks.\n", disease);
            return;
        }
    }

    // I call the function that used at diseaseFrequency command
    // because this command is like calling diseaseFrequency without
    // country and the start and end date are the current date
    StaticHashTable_diseaseFrequency(diseaseMonitor->diseaseHashTable, disease, NULL, curr_date, curr_date);
}

// exiting the disease monitor and de-allocating all the memory
void DiseaseMonitor_Exit(DiseaseMonitorPtr* diseaseMonitor){
    StaticHashTable_Destroy(&((*diseaseMonitor)->diseaseHashTable));
    StaticHashTable_Destroy(&((*diseaseMonitor)->countryHashTable));

    RecordsHashTable_Destroy(&((*diseaseMonitor)->recordsSet));

    free(*diseaseMonitor);
    *diseaseMonitor = NULL;

    return;
}