#include <string.h>

// string utilities functions that help me

char* String_lowercase(char* str){
    for(int i = 0; i < strlen(str); i++){
        if(str[i] >= 'A' && str[i] <= 'Z')
            str[i] += 32;
    }

    return str;
}

// It helps me have and exception character to just ignore it
// exception = -1 if I dont want to have exceptions
int String_onlyLetters(char* str, char exception){
    for(int i = 0; i < strlen(str); i++){
        if((!(str[i] >= 'a' && str[i] <= 'z')) && (str[i] != exception) && 
            (!(str[i] >= 'A' && str[i] <= 'Z')))
        {
            return 0;
        }
    }

    return 1;
}

int String_howManyOf(char* str, char c){
    int num = 0;

    for(int i = 0;i < strlen(str); i++){
        if(str[i] == c)
            num++;
    }

    return num;
}

// It helps me have and exception character to just ignore it
// exception = -1 if I dont want to have exceptions
int String_onlyLettersAndNums(char* str, char exception){
    for(int i = 0; i < strlen(str); i++){
        if((!(str[i] >= '0' && str[i] <= '9')) && (str[i] != exception) &&
            (!(str[i] >= 'a' && str[i] <= 'z')) && (!(str[i] >= 'A' && str[i] <= 'Z')))
        {
            return 0;
        }
    }

    return 1;   
}

char* String_removeWhitespaceSuffix(char* str){
    for(int i = strlen(str) - 1;i >= 0;i--){
        if(str[i] == '\n' || str[i] == ' ' || str[i] == '\t'){
            str[i] = '\0';
        }
        else{
            break;
        }
    }

    return str;
}