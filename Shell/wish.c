/*

*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//Paths global variable
char* paths[64];

void printError()
{
    char error_message[30] = "An error has occured\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

void changeDirectory(char *arguments[])
{
    if(arguments[1] == NULL)
    {
        printError();
    } else if(chdir(arguments[1]) != 0){
        printError();
    } 
}

void changePaths(char* arguments[])
{
    for(int i = 0;arguments[i] != NULL; i++)
    {
        paths[i] = strdup(arguments[i]);
    }
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

void executeCommand(char *arguments[])
{
    pid_t id = fork();
    if(id == -1)
    {
        printError();
        return;
    }else if(id == 0)
    {
        execvp(arguments[0], arguments);
        printError();
        exit(1);
    }
    int status;
    waitpid(id, &status, 0);
}

void runCommandFromPath(char *arguments[])
{
    int executed = 0;
    for (int i = 0; paths[i] != NULL; i++)
    {
        if(access(paths[i], X_OK) == 0)
        {
            //printf("%s %s\n", paths[i], arguments[0]);
            executeCommand(arguments);
            executed = 1;
            break;
        }
    }
    if (executed == 0)
    {
        printError();
    }
}

void processArguments(char *arguments[])
{
    if(strcmp(arguments[0], "exit") == 0)
    {
        exit(0);
    } else if(strcmp(arguments[0], "path") == 0) 
    {
        changePaths(arguments);
    } else if(strcmp(arguments[0], "cd") == 0)
    {
        changeDirectory(arguments);
    } else
    {
        runCommandFromPath(arguments);
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

    //Initalize standard path
    paths[0] = "/bin";

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