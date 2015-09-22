#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef unsigned long ulong;

int main(int argc, char** argv)
{
	uint val;
	uint best = (uint)-1;
	struct timeval start;
	struct timeval end;
	int x;
	for(x = 0;x < 10000;x++)
	{
		if(gettimeofday(&start, NULL)) perror("");
		if(gettimeofday(&end, NULL)) perror("");
		ulong endmicro = end.tv_usec + (end.tv_sec * 1000000);
		ulong startmicro = start.tv_usec + (start.tv_sec * 1000000);
		uint diff = startmicro - endmicro;
		/* Convert to nano seconds */
		diff *= 1000;
		if(diff < best) best = diff;
		if(best == 0)
		{
			printf("X: %u\n", diff);
			printf("End:   %lu\n", endmicro);
			printf("Start: %lu\n", startmicro);
			break;
		}
	}
	printf("Start cycles: %lu us\n", start.tv_usec);
	printf("End cycles  : %lu us\n", end.tv_usec);
	printf("Difference  : %lu us\n", (end.tv_usec - start.tv_usec));

	int file = open("output.txt", O_APPEND | O_RDWR | O_CREAT, 0644);
	if(file < 0) printf("BAD FILE!\n");
	char numbuffer[512];
	snprintf(numbuffer, 512, "%u\n", best);
	write(file, numbuffer, strlen(numbuffer));
	close(file);


	exit(0);
}
