#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_SIZE 100

// Define possible token types
typedef enum
{
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    PUNCTUATION,
    ERROR
} TokenType;

// Keywords list
const char *keywords[] = {"int", "float", "if", "else", "return", "while", "Yoj"};
int isKeyword(const char *str)
{
    for (int i = 0; i < 7; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

// Token structure
typedef struct
{
    TokenType type;
    char value[MAX_TOKEN_SIZE];
} Token;

// Function to classify and print token
void printToken(Token token)
{
    switch (token.type)
    {
    case KEYWORD:
        printf("Keyword: %s\n", token.value);
        break;
    case IDENTIFIER:
        printf("Identifier: %s\n", token.value);
        break;
    case NUMBER:
        printf("Number: %s\n", token.value);
        break;
    case OPERATOR:
        printf("Operator: %s\n", token.value);
        break;
    case PUNCTUATION:
        printf("Punctuation: %s\n", token.value);
        break;
    default:
        printf("Error: Unknown token %s\n", token.value);
    }
}

// Lexical analyzer function
void lexicalAnalyzer(const char *input)
{
    int i = 0, j = 0;
    char currentChar;
    Token currentToken;

    while ((currentChar = input[i]) != '\0')
    {
        // Skip whitespace
        if (isspace(currentChar))
        {
            i++;
            continue;
        }

        // Handle identifiers and keywords
        if (isalpha(currentChar))
        {
            j = 0;
            while (isalnum(input[i]) || input[i] == '_')
            {
                currentToken.value[j++] = input[i++];
            }
            currentToken.value[j] = '\0';
            currentToken.type = isKeyword(currentToken.value) ? KEYWORD : IDENTIFIER;
            printToken(currentToken);
        }
        // Handle numbers
        else if (isdigit(currentChar))
        {
            j = 0;
            while (isdigit(input[i]))
            {
                currentToken.value[j++] = input[i++];
            }
            currentToken.value[j] = '\0';
            currentToken.type = NUMBER;
            printToken(currentToken);
        }
        // Handle operators
        else if (strchr("+-*/=<>", currentChar))
        {
            currentToken.value[0] = currentChar;
            currentToken.value[1] = '\0';
            currentToken.type = OPERATOR;
            printToken(currentToken);
            i++;
        }
        // Handle punctuation
        else if (strchr(";,(){}", currentChar))
        {
            currentToken.value[0] = currentChar;
            currentToken.value[1] = '\0';
            currentToken.type = PUNCTUATION;
            printToken(currentToken);
            i++;
        }
        // Handle unknown characters
        else
        {
            currentToken.value[0] = currentChar;
            currentToken.value[1] = '\0';
            currentToken.type = ERROR;
            printToken(currentToken);
            i++;
        }
    }
}

int main()
{
    char input[1000];

    // Ask the user for input
    printf("Enter a line of code for lexical analysis:\n");
    fgets(input, sizeof(input), stdin); // Read user input

    printf("\nTokens:\n");
    lexicalAnalyzer(input);
    return 0;
}
