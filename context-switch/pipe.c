#include <string.h>
#include <signal.h>
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
uint diff;
ull high;
ull low;

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
};

int main(int argc, char** argv)
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

	/* Pipe to send messages from parent to child */
	int to_child[2];
	pipe(to_child);

	/* Pipe to send messages from child to parent */
	int to_parent[2];
	pipe(to_parent);

	

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
		
		int readp = to_child[0];
		int writep = to_parent[1];
		close(to_child[1]);
		close(to_parent[0]);
		printf("[DONE]\n");

		/* Set child ready */
		share->child_ready = 1;
		printf("Child is ready!\n");

		printf("Waking parent...\n");
		fflush(stdout);
		/* Release the lock */
		pthread_mutex_unlock(&share->lock);
		/* Wake the parent if they were sleeping */
		pthread_cond_signal(&share->cond);

		/* We are reading now. */
		int word;
		read(readp, &word, sizeof(int));

		/* Write */
		write(writep, &word, sizeof(int));

		/* We're done */
		exit(0);
	} else if(child > 0) 
	{
		freopen("parent.txt", "w", stdout);

		/* Acquire the guard */
		pthread_mutex_lock(&share->lock);
		printf("Initilizing parent...\t\t\t\t");

		int writep = to_child[1];
                int readp = to_parent[0];
                close(to_child[0]);
                close(to_parent[1]);

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

		int send = 0xAABBCCDD;
		int recv;
		RDTSC(start);

		/* Write to the child */
		write(writep, &send, sizeof(int));
		/* Read response */
		read(readp, &recv, sizeof(int));
		RDTSC(end);

		

		if(send != recv)
		{
			printf("TRANSFER ERROR!\n");
			printf("0x%x  0x%x\n", send, recv);
			return -1;
		}

		printf("Transfer success\n");
		printf("Starting cycles: %llu\n", start);
		printf("Ending cycles  : %llu\n", end);
		printf("Cycles to comp : %llu\n", (end - start));

		diff = end - start;
		int file = open("output.txt", 
			O_APPEND | O_RDWR | O_CREAT, 0644);
		if(file < 0) printf("BAD FILE!\n");
		char numbuffer[512];
		snprintf(numbuffer, 512, "%lu\n", diff);
		write(file, numbuffer, strlen(numbuffer));
		close(file);

		return 0;
	} else {
		printf("Fork failure.\n");
		perror("");
		return -1;
	}

	return 0;
}
