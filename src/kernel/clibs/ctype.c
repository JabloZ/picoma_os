
#include "ctype.h"
bool islower(char c){
    if (c>='a' && c<='z'){
        return true;
    }
    return false;
}
bool isupper(char c){
    if (c>='A' && c<='Z'){
        return true;
    }
    return false;
}
char tolower(char c){
    if (isupper){
        return c+('a'-'A');
    }
    return c;
}
char toupper(char c){
    if (islower(c)){
        return c-('a'-'A');
    }
    return c;
}