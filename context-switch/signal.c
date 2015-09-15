#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void main_handler(int sig)
{
	printf("Parent caught exception.\n");
}

void child_handler(int sig)
{
	printf("Child caught exception.\n");
}

int main(int argc, char** argv)
{
	pid_t parent = getpid();
	pid_t child = fork();


	if(child == 0)
	{
		/* In the child process */
		
		

		/* Wait for signal. */
		for(;;);
	} else if(child > 0) 
	{
		/* Assign the parent signal handler */
		struct sigaction parent_handle;
		if(sigaction(SIGUSR1, NULL, &parent_handle))
		{
			perror("");
			return -1;
		}

		parent_handle.sa_handler = main_handler;
		parent_handle.sa_sigaction = NULL;
		parent_handle.sa_flags = 0;
		if(sigaction(SIGUSR1, &parent_handle, NULL))
		{
			perror("");
			return -1;
		}

		printf("Success!\n");

		if(kill(parent, SIGUSR1))
		{
			perror("");
			return -1;
		}
	} else {
		printf("Fork failure.\n");
		perror("");
		return -1;
	}

	return 0;
}
