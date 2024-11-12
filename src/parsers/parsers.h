
typedef enum {
    LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET,
    COMMA, COLON, QUOTE, BACKSLASH, NEWLINE, SPACE,
    
    IDENTIFIER,ASSIGNMENT,

    INT,FLOAT,BOOLEAN,STRING, CHAR,
    INVALID,

    FINAL //used to mark end of array
} token_type;

typedef struct token {
    token_type type;
    int line;
    int col;
    char* lexeme;
} token;

// lexer
token *lexer_tokenizer(char *input_str);


// parser
void parser_test();