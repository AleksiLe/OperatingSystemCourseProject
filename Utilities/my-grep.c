/*
Use cases

./my-grep <searchterm> <file> <file> ...

Returns all lines that have searchterm as a substring

Additional help from:
Github CoPilot
https://stackoverflow.com/questions/12784766/check-substring-exists-in-a-string-in-c

For testing purposes copy pasta:
valgrind --leak-check=full ./my-grep test.txt

Author:
@AleksiLe
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printToUser(FILE *file, char *substring)
{
    char *line = NULL;
    size_t len = 0;
    int isStdin = (file == stdin);
    while ((getline(&line, &len, file)) != -1) 
    {
        if (isStdin && strcmp(line, "quit\n") == 0) // Break loop if quit is entered
        {
            break;
        }
        if (line[strlen(line) - 1] == '\n') // Remove newline character
        {
            line[strlen(line) - 1] = '\0';
        }
        if(strstr(line, substring))
        {
            printf("%s\n", line);
        }
    }
    free(line);
}

int main(int argc, char *argv[])
{
    if(argc < 1)
    {
        fprintf(stderr, "my-grep: searchterm [file ...]\n");
        exit(1);
    } else if (argc == 2)
    {
        printToUser(stdin, argv[1]);
    } else {
        for (int i = 2; i < argc; i++)
        {
            FILE *file = fopen(argv[i], "r");
            if (file == NULL)
            {
                fprintf(stderr, "my-cat: cannot open file %s\n", argv[i]);
                exit(1);
            }
            printToUser(file, argv[1]);
            fclose(file);
        }
    }
    return 0;
}