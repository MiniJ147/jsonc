#include "parsers.h"
#include <stdio.h>
#include <string.h>

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
        case '\t': return TAB;

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

void parse_string(token* tok, int* tracker, char *str) {
    int i; int size = 1; // 1 so it accounts for \0

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
    char* new_str = malloc(size);
    char* tp = new_str;

    // parsing string
    for(i=1; i<size; i++){
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

    // remember new_str is strdup so we can reuse this value
    token_init(tok, STRING, size, new_str, -1,-1);
    token_write_data(tok, new_str, size);

    free(new_str);
    *tracker+=i; // to skip over extra " 
}

void parse_number(token* tok, int *tracker, char *input_str){
    // sizeof int and float are same here so it doesn't matter which we use
    token_init(tok, GENERIC_NUMBER, sizeof(int), NULL, -1, -1);
    
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
        *(float*)tok->m_data = atof(input_str);
    }else{
        tok->type = INT;
        *(int*)tok->m_data = atoi(input_str);
    }
    token_strdup_str(tok, input_str);

    // replacing orginal value back
    input_str[i] = tmp;
    
    *tracker+=i-1; // adjust one back so for loop can skip
}


void parse_boolean(token* tok, int *tracker, char *input_str){
    token_init(tok, BOOLEAN, sizeof(int), NULL, -1, -1);
    int* data_dest = (int*)tok->m_data;

    if(strncmp(input_str,"true",4)==0){
        token_strdup_str(tok,"true");
        *data_dest = 1;
    }else if(strncmp(input_str,"false",4)==0){
        token_strdup_str(tok,"false");
        *data_dest = 0;
    }else{
        tok->type = INVALID;
        *data_dest = 0;
        // printf("invalid\n");
    }

    *tracker+=3; // adjust one back so for loop can skip
}

token* lexer_tokenize(char* json_str, int* size_cnt) {
    int size = 0;
    token* token_arr = token_init_arr(strlen(json_str)); // max amount of tokens ever needed;
    
    for(int i=0; json_str[i]!='\0'; i++){
        token_type tok_type = char_to_token(json_str[i]);
        token* tok_curr = &token_arr[size];

        switch(tok_type){
            case QUOTE: 
            parse_string(tok_curr,&i,&json_str[i]);
            printf("STRING: %s %zu %s |\n",tok_curr->m_str,tok_curr->mem_size,(char*)tok_curr->m_data);
            
            size++;
            break;
            
            case GENERIC_NUMBER: 
            parse_number(tok_curr, &i, &json_str[i]);
            
            if(tok_curr->type==FLOAT){
                printf("FLOAT: %s %zu %f |\n",tok_curr->m_str, tok_curr->mem_size, *(float*)tok_curr->m_data);
            }else if(tok_curr->type==INT){
                printf("INT: %s %zu %d |\n", tok_curr->m_str, tok_curr->mem_size, *(int*)tok_curr->m_data);
            }
          
            size++;  
            break;
            case BOOLEAN: 
            parse_boolean(tok_curr, &i, &json_str[i]); 
            printf("BOOL: %s %zu %d |\n",tok_curr->m_str, tok_curr->mem_size, *(int*)tok_curr->m_data);
            
            size++;
            break;
            case SPACE: break;
            case NEWLINE:break;

            default:
            if(tok_type==INVALID){
                printf("WARNING INVALID CHAR AT pos %d | char value: %c | int value: %d\n",i,json_str[i], json_str[i]);
                continue;
            }

            char str[2] = "_\0";
            str[0] = json_str[i];

            token_init(tok_curr,tok_type,0,str,-1,-1);
            size++;
        }
    }

    *size_cnt = size;
    return token_arr;
}