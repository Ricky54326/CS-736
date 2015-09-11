#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef unsigned long ulong;

int main(int argc, char** argv)
{
	uint val;
	struct timeval start;
	struct timeval end;
	if(gettimeofday(&start, NULL)) perror("");
	int x;
	for(x = 0;x < 1;x++)
	{
		//getpid();
		asm volatile("movl $20, %%eax\r\t" : "=a" (val));
		asm volatile("int $0x80" : "=a" (val));
	}

	if(gettimeofday(&end, NULL)) perror("");
	printf("Start cycles: %lu us\n", start.tv_usec);
	printf("End cycles  : %lu us\n", end.tv_usec);
	printf("Difference  : %lu us\n", (end.tv_usec - start.tv_usec));

	exit(0);
}
