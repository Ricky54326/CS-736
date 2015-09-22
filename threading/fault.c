#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "bench.h"

uint test(void)
{
        ull high;
        ull low;
	ull start;
	ull end;
	uint diff;

	int* page = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, 
		MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);


	RDTSC(start);
	page[0] = 1;
	RDTSC(end);
	diff = end - start;

	munmap(page, 0x1000);
	return diff;
}

int main(int argc, char** argv)
{
	int x;
	uint best = (uint)-1;
	for(x = 0;x < 1000;x++)
	{
		uint result = test();
		if(result < best) best = result;
	}

	unsigned long diff = best;
	int file = open("output.txt", O_APPEND | O_RDWR | O_CREAT, 0644);
        if(file < 0) printf("BAD FILE!\n");
        char numbuffer[512];
        snprintf(numbuffer, 512, "%lu\n", diff);
        write(file, numbuffer, strlen(numbuffer));
        close(file);

	return 0;
}
