#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "bench.h"

typedef unsigned long ulong;

int main(int argc, char** argv)
{
	ull high;
	ull low;
	ull start;
	ull end;
	uint diff;


	uint val;
	uint best = (uint)-1;
	struct timeval start_t;
	struct timeval end_t;
	int x;
	for(x = 0;x < 10000;x++)
	{
		RDTSC(start);
		if(gettimeofday(&start_t, NULL)) perror("");
		if(gettimeofday(&end_t, NULL)) perror("");
		RDTSC(end);
		uint diff = end - start;
		if(diff < best) best = diff;
	}

	int file = open("output.txt", O_APPEND | O_RDWR | O_CREAT, 0644);
	if(file < 0) printf("BAD FILE!\n");
	char numbuffer[512];
	snprintf(numbuffer, 512, "%u\n", best);
	write(file, numbuffer, strlen(numbuffer));
	close(file);


	exit(0);
}
