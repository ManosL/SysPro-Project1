#ifndef __STR_UTILS__
#define __STR_UTILS__

char* String_lowercase(char* str);
int String_onlyLetters(char* str, char exception);
int String_howManyOf(char* str, char c);
int String_onlyLettersAndNums(char* str, char exception);
char* String_removeWhitespaceSuffix(char* str);

#endif