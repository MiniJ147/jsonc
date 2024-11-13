#include "parsers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void token_print(token tok){
    printf("Token ID: %d\nLine: %d\nCol: %d\nStr: %s\n",tok.type,tok.line,tok.col,tok.str);
}

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
token parse_string(int* tracker, char *str) {
    int i; int size = 0;
    token tok;

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

    // copying values
    tok.type = STRING;
    tok.col = -1;
    tok.line = -1;
    tok.str = new_str;

    // printf("%s string found %d long\n",new_str, size);

    *tracker+=i; // adjust our postion in file
    
    return tok;
}

// TODO: boolean, int, float

// on begining of boolean
token parse_boolean(int *tracker, char *input_str){
    token tok = {"",BOOLEAN,-1,-1};

    if(strncmp(input_str,"true",4)==0){
        tok.str = strdup("true");
    }else if(strncmp(input_str,"false",4)==0){
        tok.str = strdup("false");
    }else{
        tok.type = INVALID;
        printf("invalid\n");
    }

    *tracker+=4;
    return tok;
}

// on begining of number
token parse_number(int *tracker, char *input_str){
    token tok;
    int i;
    int is_float = 0;

    // must be 0-9 or a (.) or (-)
    for(i=0; input_str[i]>='0' && input_str[i]<='9' || input_str[i]=='.' || input_str[i]=='-'; i++){
        if(input_str[i]=='.'){
            is_float = 1;
        }
    }

    //marking end of string for ato_ functions
    char tmp = input_str[i];
    input_str[i] = '\0';
    
    if(is_float){
        tok.type = FLOAT;
        // printf("%f\n",atof(input_str));
    }else{
        tok.type = INT;
        // printf("%d\n", atoi(input_str));
    }

    tok.col = -1;
    tok.line = -1;
    tok.str = strdup(input_str);

    // replacing orginal value back
    input_str[i] = tmp;
    
    *tracker+=i;
    return tok;
}

token *lexer_tokenizer(char *input_str) {
    token *tokens = malloc(sizeof(token) * strlen(input_str));


    for(int i=0; input_str[i]!='\0'; i++){
        // printf("it: %d\n",i);
        token_type type = char_to_token(input_str[i]);
        switch(type){
            case QUOTE: parse_string(&i,&input_str[i]); break;
            case GENERIC_NUMBER: token_print(parse_number(&i, &input_str[i])); break;
            case BOOLEAN: parse_boolean(&i, &input_str[i]); break;
        }
    }

    return tokens;
}
