#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>

//flag for exit
bool flagExit = false;

//count the number of arguments
int count;

//mutiple command argument array
char *command_arr[100];

int main(int MainArgc, char *MainArgv[]){
	if(MainArgc > 2)
	{
		printf("Usage: %s [batch_file]\n",MainArgv[0]);
		exit(1);
	}

	//interactive mode = no args
	if(MainArgc == 1)
	{
		interactive_mode();
	}
	else //batch mode = 1 arg (the batch file we want to exec)
	{
		batch_mode();
	}

	

	return(0);
}

void cmdExit()
{
    flagExit = true;
    printf("Exiting\n");
    exit(0);
}

void cmdPath(char* command)
{
    printf("Path\n");
}

void cmdCD(char* command)
{
    printf("Arrived in CD\n");
    if (count == 0 || count >2)
    {
        printf("Invalid number of arguments\n");
        printf("%d\n", count);
        return;
    }
    else if (count == 2)
    {
        chdir(command_arr[1]);
    }
}

void execCommand(char* command)
{
    //execute the multiple commands {builtin - exit, cd, path or not}
    //have to code exit cd and path.

    //expect more than 1 command, hence split the command and store in array command_arr
    char *token = strtok(command, " ");
    int i = 0;
    count = 0;
    while(token != NULL)
    {
        command_arr[i] = token;
        token = strtok(NULL, " ");
        i++;
        count++;
    }

    //is above command?
    if (!strcmp(command_arr[0], "exit"))
        cmdExit();

    else if(!strcmp(command_arr[0], "path"))
    {
        //send in the command_arr to the function
        cmdPath(command_arr);
    }
    else if(!strcmp(command_arr[0], "cd"))
    {
        //send in the command_arr to the function
        cmdCD(command_arr);
    }
}

void interactive_mode() {
    // Command buffer
    char *command = NULL;
    size_t command_size = 0;  // Initialize to 0, getline will allocate memory

    while (true) {
        printf("witsshell> ");
        ssize_t nread = getline(&command, &command_size, stdin);

        // Strip newline character
        command[strcspn(command, "\n")] = 0;
        execCommand(command);
    }
    free(command);  // Free the allocated buffer
}


void batch_mode()
{
	printf("In batch mode\n");
}

