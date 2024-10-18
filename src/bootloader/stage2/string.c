#include "string.h"
#include "stdint.h"

const char* strchr(const char* str, char chr){
    if(str==NULL){
        return NULL;
    }
    while (*str){
        if (*str==chr){
            return str;
        }
        ++str;
    }
    return NULL;
}
char* strcpy(char* destination, char* source){
    char* original=destination;
    if (destination==NULL){
        return NULL;
    }
    if (source==NULL){
        *destination='\0';
        return destination;
    }
    while(*source){
        *destination=*source;
        ++source;
        ++destination;
    }
    *destination='\0';
    return original;
}
unsigned strlen(const char* str){
    unsigned count=0;
    while (*str){
        ++count;
        ++str;
    }
    return count;
}
int strcmp(const char* str1, const char* str2, int len){
    
    for (int i=0; i<len; i++){
        if (str1[i]!=str2[i]){
            return 0;
        }
    }
    return 1;
}