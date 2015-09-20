#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#include "bench.h"

int main(int argc, char** argv)
{
	uint high;
	uint low;
	uint pid;
	ull start;
	ull end;
	uint val;
	uint best = (uint)-1;
	int x;

	for(x = 0;x < 1000;x++)
	{
		RDTSC(start);
		/* GET PID */
#ifdef GETPID
		asm volatile("movl $20, %%eax\r\t" : "=a" (val));
		asm volatile("int $0x80" : "=a" (pid));
#endif

		/* GET UID */
#ifdef GETUID
		asm volatile("movl $24, %%eax\r\t" : "=a" (val));
		asm volatile("int $0x80" : "=a" (pid));
#endif
		RDTSC(end);

		/* Is this the new best? */
		uint diff = (end - start);
		if(diff < best) best = diff;

	}
	
	printf("best run  : %u\n", best);
	printf("PID: %d  %d\n", pid, getpid());
	int file = open("output.txt", O_APPEND | O_RDWR | O_CREAT, 0644);
	if(file < 0) printf("BAD FILE!\n");
	char numbuffer[512];
	snprintf(numbuffer, 512, "%u\n", best);
	write(file, numbuffer, strlen(numbuffer));
	close(file);

	exit(0);
}
