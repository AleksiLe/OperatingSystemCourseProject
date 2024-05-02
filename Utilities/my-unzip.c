/*
Use cases

./my-unzip <file> <file> ... Writes all file decompressions to standard output
./my-unzip <file> <file> ... > <input_file> Writes all file decompressions to file

Additional help from:
Github CoPilot

For testing purposes copy pasta:
valgrind --leak-check=full ./my-unzip test.z

Author:
@AleksiLe
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
    if (argc < 2)
    {
        fprintf(stderr, "my-unzip: file1 [file2 ...]\n");
        exit(0);
    }
    char curChar;
    int count;
    for(int i = 1; i < argc; i++) //Go through all file inputs
    {
        FILE *file = fopen(argv[i], "rb");
        if (file == NULL)
        {
            fprintf(stderr, "my-unzip: cannot open file %s\n", argv[1]);
            exit(1);
        }
        while (fread(&count, sizeof(int), 1, file) == 1) //Read characters from file
        {
            curChar = fgetc(file);
            for (int j = 0; j < count; j++) //Write amount of characters
            {
                putchar(curChar);
            }
        }
        putchar('\n');
        fclose(file);
    }
    
    return 0;
}