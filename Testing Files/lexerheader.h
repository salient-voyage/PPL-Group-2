#ifndef LEXER_H
#define LEXER_H

typedef enum
{
    T_INT,
    T_IDENTIFIER,
    T_ASSIGN,
    T_NUMBER,
    T_SEMICOLON,
    T_END,
    T_INVALID
} TokenType;

typedef struct
{
    TokenType type;
    char value[50];
} Token;

extern Token current_token;

void next_token(char *input, int *index);

#endif // LEXER_H
