#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>

// to do - fix null command - crash
//  redirection
//  multi commands
// tee

#define MAX_PATHS 100
// flag for exit
bool flagExit = false;

// count the number of arguments
int ArgCount;

// mutiple command argument array
char *command_arr[100];

// path array
const char *defaultPath1 = "/bin/";
char *paths[MAX_PATHS];
int pathCount = 0;
bool pathSet = false;

int main(int MainArgc, char *MainArgv[])
{
    if (MainArgc > 2)
    {
        printf("Usage: %s [batch_file]\n", MainArgv[0]);
        exit(1);
    }

    // add the default paths to the path array
    paths[0] = strdup(defaultPath1);
    pathCount = 1;
    pathSet = true;

    // interactive mode = no args
    if (MainArgc == 1)
    {
        interactive_mode();
    }
    else // batch mode = 1 arg (the batch file we want to exec)
    {
        batch_mode();
    }

    return (0);
}

void cmdExit()
{
    flagExit = true;
    // printf("Exiting\n");
    exit(0);
}

void cmdPath(char *command[])
{
    // if no arguements are passed, set path to null and no programs can be executed
    if (ArgCount == 1)
    {
        pathSet = false; // now no programs can be executed- throw error if program is executed
        pathCount = 0;
        return;
    }
    else
    {
        // overwrite the path array with the new paths
        pathCount = 0;
        for (int i = 1; i < ArgCount; i++)
        {
            // this is not seg fault
            paths[pathCount] = strdup(command[i]);
            pathCount++;
            pathSet = true;
        }
    }
}

void cmdCD(char *command[])
{
    // printf("Arrived in CD\n");
    if (ArgCount == 0 || ArgCount > 2)
    {
        printf("Invalid number of arguments\n");
        printf("%d\n", ArgCount);
        return;
    }
    else if (ArgCount == 2)
    {
        chdir(command_arr[1]);
    }
}

void programsExec(char *command[])
{
    bool AccessProg = false;

    if (pathSet)
    {
        // Iterate through the path list to check if the program is accessible
        for (int i = 0; i < pathCount; i++)
        {
            char *path = paths[i];
            char *program = command[0];
            char fullPath[1024]; // Buffer for the full path

            // Construct the full path to the command
            snprintf(fullPath, sizeof(fullPath), "%s%s", path, program);
            //now we can exec the program
            // Check if the program is accessible
            if (access(fullPath, X_OK) == 0)
            {
                AccessProg = true;
                
                int redirect_idx = -1;
                int multi_idx_cnt;


                //check for redirect if present
                for (int i = 0; i < ArgCount; i++)
                {
                    if (strcmp(command[i], ">") == 0)
                    {
                        redirect_idx = i;
                        break; //just want where the idx of the < is so i can redirect output to the output after this
                    }
                }

                //check for & for multi commands
                for (int i = 0; i < ArgCount; i++)
                {
                    if (strcmp(command[i], "&") == 0)
                    {
                        ++multi_idx_cnt;
                    }
                }

                //if redirect is not -1 then we have redirect, fork first since we have to run prog

                pid_t pid = fork();
                if (pid == 0)
                {
                    if(redirect_idx != -1)
                    {
                        int fd = open(command[redirect_idx + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644); 
                        //+1 sicne we want to open the file after >
                        //O_CREAT -> make file if DNE
                        //O_WRONLY -> write only file
                        //O_TRUNC -> if exists trunc then overwrite (needed in assignement)
                        //reference -> https://www.geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/
                        if (fd == -1)
                        {
                            perror("Failed to open file for redirection");
                            exit(1);
                        }

                        //reference for below code -> https://www.geeksforgeeks.org/dup-dup2-linux-system-call/
                        dup2(fd, STDOUT_FILENO);
                        close(fd);


                        // Remove the redirection symbol and file from the command
                        command[redirect_idx] = NULL;
                    }

                    // exec ccommand on child proc
                    execv(fullPath, command);
                    perror("execv failed");
                    exit(1);
                }
                else if (pid > 0)
                {
                    wait(NULL);
                }
                else
                {
                    perror("fork failed");
                }
                break;
            }
        }

        if (!AccessProg)
        {
            fprintf(stderr, "Command not found: %s\n", command[0]);
        }
    }
    else
    {
        fprintf(stderr, "No path set\n");
    }
}

void execCommand(char *command)
{
    // Tokenize command string
    char *token = strtok(command, " ");
    int i = 0;
    ArgCount = 0;

    while (token != NULL)
    {
        command_arr[i] = token;
        token = strtok(NULL, " ");
        i++;
        ArgCount++;
    }
    command_arr[ArgCount] = NULL; // NULL-terminate the argument list otherwise execv will fail

    // Handle built-in commands
    if (strcmp(command_arr[0], "exit") == 0)
    {
        cmdExit();
    }
    else if (strcmp(command_arr[0], "path") == 0)
    {
        cmdPath(command_arr);
    }
    else if (strcmp(command_arr[0], "cd") == 0)
    {
        cmdCD(command_arr);
    }
    else
    {
        programsExec(command_arr);
    }
}

void interactive_mode()
{
    // Command buffer
    char *command = NULL;
    size_t command_size = 0; // Initialize to 0, getline will allocate memory

    while (true)
    {
        printf("witsshell> ");
        ssize_t nread = getline(&command, &command_size, stdin);
        //add support for no commmand given, check if command is null
        if (strcmp(command, "\n") == 0)
        {
            continue;
        }
        else
        {
            // Strip newline character
            command[strcspn(command, "\n")] = 0;
            execCommand(command);
        }
    }
    free(command); // Free the allocated buffer
}

void batch_mode()
{
    printf("In batch mode\n");
}
