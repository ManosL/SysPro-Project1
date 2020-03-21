#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RecPtrLinkedList.h"

typedef struct ListNode ListNode;

struct RecPtrLinkedList{
    ListNode* head;
    int size;
};

struct ListNode{
    Record* record;
    ListNode* next;
};

void RecPtrLinkedList_Create(RecPtrLinkedListPtr* listPtr){
    *listPtr = malloc(sizeof(struct RecPtrLinkedList));

    (*listPtr)->head = NULL;
    (*listPtr)->size = 0;
}

int RecPtrLinkedList_Length(RecPtrLinkedListPtr listPtr){
    return listPtr->size;
}

// Inserting an element to the beginning of the list
void RecPtrLinkedList_Insert(RecPtrLinkedListPtr listPtr, Record* recAddr){
    if(listPtr->head == NULL){
        listPtr->head = malloc(sizeof(struct ListNode));
        
        listPtr->head->record = recAddr;
        listPtr->head->next = NULL;
    }
    else{
        ListNode* temp = malloc(sizeof(struct ListNode));

        temp->record = recAddr;
        temp->next = listPtr->head;

        listPtr->head = temp;
    }

    (listPtr->size)++;

    return;
}

// Counting the records on the list that have country equals with the argument
// (if i dont want to check that country == NULL) and they're recorded in the monitor
// in [start,end] dates(if i don't want to check that the dates have all their fields)
// as -1

int RecPtrLinkedList_countryDateFrequency(RecPtrLinkedListPtr listPtr, char* country, Date start, Date end){
    int frequency = 0;

    ListNode* curr_node = listPtr->head;

    while(curr_node != NULL){
        if(country != NULL){
            if(!strcmp(curr_node->record->country, country)){
                if(start.day == -1){
                    frequency++;
                }
                else if((Date_isGreaterEqual(curr_node->record->entry_date, start) && Date_isLessEqual(curr_node->record->entry_date, end)) || 
                        (Date_isGreaterEqual(curr_node->record->exit_date, start) && Date_isLessEqual(curr_node->record->exit_date, end)) ||
                        ((Date_isGreater(curr_node->record->exit_date, end) || curr_node->record->exit_date.day == -1)
                        && Date_isLess(curr_node->record->entry_date, start)))
                {
                    frequency++;
                }
            }
        } else{
            if(start.day == -1){
                frequency++;
            }
            else if((Date_isGreaterEqual(curr_node->record->entry_date, start) && Date_isLessEqual(curr_node->record->entry_date, end)) || 
                    (Date_isGreaterEqual(curr_node->record->exit_date, start) && Date_isLessEqual(curr_node->record->exit_date, end)) ||
                    ((Date_isGreater(curr_node->record->exit_date, end) || curr_node->record->exit_date.day == -1)
                        && Date_isLess(curr_node->record->entry_date, start)))
            {
                frequency++;
            }
        }

        curr_node = curr_node->next;
    }

    return frequency;
}


// Just calls the function that adds 1 to the corresponding heap element
void RecPtrLinkedList_setDiseaseHeap(RecPtrLinkedListPtr listPtr, Date start, Date end, MaxHeapPtr heap){
    ListNode* curr_node = listPtr->head;

    while(curr_node != NULL){
        if(start.day == -1){            // if we dont have date restriction
            MaxHeap_addOne(heap, curr_node->record->diseaseID);
        }
        else if((Date_isGreaterEqual(curr_node->record->entry_date, start) && Date_isLessEqual(curr_node->record->entry_date, end)) || 
            (Date_isGreaterEqual(curr_node->record->exit_date, start) && Date_isLessEqual(curr_node->record->exit_date, end)) ||
            ((Date_isGreater(curr_node->record->exit_date, end) || curr_node->record->exit_date.day == -1)
            && Date_isLess(curr_node->record->entry_date, start)))
        {
            MaxHeap_addOne(heap, curr_node->record->diseaseID);
        }

        curr_node = curr_node->next;
    }
}

// It's the same function as above with only difference which is 
// the element key id that i will search.
void RecPtrLinkedList_setCountryHeap(RecPtrLinkedListPtr listPtr, Date start, Date end, MaxHeapPtr heap){
    ListNode* curr_node = listPtr->head;

    while(curr_node != NULL){
        if(start.day == -1){
            MaxHeap_addOne(heap, curr_node->record->country);
        }
        else if((Date_isGreaterEqual(curr_node->record->entry_date, start) && Date_isLessEqual(curr_node->record->entry_date, end)) || 
            (Date_isGreaterEqual(curr_node->record->exit_date, start) && Date_isLessEqual(curr_node->record->exit_date, end)) ||
            ((Date_isGreater(curr_node->record->exit_date, end) || curr_node->record->exit_date.day == -1)
            && Date_isLess(curr_node->record->entry_date, start)))
        {
            MaxHeap_addOne(heap, curr_node->record->country);
        }

        curr_node = curr_node->next;
    }
}

// destroying the list.
void RecPtrLinkedList_Destroy(RecPtrLinkedListPtr* listPtr){
    ListNode* curr_node = (*listPtr)->head;

    while(curr_node != NULL){
        ListNode* temp = curr_node;
        curr_node = curr_node->next;
        free(temp);
    }

    free(*listPtr);
    *listPtr = NULL;

    return;
}