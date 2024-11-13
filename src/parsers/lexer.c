#include "parsers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void token_print(token tok){
    printf("Token ID: %d\nLine: %d\nCol: %d\nStr: %s\n",tok.type,tok.line,tok.col,tok.str);
}

token* token_init_null(){
    return calloc(1, sizeof(token));
}

token* token_create(token_type type, size_t mem_size, char* str, int line, int col){
    token *tok = malloc(sizeof(token));
    
    tok->type=type;
    tok->mem_size = mem_size;
    tok->str = str;
    tok->line = line;
    tok->col = col;
    tok->data = malloc(mem_size);
    
    return tok;
}

void token_destory(token* tok){
    assert(tok!=NULL);

    free(tok->str);
    free(tok->data);
    free(tok);
}


// token *lexer_free(token* tokens, int size){
//     for(int i=0; i<size; i++){
//         free(tokens[i]);
//     }
// }

token_type char_to_token(char value){
    switch(value){
        case '{': return LEFT_BRACE;
        case '}': return RIGHT_BRACE;
        case '[': return LEFT_BRACKET;
        case ']': return RIGHT_BRACKET;
        case ',': return COMMA;
        case ':': return COLON;
        case '\"': return QUOTE;
        case '\\': return BACKSLASH; 
        case '\n': return NEWLINE;
        case ' ': return SPACE;

        // why do these work? 
        // well if they are in a string we will skip over these values 
        // thus, the only time we see these chars is if they are a number or a boolean
        case '-': return GENERIC_NUMBER;
        case 't': return BOOLEAN;
        case 'f': return BOOLEAN;
    }

    if(value >= '0' && value <= '9'){
        return GENERIC_NUMBER;
    }

    if(value >= 'a' && value <= 'z'){
        return CHAR;
    }

    return INVALID; 
}

// pass at the qoute mark
token *parse_string(int* tracker, char *str) {
    int i; int size = 0;
    token *tok;

    // getting size
    for(int j=1; str[j]!='\0';j++){
        token_type type = char_to_token(str[j]);
        token_type prev_type = char_to_token(str[j-1]);

        if(type==QUOTE && prev_type!=BACKSLASH){
            break;
        }

        size++;
    }

    // printf("size:%d\n",size);
    char* new_str = malloc(size+1);
    char* tp = new_str;

    // parsing string
    for(i=1; i<size+1; i++){
        token_type type = char_to_token(str[i]);
        token_type prev_type = char_to_token(str[i-1]);

        //handle escape char
        if(prev_type==BACKSLASH){
            switch(str[i]){
                case 'n': *tp++ = '\n'; break;
                case 'f': *tp++ = '\f'; break;
                case 'b': *tp++ = '\b'; break;
                case 'r': *tp++ = '\r'; break;
                case 't': *tp++ = '\t'; break;
                case '"': *tp++ = '\"'; break;
                default: *tp++ = str[i];
            }
        }else if(type!=BACKSLASH){
            *tp++ = str[i];
        }
    }
    *tp = '\0'; //ending string

    tok = token_create(STRING,size,new_str,-1,-1);
    memcpy(tok->data,new_str,size);

    // printf("%s string found %d long\n",new_str, size);

    *tracker+=i; // adjust our postion in file
    
    return tok; 
}

// TODO: boolean, int, float

// on begining of boolean
token* parse_boolean(int *tracker, char *input_str){
    token* tok = token_create(BOOLEAN, sizeof(int), NULL, -1, -1);
    int* data_dest = (int*)tok->data;

    if(strncmp(input_str,"true",4)==0){
        tok->str = strdup("true");
        *data_dest = 1;
    }else if(strncmp(input_str,"false",4)==0){
        tok->str = strdup("false");
        *data_dest = 0;
    }else{
        tok->type = INVALID;
        *data_dest = 0;
        // printf("invalid\n");
    }

    *tracker+=4;
    return tok;
}

// on begining of number
token* parse_number(int *tracker, char *input_str){
    // sizeof int and float are same here so it doesn't matter which we use
    token* tok = token_create(GENERIC_NUMBER, sizeof(int), NULL, -1, -1);
    
    int i;
    int is_float = 0;

    // must be 0-9 or a (.) or (-)
    for(i=0; input_str[i]>='0' && input_str[i]<='9' || input_str[i]=='.' || input_str[i]=='-'; i++){
        if(input_str[i]=='.'){
            is_float = 1;
        }
    }

    //marking end of string for atoi & atof functions
    char tmp = input_str[i];
    input_str[i] = '\0';
    
    if(is_float){
        tok->type = FLOAT;
        *(float*)tok->data = atof(input_str);
    }else{
        tok->type = INT;
        *(int*)tok->data = atoi(input_str);
    }

    tok->str = strdup(input_str);

    // replacing orginal value back
    input_str[i] = tmp;
    
    *tracker+=i;
    return tok;
}

token *lexer_tokenizer(char *input_str, int* size) {
    *size = strlen(input_str);
    token *tokens = malloc(sizeof(token) * (*size));

    for(int i=0; input_str[i]!='\0'; i++){
        // printf("it: %d\n",i);
        token* tok;
        token_type type = char_to_token(input_str[i]);
        switch(type){
            case QUOTE: 
            tok = parse_string(&i,&input_str[i]);
            // printf("STRING: %s %zu %s |\n",tok->str,tok->mem_size,(char*)tok->str);
            token_destory(tok);
            break;
            case GENERIC_NUMBER: 
            tok = parse_number(&i, &input_str[i]);
            if(tok->type==FLOAT){
                printf("FLOAT: %s %zu %f |\n",tok->str, tok->mem_size, *(float*)tok->data);
            }else if(tok->type==INT){
                printf("INT: %s %zu %d |\n", tok->str, tok->mem_size, *(int*)tok->data);
            }
            token_destory(tok);
            break;
            case BOOLEAN: 
            tok = parse_boolean(&i, &input_str[i]); 
            // printf("BOOL: %s %zu %d |\n",tok->str, tok->mem_size, *(int*)tok->data);
            
            token_destory(tok);
            break;
        }
    }

    return tokens;
}
