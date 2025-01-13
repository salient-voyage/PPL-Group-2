#include <stdio.h>
#include <string.h>

#define NUM_KEYWORDS 20

// List of keywords
const char *keywords[] = {"int", "float", "bool", "str", "char", "if", "else", "return", "while", "elif", "for", "continue", "break", "const", "def", "print", "input", "with", "chance", "value"};

// Function to check if a string is a keyword using a finite automaton-like approach
int isKeyword(const char *str)
{
    for (int k = 0; k < NUM_KEYWORDS; k++)
    {
        int state = 0; // Start in the initial state for each keyword

        // Compare each character of the string with the keyword
        for (int i = 0; str[i] != '\0'; i++)
        {
            char current_char = str[i];
            char keyword_char = keywords[k][state];

            // If the characters match, move to the next state (next character in keyword)
            if (current_char == keyword_char)
            {
                state++;
                if (state == strlen(keywords[k])) // If we've matched the entire keyword, it's an accepting state
                {
                    return 1; // The string is a keyword
                }
            }
            else
            {
                break; // If characters don't match, stop processing this keyword
            }
        }
    }
    return 0; // If no match is found after checking all keywords, return 0 (not a keyword)
}

int main()
{
    const char *test_str = "int";
    if (isKeyword(test_str))
    {
        printf("\"%s\" is a keyword.\n", test_str);
    }
    else
    {
        printf("\"%s\" is not a keyword.\n", test_str);
    }

    return 0;
}
