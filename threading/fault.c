#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "bench.h"

int main(int argc, char** argv)
{
	ull high;
	ull low;

	ull start;
	ull end;

	uint diff;

	char* page = mmap(NULL, 4096, PROT_READ | PROT_WRITE, 
		MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

	RDTSC(start);
	page[0] = 1;
	RDTSC(end);

        printf("Start cycles: %llu\n", start);
        printf("End cycles  : %llu\n", end);
        printf("Difference  : %llu\n", (end - start));
	diff = end - start;
	int file = open("output.txt", O_APPEND | O_RDWR | O_CREAT, 0644);
        if(file < 0) printf("BAD FILE!\n");
        char numbuffer[512];
        snprintf(numbuffer, 512, "%lu\n", diff);
        write(file, numbuffer, strlen(numbuffer));
        close(file);

	return 0;
}
