#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

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

Token current_token;

void next_token(char *input, int *index)
{
    while (isspace(input[*index]))
        (*index)++; // Skip whitespace

    if (strncmp(&input[*index], "int", 3) == 0)
    {
        current_token.type = T_INT;
        strcpy(current_token.value, "int");
        *index += 3;
    }
    else if (isalpha(input[*index]))
    {
        int start = *index;
        while (isalnum(input[*index]))
            (*index)++;
        int length = *index - start;
        strncpy(current_token.value, &input[start], length);
        current_token.value[length] = '\0';
        current_token.type = T_IDENTIFIER;
    }
    else if (input[*index] == '=')
    {
        current_token.type = T_ASSIGN;
        current_token.value[0] = '=';
        current_token.value[1] = '\0';
        (*index)++;
    }
    else if (isdigit(input[*index]))
    {
        int start = *index;
        while (isdigit(input[*index]))
            (*index)++;
        int length = *index - start;
        strncpy(current_token.value, &input[start], length);
        current_token.value[length] = '\0';
        current_token.type = T_NUMBER;
    }
    else if (input[*index] == ';')
    {
        current_token.type = T_SEMICOLON;
        current_token.value[0] = ';';
        current_token.value[1] = '\0';
        (*index)++;
    }
    else if (input[*index] == '\0')
    {
        current_token.type = T_END;
    }
    else
    {
        current_token.type = T_INVALID;
    }

    // Print statement to show the current token being processed
    printf("Lexer: Token found -> Type: %d, Value: '%s'\n", current_token.type, current_token.value);
}
