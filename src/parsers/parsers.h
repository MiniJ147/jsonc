
#include <stdlib.h>

// SECTION: TOKENS

typedef enum {
    NULL_TOK, // DO NOT MOVE NULL_TOK IT SHOULD BE 0 
    LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET,
    COMMA, COLON, QUOTE, BACKSLASH, NEWLINE, SPACE, TAB,
    
    IDENTIFIER,ASSIGNMENT, GENERIC_NUMBER, 
    

    INT,FLOAT,BOOLEAN,STRING,CHAR,
    INVALID,

    FINAL //used to mark end of array
} token_type;

// m -> signifies that it is malloc and should be freed
typedef struct token{
    token_type type;
    int line;
    int col;
    size_t mem_size;
    char* m_str;
    void* m_data;
} token;

token* token_init_arr(int n);

// mem_size <= 0 will not malloc into m_data
// raw_str will be strdup NOT stored as pointer, enter NULL if no string is wished to be allocated
void token_init(token* token, token_type type, size_t mem_size, char* raw_str, int line, int col);

void token_internal_destory(token* token); // frees internal data of struct, but not the pointer itself

void token_strdup_str(token* token, char* str);

// writes data into m_data slot, uses memcpy and does NOT free void*
void token_write_data(token* token, void* data, size_t size);

// frees all tokens in given token array
// also frees token_arr ptr itself
void token_free_arr(token* token_arr, int size);


// SECTION: lexer

// takes json_str and size_cnt as agurments. Size_cnt will write the total size of the array
// returns an array of tokens
token* lexer_tokenize(char* json_str, int* size_cnt);

// typedef struct token {
//     token_type type;
//     int line;
//     int col;
//     size_t mem_size;
//     char* str;
//     void* data;
// } token;


// struct token_v2;
// struct token_v2* token_init_array(int n);
// void token_init(struct token_v2* token);

// // lexer

// // token *lexer_free(token* tokens, int size);
// token *lexer_tokenizer(char *input_str, int* size);

// // parser
// void parser_test();