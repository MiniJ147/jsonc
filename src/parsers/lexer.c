#include "parsers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

token_type lexer_char_to_token(char value){
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

token *lexer_tokenizer(char *input_str) {
    token *tokens = malloc(sizeof(token) * strlen(input_str)); // rough size of what we will need
    int row = 0; int col = 0;
    int token_index = 0;

    for(int i=0; input_str[i]!='\0'; i++){
        char curr_char = input_str[i];
        token *curr_token = &tokens[token_index];

        token_type type = lexer_char_to_token(curr_char);
        if(type==NEWLINE){
            col = 0;
            row++;
        }

        printf("token: %c | token_id: %d | col: %d | row: %d\n",curr_char,type,col,row);
        col++;
    }

    return tokens;
}
