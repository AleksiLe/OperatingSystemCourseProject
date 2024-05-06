/*
Use cases

./wish to run shell
./wish <file> to run shell commands from file

Additional help from:
A lot of man pages

For testing purposes copy pasta:
valgrind --leak-check=full ./wish testU.txt

Author:
@AleksiLe
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//Paths global variable
char* paths[64];

//Prints standard error
void printError()
{
    char error_message[30] = "An error has occured\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

//Uses chdir to change working directory
void changeDirectory(char *arguments[])
{
    if(arguments[1] == NULL)
    {
        fprintf(stderr, "changeDirectory: argument NULL");
        printError();
    } else if(chdir(arguments[1]) != 0){
        fprintf(stderr, "changeDirectory: chdir failure");
        printError();
    } 
}

//Resets paths to new inputs
void changePaths(char* arguments[])
{
    for(int i = 1;arguments[i] != NULL; i++)
    {
        paths[i] = strdup(arguments[i]);
    }
}

//Parse line to arguments from input line, separators are " \t\n"
void parseInput(char *input, char *arguments[])
{
    char *token = strtok(input, " \t\n");
    for(int i = 0; token != NULL; i++)
    {
        printf("%s\n", token);
        arguments[i] = token;
        token = strtok(NULL, " \t\n");
    }
    free(token);
}

//New process for command
void executeCommand(char *arguments[])
{
    pid_t id = fork();
    if(id == -1)
    {   
        fprintf(stderr, "executeCommand: pid -1");
        printError();
        return;
    }else if(id == 0)
    {
        execvp(arguments[0], arguments);
        exit(1);
    }
    int status;
    waitpid(id, &status, 0);
}

//Executing commands from different paths
void runCommandFromPath(char *arguments[])
{
    int executed = 0;
    for (int i = 0; paths[i] != NULL; i++)
    {
        if(access(paths[i], X_OK) == 0)
        {
            executeCommand(arguments);
            executed = 1;
            break;
        }
    }
    if (executed == 0)
    {   
        fprintf(stderr, "runCommandFromPath: no command in path");
        printError();
    }
}

//Function to run correct command
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

//Loop for interactive where loop runs until exit
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
    free(line);
}

//Loop for batch operation where loop runs until error or EOF
void runBatch(FILE *file)
{
    char *line = NULL;
    size_t len = 0;
    char *arguments[64];
    while((getline(&line, &len, file)) != -1) 
    {
        parseInput(line, arguments);
        processArguments(arguments);
    } 
    free(line); 
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
        runBatch(file);
    } else {
        // run interactive commands
        runInteractive(file);
    }
    fclose(file);
    return 0;
}