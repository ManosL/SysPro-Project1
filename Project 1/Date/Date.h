#ifndef __DATE_H__
#define __DATE_H__

#include <stdbool.h>

typedef struct Date Date;

struct Date{
    short day;
    short month;
    short year;
};

bool Date_isGreater(Date d1, Date d2);
bool Date_isGreaterEqual(Date d1, Date d2);
bool Date_isLess(Date d1, Date d2);
bool Date_isLessEqual(Date d1, Date d2);
bool Date_areEqual(Date d1, Date d2);
void Date_Copy(Date src, Date* dest);

Date Date_fromStr(char* d_str);
#endif