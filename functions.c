#include "functions.h"
#include <string.h>
#include <unistd.h>

void print(const char* str){
    write(1, str, strlen(str)+1);
}

void perror(const char* str){
    write(2, str, strlen(str)+1);
}

void int_to_string(char* res, int num){
    res[0] = '0' + (num/10);
    res[1] = '0' + (num%10);
    res[2] = '\0';
}

void remove_substring(char *s,const char *toremove)
{
    memmove(s,s+strlen(toremove),1+strlen(s+strlen(toremove)));
}
