#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_SIZE 1000
#define BUFFER_SIZE 1024

int statement_number = 1;

// Token Categories
typedef enum
{
    KEYWORD,
    RESERVED_WORDS,
    NOISE_WORDS,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    DELIMITER,
    COMMENT,
    WHITESPACE,
    STRING_LITERAL,
    CHARACTER,
    ERROR,
    DATA_TYPE
} TokenType;

// Token structure
typedef struct
{
    TokenType type;
    char value[MAX_TOKEN_SIZE];
    int line_number;
} Token;

int get_token(FILE *input_file, Token *current_token);

// Function to map string to TokenType enum
TokenType get_token_type(const char *type_str)
{
    if (strcmp(type_str, "KEYWORD") == 0)
        return KEYWORD;
    if (strcmp(type_str, "RESERVED_WORDS") == 0)
        return RESERVED_WORDS;
    if (strcmp(type_str, "NOISE_WORDS") == 0)
        return NOISE_WORDS;
    if (strcmp(type_str, "IDENTIFIER") == 0)
        return IDENTIFIER;
    if (strcmp(type_str, "NUMBER") == 0)
        return NUMBER;
    if (strcmp(type_str, "OPERATOR") == 0)
        return OPERATOR;
    if (strcmp(type_str, "DELIMITER") == 0)
        return DELIMITER;
    if (strcmp(type_str, "COMMENT") == 0)
        return COMMENT;
    if (strcmp(type_str, "WHITESPACE") == 0)
        return WHITESPACE;
    if (strcmp(type_str, "STRING_LITERAL") == 0)
        return STRING_LITERAL;
    if (strcmp(type_str, "CHARACTER") == 0)
        return CHARACTER;
    if (strcmp(type_str, "DATA_TYPE") == 0)
        return DATA_TYPE;
    return ERROR;
}

// Function to map TokenType enum to string
const char *get_token_type_string(TokenType type)
{
    switch (type)
    {
    case KEYWORD:
        return "KEYWORD";
    case RESERVED_WORDS:
        return "RESERVED_WORDS";
    case NOISE_WORDS:
        return "NOISE_WORDS";
    case IDENTIFIER:
        return "IDENTIFIER";
    case NUMBER:
        return "NUMBER";
    case OPERATOR:
        return "OPERATOR";
    case DELIMITER:
        return "DELIMITER";
    case COMMENT:
        return "COMMENT";
    case WHITESPACE:
        return "WHITESPACE";
    case STRING_LITERAL:
        return "STRING_LITERAL";
    case CHARACTER:
        return "CHARACTER";
    case DATA_TYPE:
        return "DATA TYPE";
    default:
        return "ERROR";
    }
}

// Get the next token from the input file
int get_token(FILE *input_file, Token *current_token)
{
    char line[BUFFER_SIZE];
    if (fgets(line, sizeof(line), input_file) == NULL)
        return 0;

    char value[MAX_TOKEN_SIZE], type_str[MAX_TOKEN_SIZE];
    int line_number;
    if (sscanf(line, "%s %s %d", value, type_str, &line_number) != 3)
        return 0;

    // Populate the token structure
    strncpy(current_token->value, value, MAX_TOKEN_SIZE);
    current_token->type = get_token_type(type_str);
    current_token->line_number = line_number;
    return 1;
}

// Function to handle data type
void parse_data_type(FILE *input_file, FILE *output_file, Token *current_token)
{
    // Check if the current token is a data type
    if (current_token->type == DATA_TYPE)
    {
        fprintf(output_file, "\tDATA TYPE: ('%s')\n", current_token->value);
        get_token(input_file, current_token); // Get next token
    }
    else
    {
        fprintf(output_file, "Error (Line %d): Expected DATA TYPE for declarative statement\n", current_token->line_number);
    }
}

// Function to handle identifier
void parse_identifier(FILE *input_file, FILE *output_file, Token *current_token)
{
    // Check if the current token is an identifier
    if (current_token->type == IDENTIFIER)
    {
        fprintf(output_file, "\tIDENTIFIER: ('%s')\n", current_token->value);
        get_token(input_file, current_token); // Get next token
    }
    else
    {
        fprintf(output_file, "Error (Line %d): Expected IDENTIFIER after data type\n", current_token->line_number);
    }
}

// Function to handle semicolon
void parse_semicolon(FILE *input_file, FILE *output_file, Token *current_token)
{
    // Check for the SEMICOLON token
    if (current_token->type == DELIMITER && strcmp(current_token->value, ";") == 0)
    {
        fprintf(output_file, "\tSEMICOLON: ('%s')\n", current_token->value);
    }
    else
    {
        fprintf(output_file, "Error (Line %d): Expected SEMICOLON (';') after identifier\n", current_token->line_number);
    }
}

// Function to handle declarative statements
void parse_declarative(FILE *input_file, FILE *output_file, Token *current_token)
{
    // Print the declarative statement with statement number and line number
    fprintf(output_file, "Statement %d (Line %d): Declaration\n", statement_number++, current_token->line_number);

    // Call parse functions
    parse_data_type(input_file, output_file, current_token);
    parse_identifier(input_file, output_file, current_token);
    // Check if there are additional identifiers (separated by commas)
    while (current_token->type == DELIMITER && strcmp(current_token->value, ",") == 0)
    {
        fprintf(output_file, "\tCOMMA: ('%s')\n", current_token->value);
        get_token(input_file, current_token); // Get next token

        // Process the next identifier
        parse_identifier(input_file, output_file, current_token);
    }
    parse_semicolon(input_file, output_file, current_token);

    fprintf(output_file, "\n");
}

// Function to handle assignment statements
void parse_assignment(FILE *input_file, FILE *output_file, Token *current_token)
{
    if (current_token->type == IDENTIFIER)
    {
        // Print the assignment statement with statement number and line number
        fprintf(output_file, "Statement %d (Line %d): Assignment\n", statement_number++, current_token->line_number);
        fprintf(output_file, "\tIDENTIFIER: ('%s')\n", current_token->value);
        get_token(input_file, current_token); // Get next token

        if (current_token->type == OPERATOR && strcmp(current_token->value, "=") == 0)
        {
            fprintf(output_file, "\tASSIGNMENT OPERATOR: ('%s')\n", current_token->value);
            get_token(input_file, current_token); // Get next token

            // Now you would process the right-hand side of the assignment (e.g., a value or expression)
            if (current_token->type == NUMBER || current_token->type == IDENTIFIER)
            {
                fprintf(output_file, "\tASSIGNMENT VALUE: ('%s')\n", current_token->value);

                // Check for the SEMICOLON token
                get_token(input_file, current_token);
                if (current_token->type == DELIMITER && strcmp(current_token->value, ";") == 0)
                {
                    fprintf(output_file, "\tSEMICOLON: ('%s')\n", current_token->value);
                }
                else
                {
                    fprintf(output_file, "Error (Line %d): Expected SEMICOLON (';') after assignment value\n", current_token->line_number);
                }
            }
            else
            {
                fprintf(output_file, "Error (Line %d): Expected value or identifier after '=' in assignment statement\n", current_token->line_number);
            }
        }
        else
        {
            fprintf(output_file, "Error (Line %d): Expected '=' after identifier in assignment statement\n", current_token->line_number);
        }
    }
    else
    {
        fprintf(output_file, "Error (Line %d): Expected identifier for assignment statement\n", current_token->line_number);
    }
    fprintf(output_file, "\n");
}

// Function to determine the type of statement and call appropriate parsing functions
void determine_statement(FILE *input_file, FILE *output_file, Token *current_token)
{
    // Check if it is a declarative statement
    if (current_token->type == DATA_TYPE)
    {
        parse_declarative(input_file, output_file, current_token);
    }
    // Check if it is an assignment statement
    else if (current_token->type == IDENTIFIER)
    {
        parse_assignment(input_file, output_file, current_token);
    }
}

int main()
{
    const char *input_filename = "Symbol Table.txt";
    const char *output_filename = "Parse Tree.txt";

    FILE *input_file = fopen(input_filename, "r");
    if (input_file == NULL)
    {
        printf("Error: Could not open file '%s'\n", input_filename);
        return 1;
    }

    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL)
    {
        printf("Error: Could not open file '%s' for writing\n", output_filename);
        fclose(input_file);
        return 1;
    }

    Token current_token;

    while (get_token(input_file, &current_token))
    {
        // Use the determine_statement function to process the token
        determine_statement(input_file, output_file, &current_token);
    }

    fclose(input_file);
    fclose(output_file);

    printf("Parsing completed, results written to '%s'\n", output_filename);
    return 0;
}
