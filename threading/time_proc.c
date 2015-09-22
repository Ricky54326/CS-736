#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "bench.h"

void* pthread_func(void* argument)
{
	return NULL;
}

void time_proc(){
	pthread_t thr;

	ull start;
	ull end;
	ull high;
	ull low;
	unsigned long diff;
	unsigned long best = (unsigned long)-1;
	uint val;

	int x;
	for(x = 0;x < 10000;x++)
	{
		RDTSC(start);
		//child
		if(fork() == 0){
			exit(EXIT_SUCCESS);
		}
		//parent
		else{
			wait();
		}
		RDTSC(end);

		diff = end - start;
		if(diff < best) best = diff;
	}

	diff = best;

	int file = open("output.txt", O_APPEND | O_RDWR | O_CREAT, 0644);
	if(file < 0) printf("BAD FILE!\n");
	char numbuffer[512];
	snprintf(numbuffer, 512, "%lu\n", diff);
	write(file, numbuffer, strlen(numbuffer));
	close(file);

	return;
}

int main(int argc, char** argv)
{
	time_proc();
	return 0;

}
