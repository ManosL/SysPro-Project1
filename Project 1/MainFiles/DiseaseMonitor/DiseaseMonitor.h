#ifndef __DISEASE_MONITOR__
#define __DISEASE_MONITOR__

typedef struct DiseaseMonitor* DiseaseMonitorPtr;

int  DiseaseMonitor_Create(DiseaseMonitorPtr*,int,int,int,long (*hashFn)(char*));
int  DiseaseMonitor_InitializeData(DiseaseMonitorPtr, FILE*);
void DiseaseMonitor_Run(DiseaseMonitorPtr);
void DiseaseMonitor_Exit(DiseaseMonitorPtr*);

#endif