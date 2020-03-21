#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "Date.h"

// Simple date utilities functions...
// By convention the "NULL" date is the date that all its members
// equal to -1
 
bool Date_isGreater(Date d1, Date d2){
    return (d1.year > d2.year) || 
    ((d1.year == d2.year) && (d1.month > d2.month)) ||
    ((d1.year == d2.year) && (d1.month == d2.month) && (d1.day > d2.day));
}

bool Date_isGreaterEqual(Date d1, Date d2){
    return (d1.year >= d2.year) || 
    ((d1.year == d2.year) && (d1.month >= d2.month)) ||
    ((d1.year == d2.year) && (d1.month == d2.month) && (d1.day >= d2.day));
}

bool Date_isLess(Date d1, Date d2){
    return (d1.year < d2.year) || 
    ((d1.year == d2.year) && (d1.month < d2.month)) ||
    ((d1.year == d2.year) && (d1.month == d2.month) && (d1.day < d2.day));
}

bool Date_isLessEqual(Date d1, Date d2){
    return (d1.year <= d2.year) || 
    ((d1.year == d2.year) && (d1.month <= d2.month)) ||
    ((d1.year == d2.year) && (d1.month == d2.month) && (d1.day <= d2.day));
}

bool Date_areEqual(Date d1, Date d2){
    return (d1.year == d2.year) && (d1.month == d2.month) && (d1.day == d2.day);
}

void Date_Copy(Date src, Date* dest){
    dest->year = src.year;
    dest->month = src.month;
    dest->day = src.day;
}

int Date_isLeapYear(int year){
    if(year % 4 != 0){
        return 0;
    } else if(year % 100 != 0){
        return 1;
    }else if(year % 400 != 0){
        return 0;
    }

    return 1;
}

Date Date_fromStr(char* d_str){
    Date result;
    result.day = result.month = result.year = -1;

    char* day_str = strtok(d_str,"-");
    char* month_str = strtok(NULL,"-");
    char* year_str  = strtok(NULL,"-");

    if(strtok(NULL,"-") != NULL){
        //printf("Invalid Date Format %s\n", d_str);
        return result;
    }

    if(day_str == NULL || month_str == NULL || year_str == NULL){
        //printf("Invalid Date Format %s\n",d_str);
        return result;
    }

    short day = (short) atoi(day_str);
    short month = (short) atoi(month_str);
    short year = (short) atoi(year_str);

    if(day == 0 || month == 0 || year == 0){
        //printf("Invalid Date %s\n",d_str);
        return result;
    }

    if(day > 31){
        //printf("Invalid Date %s\n",d_str);
        return result;
    }

    if(day > 29 && month == 2){
        //printf("Invalid Date %s\n", d_str);
        return result;
    }

    if(day == 29 && month == 2 && !Date_isLeapYear(year)){
        return result;
    }

    if(day > 30 && (month == 4 || month == 6 || month == 9 || month == 11)){
        //printf("Invalid Date %s\n", d_str);
        return result;
    }

    result.day = day;
    result.month = month;
    result.year = year;
    
    return result;
}
