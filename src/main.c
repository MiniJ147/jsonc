#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "linker.h"

#define KILOBYTE 1000 //1000 bytes
#define MAX_FILE_SIZE KILOBYTE * 56// 56kb max size

// reads file 
// returns string with number of bytes read
// can return up to MAX_FILE_SIZE
// calls malloc
char* read_file(const char* path){    
    FILE *fptr = fopen(path,"r");
    assert(fptr!=NULL);

    char buff[MAX_FILE_SIZE];
    
    size_t bytesRead = fread(buff,sizeof(char),MAX_FILE_SIZE+1,fptr); // read 1 for file size thus, read MAX_FILE_SZIE bytes
    assert(bytesRead<=MAX_FILE_SIZE);
    
    char* result = (char*)malloc(bytesRead);
    strcpy(result,buff);

    return result;
}

int main(){
    char* json_str = read_file("../test/test1.json");
    lexer_tokenizer(json_str);

    return 0;
}