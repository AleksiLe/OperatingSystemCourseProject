/*
Use case

./my-cat <file> <file> ...

Returns all lines that are in files.

Additional help from:
Github CoPilot

For testing purposes copy pasta:
valgrind --leak-check=full ./my-cat test.txt

Author:
@AleksiLe
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "my-cat: no files provided\n");
        exit(0);
    }
    for (int i = 1; i < argc; i++)
    {
        FILE *file = fopen(argv[i], "r");
        if (file == NULL)
        {
            fprintf(stderr, "my-cat: cannot open file %s\n", argv[i]);
            exit(1);
        }
        char *line = NULL;
        size_t len = 0;
        while ((getline(&line, &len, file)) != -1) 
        {
            if (line[strlen(line) - 1] == '\n') // Remove newline character
            {
                line[strlen(line) - 1] = '\0';
            }
            printf("%s\n", line);
        }
        free(line);
        fclose(file);
    }
    return 0;
}