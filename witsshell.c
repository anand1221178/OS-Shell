#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>

bool flagExit = false;

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

void cmdPath()
{
    printf("Path\n");
}

void cmdCD()
{
    printf("CD\n");
}

void execCommand(char* command)
{
    //execute the multiple commands {builtin - exit, cd, path or not}
    //have to code exit cd and path.

    //is above command?
    if (!strcmp(command, "exit"))
        cmdExit();
    else if(!strcmp(command, "path"))
        cmdPath();
    else if(!strcmp(command, "cd"))
        cmdCD();
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

