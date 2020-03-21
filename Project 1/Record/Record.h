#ifndef __RECORD_H__
#define __RECORD_H__

#include "../Date/Date.h"

typedef struct Record Record;

struct Record{
    char* recordID;
    char* first_name;
    char* last_name;
    char* diseaseID;
    char* country;
    Date entry_date;
    Date exit_date;
};

void Record_Initialize(Record** record, char* recordID, char* firstName, char* lastName,
                    char* disease, char* country, Date entry, Date exit);

void Record_Destroy(Record** record);

#endif