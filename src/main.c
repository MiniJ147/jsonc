#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define KILOBYTE 1000 //1000 bytes
#define MAX_FILE_SIZE KILOBYTE * 56// 56kb max size

// reads file 
// returns string with number of bytes read
// can return up to MAX_FILE_SIZE
// calls malloc
char* readFile(const char* path){    
    FILE *fptr = fopen(path,"r");
    assert(fptr!=NULL);

    char buff[MAX_FILE_SIZE];
    
    size_t bytesRead = fread(buff,sizeof(char),MAX_FILE_SIZE+1,fptr); // read 1 for file size thus, read MAX_FILE_SZIE bytes
    assert(bytesRead<=MAX_FILE_SIZE);
    
    char* result = (char*)malloc(bytesRead);
    strcpy(result, buff);
    
    return result;
}

typedef struct test test;
struct test{
    int val1;
    int val2;
};

void* editTest(void* dest, size_t bytes){
    char* cast = (char*)dest;
    return (void*)(cast+bytes);
}

int main(){
    char* JSON = readFile("../test/test1.json");
    printf("%s\n",JSON);
    // test v = {10,20};

    
    // assert(&v==editTest(&v,0));
    // assert(&v.val1==editTest(&v,0));
    // assert(&v.val2==editTest(&v,4));
    
    return 0;
}