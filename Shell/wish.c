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

void runInteractive(FILE *file)
{
    char *line = NULL;
    size_t len = 0;
    while(1) 
    {
        printf("wish >");
        getline(&line, &len, file);
        printf("%s\n", line);
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