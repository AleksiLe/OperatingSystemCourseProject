/*

*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printError()
{
    char error_message[30] = "An error has occured\n";
    write(stderr, error_message, strlen(error_message));
}

void parseInput(char *input, char *arguments[])
{
    char *token = strtok(input, " \t\n");
    for(int i = 0; token != NULL; i++)
    {
        arguments[i] = token;
        token = strtok(NULL, " \t\n");
    }
}

void processArguments(char *arguments[])
{
    if(strcmp(arguments[0], "exit") == 0)
    {
        exit(0);
    }
}

void runInteractive(FILE *file)
{
    char *line = NULL;
    size_t len = 0;
    char *arguments[64];
    while(1) 
    {
        printf("wish >");
        getline(&line, &len, file);
        parseInput(line, arguments);
        processArguments(arguments);
    }
}


int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        fprintf(stderr, "Usage: %s [batch]", argv[0]);
        exit(1);
    }

    FILE *file = stdin;
    if(argc == 2) //Batch file was given
    {
        file = fopen(argv[1], "r");
        if(file == NULL)
        {
            printError();
            exit(1);
        }
        // run commands
        fclose(file);
    } 
    // run interactive commands
    runInteractive(file);
}