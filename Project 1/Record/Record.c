#include <stdlib.h>
#include <string.h>

#include "Record.h"

// Record helpers
void Record_Initialize(Record** record, char* recordID, char* firstName, char* lastName,
                    char* disease, char* country, Date entry, Date exit)
{
    *record = malloc(sizeof(Record));

    (*record)->recordID = malloc((strlen(recordID) + 1) * sizeof(char));
    strcpy((*record)->recordID, recordID);
    
    (*record)->first_name = malloc((strlen(firstName) + 1) * sizeof(char));
    strcpy((*record)->first_name, firstName);

    (*record)->last_name = malloc((strlen(lastName) + 1) * sizeof(char));
    strcpy((*record)->last_name, lastName);

    (*record)->diseaseID = malloc((strlen(disease) + 1) * sizeof(char));
    strcpy((*record)->diseaseID, disease);

    (*record)->country = malloc((strlen(country) + 1) * sizeof(char));
    strcpy((*record)->country, country);

    /*
    (*record)->recordID = recordID;
    (*record)->first_name = firstName;
    (*record)->last_name = lastName;
    (*record)->diseaseID = disease;
    (*record)->country = country;
    */
    Date_Copy(entry, &((*record)->entry_date));
    Date_Copy(exit, &((*record)->exit_date));

    return;
}

void Record_Destroy(Record** record){
    free((*record)->recordID);
    free((*record)->first_name);
    free((*record)->last_name);
    free((*record)->diseaseID);
    free((*record)->country);

    free(*record);
    *record = NULL;

    return;
}