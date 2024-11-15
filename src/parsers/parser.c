#include "parsers.h"

#include <stdio.h>
#include <string.h>

void parser_test() {
    printf("parser test!\n");
}

size_t parser_get_size_est(token* tokens, int token_size){
    size_t total_size = 0;
    
    int can_insert = 0;
    int in_arr = 0;

    for(int i=0; i<token_size-1;i++){
        token tok_curr = tokens[i];
        switch(tok_curr.type){
            case LEFT_BRACKET: in_arr=1; break;
            case RIGHT_BRACKET: in_arr=0; break;
            case COMMA:
            if(!in_arr){
                can_insert=0;
            }
            break;
            case COLON:can_insert=1;break;
        }

        if(!can_insert){
            continue;
        }

        if(tok_curr.type==STRING || tok_curr.type==LEFT_BRACKET){
            total_size+=8; // size plus pointer to buffer
        }

        total_size+=tok_curr.mem_size;
    }
    return total_size;
}

void* parser_parse_array(token* tokens, int token_size, int* tracker){
    size_t arr_size = 0;
    int it_count=0;

    for(int i=*tracker; i<token_size;i++){
        if(tokens[i].type==RIGHT_BRACKET){
            break;
        }

        if(tokens[i].type==STRING){
            arr_size+=8;
        }else{
            arr_size+=tokens[i].mem_size;
        }
        it_count++;
    }

    if(arr_size==0){
        printf("array size 0\n");
        return NULL;
    }

    unsigned char* buff = malloc(arr_size);
    size_t ptr = 0;

    for(int i=*tracker; i<token_size; i++){
        token curr_token = tokens[i];
        token_type curr_type = curr_token.type;
        
        if(curr_type==RIGHT_BRACKET){
            break;
        }

        if(curr_type==STRING){
            char* str = strdup(curr_token.m_data);
            memcpy(buff+ptr,&str,8);
            ptr+=8;
            printf("string!!! %d\n",arr_size);
            continue;
        }

        memcpy(buff+ptr,curr_token.m_data,curr_token.mem_size);
        ptr+=curr_token.mem_size;
    }

    // printf("%zu %d %zu\n",ptr, it_count);
    *tracker+=it_count;
    return buff;
}

void* parser_parse(token* tokens, int token_size) {
    if(token_size<=0 || tokens==NULL){
        return NULL;
    }
    size_t global_size = parser_get_size_est(tokens,token_size)+24; //+24 safeguard
    size_t ptr = 0;

    unsigned char* buff_global = malloc(global_size);
    int can_insert = 0;

    int i;
    for(i=0; i<token_size-1; i++){
        // printf("it:%d\n",i);
        token curr_token = tokens[i];
        token_type curr_type = curr_token.type;

        //flag mananger
        switch(curr_type){
            case LEFT_BRACKET: 
            void* buff_arr = parser_parse_array(tokens,token_size,&i);
            memcpy(buff_global+ptr,&buff_arr,8);
            ptr+=8;
            break;
            case RIGHT_BRACKET: break;
            case COMMA: can_insert=0; break; // we don't need to worry about arrays since that is handle by the function above.
            case COLON: can_insert=1; break;
        }

        // pointless token
        if(curr_token.mem_size==0 || !can_insert){
            continue;
        }

        // writer
        if(curr_type==STRING){
            char* str = strdup(curr_token.m_data);
            memcpy(buff_global+ptr,&str,8);
            ptr+=8;
            continue;
        }

        memcpy(buff_global+ptr,curr_token.m_data,curr_token.mem_size);
        ptr+=curr_token.mem_size;


        // printf("%s | %zu | flags: %d %d \n",curr_token.m_str, curr_token.mem_size, 0, can_insert);
    }
    return buff_global;
}