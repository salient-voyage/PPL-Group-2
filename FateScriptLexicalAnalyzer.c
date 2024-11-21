#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_SIZE 100

// Define possible token types
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
    ERROR
} TokenType;

// Keywords list
const char *keywords[] = {"int", "float", "bool", "str", "char", "if", "else", "return", "while", "elif", "for", "continue", "break", "const", "def", "true", "false", "print", "input", "with", "chance", "value"};
int isKeyword(const char *str)
{
    for (int i = 0; i < 21; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

// Reserved words list
const char *reservedWords[] = {"pipeline", "using", "extension", "trait", "assert", "module", "delegate"};
int isReservedWord(const char *str)
{
    for (int i = 0; i < 7; i++)
    {
        if (strcmp(str, reservedWords[i]) == 0)
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
void printToken(Token token, FILE *file)
{
    // Print to console
    switch (token.type)
    {
    case KEYWORD:
        printf("Keyword: %s\n", token.value);
        break;
    case RESERVED_WORDS:
        printf("Reserved Word: %s\n", token.value);
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
    case DELIMITER:
        printf("Delimiter: %s\n", token.value);
        break;
    case COMMENT:
        printf("Comment: %s\n", token.value);
        break;
    default:
        printf("Error: Unknown token %s\n", token.value);
    }

    // Write to the file
    switch (token.type)
    {
    case KEYWORD:
        fprintf(file, "Keyword: %s\n", token.value);
        break;
    case RESERVED_WORDS:
        fprintf(file, "Reserved Word: %s\n", token.value);
        break;
    case IDENTIFIER:
        fprintf(file, "Identifier: %s\n", token.value);
        break;
    case NUMBER:
        fprintf(file, "Number: %s\n", token.value);
        break;
    case OPERATOR:
        fprintf(file, "Operator: %s\n", token.value);
        break;
    case DELIMITER:
        fprintf(file, "Delimiter: %s\n", token.value);
        break;
    case COMMENT:
        fprintf(file, "Comment: %s\n", token.value);
        break;
    default:
        fprintf(file, "Error: Unknown token %s\n", token.value);
    }
}

// Lexical analyzer function
void lexicalAnalyzer(const char *input, FILE *file)
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

        // Handle single-line comments (starting with #)
        if (currentChar == '#')
        {
            currentToken.type = COMMENT;
            j = 0;
            currentToken.value[j++] = currentChar; // Store the '#' character

            // Collect all characters until the end of the line
            i++;
            while (input[i] != '\n' && input[i] != '\0')
            {
                currentToken.value[j++] = input[i++];
            }
            currentToken.value[j] = '\0';
            printToken(currentToken, file);
        }
        // Handle multi-line comments (starting with """ and ending with """)
        else if (input[i] == '"' && input[i + 1] == '"' && input[i + 2] == '"')
        {
            currentToken.type = COMMENT;
            j = 0;
            currentToken.value[j++] = input[i++];
            currentToken.value[j++] = input[i++];
            currentToken.value[j++] = input[i++];

            // Collect all characters until the closing triple quotes
            while (!(input[i] == '"' && input[i + 1] == '"' && input[i + 2] == '"') && input[i] != '\0')
            {
                currentToken.value[j++] = input[i++];
            }

            // Add closing triple quotes if found
            if (input[i] == '"' && input[i + 1] == '"' && input[i + 2] == '"')
            {
                currentToken.value[j++] = input[i++];
                currentToken.value[j++] = input[i++];
                currentToken.value[j++] = input[i++];
            }

            currentToken.value[j] = '\0';
            printToken(currentToken, file);
        }
        // Handle identifiers, reserved words, and keywords
        else if (isalpha(currentChar))
        {
            j = 0;
            while (isalnum(input[i]) || input[i] == '_')
            {
                currentToken.value[j++] = input[i++];
            }
            currentToken.value[j] = '\0';

            if (isReservedWord(currentToken.value))
            {
                currentToken.type = RESERVED_WORDS;
            }
            else if (isKeyword(currentToken.value))
            {
                currentToken.type = KEYWORD;
            }
            else
            {
                currentToken.type = IDENTIFIER;
            }
            printToken(currentToken, file);
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
            printToken(currentToken, file);
        }

        // Handle Arithmetic, Relational, Logical, Assignment, and Unary Operators
        else if (input[i] == '~' && input[i + 1] == '=') // Special handling for ~= operator (integer division assignment)
        {
            // Handle the ~= operator (integer division assignment)
            currentToken.value[0] = input[i];
            currentToken.value[1] = input[i + 1];
            currentToken.value[2] = '\0'; // Null-terminate
            currentToken.type = OPERATOR; // Classify as a special arithmetic operator
            printToken(currentToken, file);
            i += 2; // Advance past both characters
        }
        else if ((input[i] == '+' && input[i + 1] == '+') ||   // Unary increment
                 (input[i] == '-' && input[i + 1] == '-') ||   // Unary decrement
                 (input[i] == '+' && isdigit(input[i + 1])) || // Unary plus (e.g. +3)
                 (input[i] == '-' && isdigit(input[i + 1])))   // Unary minus (e.g. -3)
        {
            // Handle unary operators (+<value>, -<value>, ++<value>, --<value>)
            currentToken.value[0] = input[i];
            currentToken.value[1] = input[i + 1];
            currentToken.value[2] = '\0'; // Null-terminate
            currentToken.type = OPERATOR; // Classify as a unary operator
            printToken(currentToken, file);
            i += 2; // Advance past both characters
        }
        else if ((input[i] == '=' && input[i + 1] == '=')) // Relational operator (==)
        {
            // Handle relational equality (==)
            currentToken.value[0] = input[i];
            currentToken.value[1] = input[i + 1];
            currentToken.value[2] = '\0'; // Null-terminate
            currentToken.type = OPERATOR; // Classify as a relational operator
            printToken(currentToken, file);
            i += 2; // Advance past both characters
        }
        else if (input[i] == '=') // Assignment operator (=)
        {
            // Handle assignment operator (=)
            currentToken.value[0] = input[i];
            currentToken.value[1] = '\0'; // Null-terminate
            currentToken.type = OPERATOR; // Classify as an assignment operator
            printToken(currentToken, file);
            i++; // Advance past the single '='
        }
        else if ((input[i] == '+' && input[i + 1] == '=') || // Assignment (+=)
                 (input[i] == '-' && input[i + 1] == '=') || // Assignment (-=)
                 (input[i] == '*' && input[i + 1] == '=') || // Assignment (*=)
                 (input[i] == '/' && input[i + 1] == '=') || // Assignment (/=)
                 (input[i] == '%' && input[i + 1] == '=') || // Assignment (%=)
                 (input[i] == '^' && input[i + 1] == '='))   // Assignment (^=)
        {
            // Handle other assignment operators (+=, -=, *=, /=, %=, ^=)
            currentToken.value[0] = input[i];
            currentToken.value[1] = input[i + 1];
            currentToken.value[2] = '\0'; // Null-terminate
            currentToken.type = OPERATOR; // Classify as an assignment operator
            printToken(currentToken, file);
            i += 2; // Advance past both characters
        }
        else if ((input[i] == '>' && input[i + 1] == '=') || // Greater than or equal (>=)
                 (input[i] == '<' && input[i + 1] == '=') || // Less than or equal (<=)
                 (input[i] == '!' && input[i + 1] == '=') || // Not equal (!=)
                 (input[i] == '=' && input[i + 1] == '='))   // Equal to (==)
        {
            // Handle two-character relational operators (>=, <=, !=, ==)
            currentToken.value[0] = input[i];
            currentToken.value[1] = input[i + 1];
            currentToken.value[2] = '\0'; // Null-terminate the token string
            currentToken.type = OPERATOR; // Classify as a relational operator
            printToken(currentToken, file);
            i += 2; // Advance past the two characters
        }
        else if (input[i] == '>' || input[i] == '<') // Handle single-character relational operators (> or <)
        {
            currentToken.value[0] = input[i];
            currentToken.value[1] = '\0'; // Null-terminate the token string
            currentToken.type = OPERATOR; // Classify as a relational operator
            printToken(currentToken, file);
            i++; // Advance past the single character
        }
        else if ((input[i] == '&' && input[i + 1] == '&') || // Logical AND (&&)
                 (input[i] == '|' && input[i + 1] == '|'))   // Logical OR (||)
        {
            // Handle logical operators (&&, ||)
            currentToken.value[0] = input[i];
            currentToken.value[1] = input[i + 1];
            currentToken.value[2] = '\0'; // Null-terminate
            currentToken.type = OPERATOR; // Classify as a logical operator
            printToken(currentToken, file);
            i += 2; // Advance past both characters
        }

        // Handle Logical NOT Operator (!)
        else if (input[i] == '!')
        {
            // Handle logical NOT operator (!)
            currentToken.value[0] = input[i];
            currentToken.value[1] = '\0'; // Null-terminate
            currentToken.type = OPERATOR; // Classify as a logical operator
            printToken(currentToken, file);
            i++; // Advance past the '!'
        }

        // Handle arithmetic operators (+, -, *, /, ~, %, ^)
        else if ((input[i] == '+') || // Arithmetic addition (+)
                 (input[i] == '-') || // Arithmetic subtraction (-)
                 (input[i] == '*') || // Arithmetic multiplication (*)
                 (input[i] == '/') || // Arithmetic division (/)
                 (input[i] == '~') || // Arithmetic integer division (~)
                 (input[i] == '%') || // Arithmetic modulus (%)
                 (input[i] == '^'))   // Arithmetic exponentiation (^)
        {
            // Handle arithmetic operators (+, -, *, /, ~, %, ^)
            currentToken.value[0] = input[i];
            currentToken.value[1] = '\0'; // Null-terminate
            currentToken.type = OPERATOR; // Classify as an arithmetic operator
            printToken(currentToken, file);
            i++; // Advance past the current operator
        }
        // Handle delimiters
        else if (strchr(";,(){}[].'\"'", currentChar))
        {
            currentToken.value[0] = currentChar;
            currentToken.value[1] = '\0';
            currentToken.type = DELIMITER;
            printToken(currentToken, file);
            i++;
        }
        // Handle unknown characters
        else
        {
            currentToken.value[0] = currentChar;
            currentToken.value[1] = '\0';
            currentToken.type = ERROR;
            printToken(currentToken, file);
            i++;
        }
    }
}

// Check if the file is a .fate file
int isFateFile(const char *filename)
{
    // Check the file extension
    const char *extension = strrchr(filename, '.');
    return (extension != NULL && strcmp(extension, ".fate") == 0);
}

int main()
{
    FILE *file;
    char *filename = "FateScript Files/sample.fate"; // Path to the file
    char input[1000];
    int i = 0;

    // Check if the file has the .fate extension
    if (!isFateFile(filename))
    {
        printf("Error: The file is not a FateScript file.\n");
        return 1;
    }

    // Open the file in write mode for the symbol table
    file = fopen("Symbol Table.txt", "w"); // Open for writing
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    // Open the source .fate file
    FILE *sourceFile = fopen(filename, "r");
    if (sourceFile == NULL)
    {
        perror("Error opening source file");
        return 1;
    }

    // Read the content of the file into input buffer
    i = 0;
    while ((input[i] = fgetc(sourceFile)) != EOF && i < sizeof(input) - 1)
    {
        i++;
    }
    input[i] = '\0'; // Null-terminate the string

    // Close the source file after reading
    fclose(sourceFile);

    printf("\nTokens from file '%s':\n", filename);
    lexicalAnalyzer(input, file); // Perform lexical analysis and write to the file

    // Close the output file after writing
    fclose(file);

    printf("\nLexical analysis is complete. Output written to 'Symbol Table.txt'.\n");

    return 0;
}
