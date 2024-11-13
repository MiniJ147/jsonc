
#include <stdlib.h>

typedef enum {
    NULL_TOK, // DO NOT MOVE NULL_TOK IT SHOULD BE 0 
    LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET,
    COMMA, COLON, QUOTE, BACKSLASH, NEWLINE, SPACE,
    
    IDENTIFIER,ASSIGNMENT, GENERIC_NUMBER, 
    

    INT,FLOAT,BOOLEAN,STRING,CHAR,
    INVALID,

    FINAL //used to mark end of array
} token_type;

typedef struct token {
    token_type type;
    int line;
    int col;
    size_t mem_size;
    char* str;
    void* data;
} token;

// lexer

// token *lexer_free(token* tokens, int size);
token *lexer_tokenizer(char *input_str, int* size);

// parser
void parser_test();