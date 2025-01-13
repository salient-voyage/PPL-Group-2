#include <stdio.h>
#include "file_b.h"

void print_letter(FILE *file, char letter)
{
    // Base case: Stop recursion when 'c' is printed
    if (letter > 'c')
    {
        return;
    }

    // Print the current letter to the file
    fprintf(file, "%c\n", letter);

    // Recursively call print_letter for the next letter
    print_letter(file, letter + 1);
}
