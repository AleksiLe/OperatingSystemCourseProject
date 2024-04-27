/*
Different use cases

No inputs: Program reads from standard input stream and reverses the input
One input: Program reads from the first arg file and reverses the content of the file to the terminal
Two inputs: Program reads from the first arg file and writes the reversed content to the second arg file

Additional help from:
GitHub CoPilot
https://stackoverflow.com/questions/3495092/read-from-file-or-stdin
https://solarianprogrammer.com/2019/04/03/c-programming-read-file-lines-fgets-getline-implement-portable-getline/

For testing purposes copy pasta:
valgrind --leak-check=full ./reverse input.txt

Author:
@AleksiLe
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node
{
    char *line;
    struct Node *next;
    struct Node *prev;
};

struct Node *readToLinkedList(FILE *inputFile)
{
    char *line = NULL;
    size_t len = 0;
    int isStdin = (inputFile == stdin);
    struct Node *head = NULL;
    struct Node *end = NULL;

    while ((getline(&line, &len, inputFile)) != -1)
    {   
        if (isStdin && strcmp(line, "quit\n") == 0) // Break loop if quit is entered
        {
            break;
        }
        if (line[strlen(line) - 1] == '\n') // Remove newline character
        {
            line[strlen(line) - 1] = '\0';
        }
        printf("%s\n", line); // Print line to terminal (for testing purposes)
        //  struct Node *newNode = malloc(sizeof(struct Node));
    }
    free(line); 
    line = NULL; 
    return end;
}

int main(int argc, char *argv[])
{
    if (argc > 3) // Check correct amount of arguments
    {
        fprintf(stderr, "Usage: reverse <input> <output>\n");
        exit(1);
    } else if (argc > 1) // Input file and output file or print to terminal
    {
        FILE *inputFile = fopen(argv[1], "r");
        if (inputFile == NULL) // Check if file exists
        {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }

        // Read from input file and append words to linked list
        struct Node *end = readToLinkedList(inputFile);
        fclose(inputFile);
    } else if (argc == 1) // Read from standard input and print to terminal
    {
        struct Node *end = readToLinkedList(stdin);
    }
    return 0;
}