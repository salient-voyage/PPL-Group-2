#include <stdio.h>
#include <stdlib.h>
#include "lexerheader.h"

// Function declarations for parsing individual components of a declaration
void parse_declaration(char *input, int *index);
void parse_type(char *input, int *index);
void parse_identifier(char *input, int *index);
void parse_assignment(char *input, int *index);
void parse_number(char *input, int *index);
void parse_semicolon(char *input, int *index);

// Parse the full declaration: int <identifier> = <number>;
void parse_declaration(char *input, int *index)
{
    // Parse the type (e.g., "int")
    parse_type(input, index);

    // Parse the identifier (e.g., "age")
    parse_identifier(input, index);

    // Parse the assignment operator (e.g., "=")
    parse_assignment(input, index);

    // Parse the number (e.g., "12")
    parse_number(input, index);

    // Parse the semicolon at the end (e.g., ";")
    parse_semicolon(input, index);
}

// Parse the type part of the declaration (e.g., "int")
void parse_type(char *input, int *index)
{
    // Check if the current token is a valid type (e.g., T_INT)
    if (current_token.type == T_INT)
    {
        printf("Parser: Type token ('%s') processed.\n", current_token.value);
        next_token(input, index); // Get the next token from the lexer
    }
    else
    {
        // Print error if the expected type is not found
        printf("Error: Expected 'int', got '%s'\n", current_token.value);
        exit(1);
    }
}

// Parse the identifier (e.g., "age")
void parse_identifier(char *input, int *index)
{
    // Check if the current token is a valid identifier
    if (current_token.type == T_IDENTIFIER)
    {
        printf("Parser: Identifier token ('%s') processed.\n", current_token.value);
        next_token(input, index); // Get the next token from the lexer
    }
    else
    {
        // Print error if the expected identifier is not found
        printf("Error: Expected identifier, got '%s'\n", current_token.value);
        exit(1);
    }
}

// Parse the assignment operator (e.g., "=")
void parse_assignment(char *input, int *index)
{
    // Check if the current token is an assignment operator
    if (current_token.type == T_ASSIGN)
    {
        printf("Parser: Assignment token ('%s') processed.\n", current_token.value);
        next_token(input, index); // Get the next token from the lexer
    }
    else
    {
        // Print error if the expected assignment operator is not found
        printf("Error: Expected '=', got '%s'\n", current_token.value);
        exit(1);
    }
}

// Parse the number (e.g., "12")
void parse_number(char *input, int *index)
{
    // Check if the current token is a valid number
    if (current_token.type == T_NUMBER)
    {
        printf("Parser: Number token ('%s') processed.\n", current_token.value);
        next_token(input, index); // Get the next token from the lexer
    }
    else
    {
        // Print error if the expected number is not found
        printf("Error: Expected number, got '%s'\n", current_token.value);
        exit(1);
    }
}

// Parse the semicolon (e.g., ";")
void parse_semicolon(char *input, int *index)
{
    // Check if the current token is a semicolon
    if (current_token.type == T_SEMICOLON)
    {
        printf("Parser: Semicolon token ('%s') processed.\n", current_token.value);
        next_token(input, index); // Get the next token from the lexer
    }
    else
    {
        // Print error if the expected semicolon is not found
        printf("Error: Expected ';', got '%s'\n", current_token.value);
        exit(1);
    }
}

int main()
{
    // The input string to be lexed and parsed
    char input[] = "int age = 13;"; // This will cause an error since "jeff" is not a number
    int index = 0;                  // Index to track position in the input string

    // Start lexing to get the first token
    printf("Lexer: Starting tokenization process.\n");
    next_token(input, &index);

    // Start parsing the declaration
    printf("Parser: Starting parsing process.\n");
    parse_declaration(input, &index);

    // Check if there are any unexpected tokens remaining after parsing
    if (current_token.type != T_END)
    {
        printf("Error: Unexpected token '%s'\n", current_token.value);
        return 1;
    }

    // Print success message if parsing completed without errors
    printf("Parser: Declaration parsed successfully: %s\n", input);
    return 0;
}

// Compile command: gcc samplelexer.c sampleparser.c -o parser
// run executable: ./parser
