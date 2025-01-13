#include <stdio.h>
#include "file_b.h"

int main()
{
    FILE *file = fopen("alphabet.txt", "w"); // Open file to write
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    // Start the recursive printing process
    print_letter(file, 'a');

    // Close the file
    fclose(file);

    printf("Alphabet written to alphabet.txt.\n");
    return 0;
}
