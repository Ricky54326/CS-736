#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "bench.h"

ull start;
ull end;
ull high;
ull low;
unsigned long diff;

pid_t parent;
pid_t child;

#define SIGNAL SIGUSR2

struct process_sync
{
	pthread_mutex_t lock;
	pthread_mutexattr_t lock_attr;
	pthread_cond_t cond;
	pthread_condattr_t cond_attr;
	char child_ready;
	char value;
};

unsigned int final_val;
void main_handler(int sig)
{
	RDTSC(end);
	final_val = end - start;
	if(sig != SIGNAL)
	{
		/* Not the right signal!! */
		printf("An invalid signal was received.\n");
		end = start + 10000000;
		final_val = (unsigned int)-1;
	}
}

void child_handler(int sig)
{
	/* Relay the signal */
	kill(parent, sig);
	/* Immediatly end process! */
	exit(0);
}

unsigned int experiment(void)
{
	struct process_sync* share = mmap(NULL, 4096, PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS | MAP_POPULATE, -1, 0);
	/* Did we get the requested area? */
	if(share == (void*)-1)
	{
		perror("");
		return -1;
	}

	pthread_mutexattr_init(&share->lock_attr);
	/* Allow mutex's to be used across processes */
	pthread_mutexattr_setpshared(&share->lock_attr, 
			PTHREAD_PROCESS_SHARED);
	pthread_condattr_init(&share->cond_attr);
	/* Allow condition variables to be used across processes */
	pthread_condattr_setpshared(&share->cond_attr, PTHREAD_PROCESS_SHARED);

	/* Do final initilization for mutex and condition variable */
	pthread_mutex_init(&share->lock, &share->lock_attr);
	pthread_cond_init(&share->cond, &share->cond_attr);
	/* Set child unready */
	share->child_ready = 0;

	/* Set the parent pid */
	parent = getpid();
	/* Create the child + get the child pid */
	child = fork();

	if(child == 0)
	{
		/* In the child process */
		freopen("child.txt", "w", stdout);
		/* Acquire the guard */
		pthread_mutex_lock(&share->lock);
		printf("Initilizing child...\t\t\t\t");

		struct sigaction child_handle;
		if(sigaction(SIGNAL, NULL, &child_handle))
		{
			printf("[FAIL]\n");
			return -1;
		}

		child_handle.sa_handler = child_handler;
		if(sigaction(SIGNAL, &child_handle, NULL))
		{
			printf("[FAIL]\n");
			return -1;
		}
		printf("[ OK ]\n");
		fflush(stdout);

		/* Set child ready */
		share->child_ready = 1;
		printf("Child is ready!\n");

		printf("Waking parent...\n");
		fflush(stdout);
		/* Release the lock */
		pthread_mutex_unlock(&share->lock);
		/* Wake the parent if they were sleeping */
		pthread_cond_signal(&share->cond);

		/* We are waiting now. */
		for(;;);
	} else if(child > 0) 
	{
		freopen("parent.txt", "w", stdout);

		/* Acquire the guard */
		pthread_mutex_lock(&share->lock);
		printf("Initilizing parent...\t\t\t\t");

		/* Assign the parent signal handler */
		struct sigaction parent_handle;
		if(sigaction(SIGNAL, NULL, &parent_handle))
		{
			perror("");
			return -1;
		}

		parent_handle.sa_handler = main_handler;
		if(sigaction(SIGNAL, &parent_handle, NULL))
		{
			perror("");
			return -1;
		}

		printf("[ OK ]\n");

		printf("Checking for child ready...\t\t\t");
		while(!share->child_ready)
		{
			printf("[WAIT]\n");
			fflush(stdout);
			pthread_cond_wait(&share->cond, &share->lock);
			printf("Checking for child ready...\t\t\t");
			fflush(stdout);
		}
		printf("[DONE]\n");
		printf("Starting experiment...\n");
		fflush(stdout);

		unsigned int val;
		RDTSC(start); /* Start the timer */
		kill(child, SIGNAL); /* Signal the child */

		/* Wait for signal to come back. */	
		for(;!final_val;);

		diff = final_val;

		int file = open("output.txt", O_APPEND |O_RDWR | O_CREAT, 0644);
		if(file < 0) printf("BAD FILE!\n");
		char numbuffer[512];
		snprintf(numbuffer, 512, "%lu\n", diff);
		write(file, numbuffer, strlen(numbuffer));
		close(file);
		return diff;

	} else {
		printf("Fork failure.\n");
		perror("");
		return -1;
	}

	return 0;
}

int main(int argc, char** argv)
{
	unsigned int best = (unsigned int)-1;
	
	int x;
	for(x = 0;x < 10000;x++)
	{
		unsigned int exp = experiment();
		if(exp < best) best = exp;
	}

	unsigned int diff = best;

	int file = open("output.txt", O_APPEND |O_RDWR | O_CREAT, 0644);
                if(file < 0) printf("BAD FILE!\n");
                char numbuffer[512];
                snprintf(numbuffer, 512, "%lu\n", diff);
                write(file, numbuffer, strlen(numbuffer));
                close(file);

	return 0;
}
