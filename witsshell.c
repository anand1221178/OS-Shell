#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>


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
    printf("Exiting\n");
    exit(0);
}

void cmdPath()
{
    printf("Path\n");
}

void cmdCD()
{
    printf("CD\n")
}

void execCommand(char* command)
{
    //execute the multiple commands {builtin - exit, cd, path or not}
    //have to code exit cd and path.

    //is above command?
    if (strcmp(command, "exit"))
        cmdExit();
    else if(strcmp(command, "path"))
        cmdPath();
    else if(strcmp(command, "cd"))
        cmdCD();
}

void interactive_mode()
{
    // command buffer
    char *command = NULL;
    size_t command_size = 128;
    command = (char *)malloc(command_size * sizeof(char));

    bool flag = false;
    //read in command and send for execution to executing function -> do this in a loop
    while (!flag)
    {
        printf("witsshell> ");
        getline();
    }
}


void batch_mode()
{
	printf("In batch mode\n");
}

