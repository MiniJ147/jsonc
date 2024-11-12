#include "parsers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    }


    if(value >= 'a' && value <= 'z'){
        return CHAR;
    }

    return INVALID; 
}

// pass at the qoute mark
token parse_string(int* tracker, char *str) {
    int flag_handle_escape_char = 0;
    
    int i;
    token tok;
    char *test = malloc(100);
    char *tp = test;

    for(i=1; str[i]!='\0';i++){
        token_type type = char_to_token(str[i]);
        token_type prev_type = char_to_token(str[i-1]);

        if(type==QUOTE && prev_type!=BACKSLASH){
            break;
        }

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


    // strncpy(test,(str+1),i-1);

    printf("%s string found %d long\n",test, i);
    free(test);
    *tracker+=i;
}

token *lexer_tokenizer(char *input_str) {
    token *tokens = malloc(sizeof(token) * strlen(input_str));


    for(int i=0; input_str[i]!='\0'; i++){
        printf("it: %d\n",i);
        token_type type = char_to_token(input_str[i]);
        if(type==QUOTE){
            parse_string(&i,&input_str[i]);
        }
    }

    return tokens;
}
