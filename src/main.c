#include <stdio.h>
#include <assert.h>

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
    test v = {10,20};

    
    assert(&v==editTest(&v,0));
    assert(&v.val1==editTest(&v,0));
    assert(&v.val2==editTest(&v,4));
    
    return 0;
}