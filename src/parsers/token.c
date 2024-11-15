#include "parsers.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

token* token_init_arr(int n) {
    return calloc(n, sizeof(token));
}

void token_init(token* token, token_type type, size_t mem_size, char* raw_str, int line, int col) {    
    token->type = type;
    token->col = col;
    token->line = line;
    token->mem_size = mem_size;
    
    token->m_str = NULL;
    token->m_data = NULL;

    if(raw_str!=NULL){
        token->m_str = strdup(raw_str);
    }

    if(mem_size>0){
        token->m_data = malloc(mem_size);
    }
}


void token_internal_destory(token* token) {
    assert(token!=NULL);

    if(token->m_str != NULL){
        free(token->m_str);
    } 
    
    if(token->m_data != NULL){
        free(token->m_data);
    }
}

void token_strdup_str(token* token, char* str){
    assert(token!=NULL);
    assert(token->m_str==NULL);
    token->m_str = strdup(str);
}


void token_free_arr(token* token_arr, int size) {
    for(int i=0; i<size; i++){
        token_internal_destory(&token_arr[i]);
    }
    free(token_arr);
}

void token_write_data(token* token, void* data, size_t size){
    assert(token!=NULL);
    assert(data!=NULL);
    assert(size<=token->mem_size);
    
    memcpy(token->m_data,data,size);
}