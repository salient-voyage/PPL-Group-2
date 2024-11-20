#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *file;
    char *filename = "FateScript Files/sample.fate"; // Path to the file
    char ch;

    // Open the file in read mode
    file = fopen(filename, "r");

    if (file == NULL)
    {
        // Error handling if the file cannot be opened
        perror("Error opening file");
        return 1;
    }

    // Read and print the content of the file character by character
    while ((ch = fgetc(file)) != EOF)
    {
        putchar(ch);
    }

    // Close the file after reading
    fclose(file);

    return 0;
}
