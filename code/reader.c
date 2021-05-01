#include "../lib/reader.h"

int strcount(char* str, char c) {
    int r = 0;
    for(int i = 0; str[i]; i++){
        r += str[i] == c; 
    }
    return r;
}

char* ltrim(char* s)
{
    while(isspace(*s)) s++;
    return s;
}

char* rtrim(char* s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char* trim(char* s)
{
    return rtrim(ltrim(s)); 
}