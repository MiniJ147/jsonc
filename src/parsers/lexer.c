
// #include "parsers.h"
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <assert.h>

// void token_print(token tok){
//     printf("Token ID: %d\nLine: %d\nCol: %d\nStr: %s\n",tok.type,tok.line,tok.col,tok.str);
// }

// token* token_init_null(){
//     return calloc(1, sizeof(token));
// }

// void token_init(token* tok, token_type type, size_t mem_size, char* str, int line, int col){
//     tok->type=type;
//     tok->mem_size = mem_size;
//     tok->str = strdup(str);
//     tok->line = line;
//     tok->col = col;
//     tok->data = malloc(mem_size);
// }

// token* token_create(token_type type, size_t mem_size, char* str, int line, int col){
//     token *tok = malloc(sizeof(token));
    
//     tok->str = str;
//     tok->type=type;
//     tok->mem_size = mem_size;
//     tok->line = line;
//     tok->col = col;
//     tok->data = malloc(mem_size);
    
//     return tok;
// }

// void token_total_destory(token* tok){
//     assert(tok!=NULL);

//     free(tok->str);
//     free(tok->data);
//     free(tok);
// }

// void token_internal_destory(token* tok){
//     assert(tok!=NULL);

//     free(tok->str);
//     free(tok->data);
// }


// // token *lexer_free(token* tokens, int size){
// //     for(int i=0; i<size; i++){
// //         free(tokens[i]);
// //     }
// // }

// token_type char_to_token(char value){
//     switch(value){
//         case '{': return LEFT_BRACE;
//         case '}': return RIGHT_BRACE;
//         case '[': return LEFT_BRACKET;
//         case ']': return RIGHT_BRACKET;
//         case ',': return COMMA;
//         case ':': return COLON;
//         case '\"': return QUOTE;
//         case '\\': return BACKSLASH; 
//         case '\n': return NEWLINE;
//         case ' ': return SPACE;

//         // why do these work? 
//         // well if they are in a string we will skip over these values 
//         // thus, the only time we see these chars is if they are a number or a boolean
//         case '-': return GENERIC_NUMBER;
//         case 't': return BOOLEAN;
//         case 'f': return BOOLEAN;
//     }

//     if(value >= '0' && value <= '9'){
//         return GENERIC_NUMBER;
//     }

//     if(value >= 'a' && value <= 'z'){
//         return CHAR;
//     }

//     return INVALID; 
// }

// // pass at the qoute mark
// token *parse_string(int* tracker, char *str) {
//     int i; int size = 0;
//     token *tok;

//     // getting size
//     for(int j=1; str[j]!='\0';j++){
//         token_type type = char_to_token(str[j]);
//         token_type prev_type = char_to_token(str[j-1]);

//         if(type==QUOTE && prev_type!=BACKSLASH){
//             break;
//         }

//         size++;
//     }

//     // printf("size:%d\n",size);
//     char* new_str = malloc(size+1);
//     char* tp = new_str;

//     // parsing string
//     for(i=1; i<size+1; i++){
//         token_type type = char_to_token(str[i]);
//         token_type prev_type = char_to_token(str[i-1]);

//         //handle escape char
//         if(prev_type==BACKSLASH){
//             switch(str[i]){
//                 case 'n': *tp++ = '\n'; break;
//                 case 'f': *tp++ = '\f'; break;
//                 case 'b': *tp++ = '\b'; break;
//                 case 'r': *tp++ = '\r'; break;
//                 case 't': *tp++ = '\t'; break;
//                 case '"': *tp++ = '\"'; break;
//                 default: *tp++ = str[i];
//             }
//         }else if(type!=BACKSLASH){
//             *tp++ = str[i];
//         }
//     }
//     *tp = '\0'; //ending string

//     tok = token_create(STRING,size,new_str,-1,-1);
//     memcpy(tok->data,new_str,size);

//     // printf("%s string found %d long\n",new_str, size);

//     *tracker+=i; // adjust our postion in file
    
//     return tok; 
// }

// // TODO: boolean, int, float

// // on begining of boolean
// token* parse_boolean(int *tracker, char *input_str){
//     token* tok = token_create(BOOLEAN, sizeof(int), NULL, -1, -1);
//     int* data_dest = (int*)tok->data;

//     if(strncmp(input_str,"true",4)==0){
//         tok->str = strdup("true");
//         *data_dest = 1;
//     }else if(strncmp(input_str,"false",4)==0){
//         tok->str = strdup("false");
//         *data_dest = 0;
//     }else{
//         tok->type = INVALID;
//         *data_dest = 0;
//         // printf("invalid\n");
//     }

//     *tracker+=4;
//     return tok;
// }

// // on begining of number
// token* parse_number(int *tracker, char *input_str){
//     // sizeof int and float are same here so it doesn't matter which we use
//     token* tok = token_create(GENERIC_NUMBER, sizeof(int), NULL, -1, -1);
    
//     int i;
//     int is_float = 0;

//     // must be 0-9 or a (.) or (-)
//     for(i=0; input_str[i]>='0' && input_str[i]<='9' || input_str[i]=='.' || input_str[i]=='-'; i++){
//         if(input_str[i]=='.'){
//             is_float = 1;
//         }
//     }

//     //marking end of string for atoi & atof functions
//     char tmp = input_str[i];
//     input_str[i] = '\0';
    
//     if(is_float){
//         tok->type = FLOAT;
//         *(float*)tok->data = atof(input_str);
//     }else{
//         tok->type = INT;
//         *(int*)tok->data = atoi(input_str);
//     }

//     tok->str = strdup(input_str);

//     // replacing orginal value back
//     input_str[i] = tmp;
    
//     *tracker+=i;
//     return tok;
// }

// token *lexer_tokenizer(char *input_str, int* size) {
//     *size = strlen(input_str);
//     token *tokens = malloc(sizeof(token) * (*size));
    
//     token_init(&tokens[0], QUOTE, 10, "Test", -1, -1);
//     printf("%d %s\n",tokens[0].type, tokens[0].str);
//     token_internal_destory(&tokens[0]);

//     for(int i=0; input_str[i]=='\0'; i++){
//         // printf("it: %d\n",i);
//         token* tok;
//         token_type type = char_to_token(input_str[i]);
//         switch(type){
//             case QUOTE: 
//             tok = parse_string(&i,&input_str[i]);
//             // printf("STRING: %s %zu %s |\n",tok->str,tok->mem_size,(char*)tok->str);
//             token_total_destory(tok);
//             break;
            
//             case GENERIC_NUMBER: 
//             tok = parse_number(&i, &input_str[i]);
            
//             // if(tok->type==FLOAT){
//             //     printf("FLOAT: %s %zu %f |\n",tok->str, tok->mem_size, *(float*)tok->data);
//             // }else if(tok->type==INT){
//             //     printf("INT: %s %zu %d |\n", tok->str, tok->mem_size, *(int*)tok->data);
//             // }
//             token_total_destory(tok);
            
//             break;
//             case BOOLEAN: 
//             tok = parse_boolean(&i, &input_str[i]); 
//             // printf("BOOL: %s %zu %d |\n",tok->str, tok->mem_size, *(int*)tok->data);
            
//             token_total_destory(tok);
//             break;
//         }
//     }

//     return tokens;
// }
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

    *tracker+=3;
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