/*
Different use cases

No inputs: Program reads from standard input stream and reverses the input
One input: Program reads from the first arg file and reverses the content of the file to the terminal
Two inputs: Program reads from the first arg file and writes the reversed content to the second arg file

Additional help from:
GitHub CoPilot
https://stackoverflow.com/questions/3495092/read-from-file-or-stdin
https://solarianprogrammer.com/2019/04/03/c-programming-read-file-lines-fgets-getline-implement-portable-getline/
https://stackoverflow.com/questions/29522264/correct-way-to-use-malloc-and-free-with-linked-lists

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

// Read from input file and append words to linked list
// Return the end of the linked list
// Because of this the linked list is efficient to reverse
struct Node *readToLinkedList(FILE *inputFile)
{
    char *line = NULL;
    size_t len = 0;
    int isStdin = (inputFile == stdin);
    struct Node *head = NULL;

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
        //  printf("%s\n", line); // Print line to terminal (for testing purposes)
        struct Node *newNode = malloc(sizeof(struct Node));
        if (newNode == NULL) // Check if malloc failed
        {
            fprintf(stderr, "malloc failed\n");
            free(newNode);
            exit(1);
        }
        newNode->line = strdup(line);
        if (newNode->line == NULL) // Check if strdup failed
        {
            fprintf(stderr, "malloc(strderr) failed\n");
            free(newNode->line);
            exit(1);
        }
        if (head == NULL)
        {
            newNode->prev = NULL;
            newNode->next = NULL;
            head = newNode;
        } else
        {
            newNode->prev = head;
            newNode->next = NULL;
            head->next = newNode;
            head = newNode;
        }
    }
    free(line); 
    line = NULL; 
    return head;
}

void readFromLinkedListToFile(struct Node *end, FILE *outputFile)
{
    while (end != NULL)
    {
        fprintf(outputFile, "%s\n", end->line);
        end = end->prev;
    }
    return;
}

void readFromLinkedListToTerminal(struct Node *end)
{
    while (end != NULL)
    {
        printf("%s\n", end->line);
        end = end->prev;
    }
    return;
}

void freeLinkedList(struct Node *end)
{
    struct Node *temp;
    while (end != NULL)
    {
        temp = end;
        end = end->prev;
        free(temp->line);
        free(temp);
    }
    return;
}

int main(int argc, char *argv[])
{
    struct Node *end = NULL;
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
        end = readToLinkedList(inputFile);
        fclose(inputFile);

        if (argv[2] != NULL) // Write to output file
        {
            if (strcmp(argv[1], argv[2]) == 0) // Check if input and output file are the same
            {
                fprintf(stderr, "reverse: input and output file must differ\n");
                exit(1);
            }
            FILE *outputFile = fopen(argv[2], "w");
            if (outputFile == NULL) // Check if file exists
            {
                fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
                exit(1);
            }
            readFromLinkedListToFile(end, outputFile);
            fclose(outputFile);
        } else // Print to terminal
        {
            readFromLinkedListToTerminal(end);
        }
    } else if (argc == 1) // Read from standard input and print to terminal
    {
        end = readToLinkedList(stdin);
        readFromLinkedListToTerminal(end);
    }
    freeLinkedList(end);
    return 0;
}