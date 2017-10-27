#include "functions.h"
#include <string.h>
#include <unistd.h>

void print(const char* str){
    write(1, str, strlen(str)+1);
}

void perror(const char* str){
    write(2, str, strlen(str)+1);
}
