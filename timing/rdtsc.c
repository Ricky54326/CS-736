#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

typedef unsigned long long u64;
typedef unsigned int uint;
typedef unsigned long long ull;

/** From the Linux Kernel */
static inline unsigned long long read_tscp(unsigned int *aux)
{
	unsigned long low, high;
	asm volatile("cpuid");
	asm volatile("rdtsc"
			: "=a" (low), "=d" (high), "=c" (*aux));
	return low | ((u64)high << 32);
}

// #define GETPID
#define GETUID

#define RDTSC(high, low, result) \
	asm volatile("cpuid"); \
	asm volatile("rdtsc" : "=a" (low), "=d" (high)); \
	result = ((ull)low) | ((ull)high << 32);

int main(int argc, char** argv)
{
	uint high;
	uint low;
	uint pid;
	ull start;
	ull end;
	uint val;
	RDTSC(high, low, start);
	/* GET PID */
#ifdef GETPID
	asm volatile("movl $20, %%eax\r\t" : "=a" (val));
	asm volatile("int $0x80" : "=a" (pid));
#endif

	/* GET UID */
#ifdef GETUID
	asm volatile("movl $20, %%eax\r\t" : "=a" (val));
	asm volatile("int $0x80" : "=a" (pid));
#endif

	RDTSC(high, low, end);
	printf("Start cycles: %llu\n", start);
	printf("End cycles  : %llu\n", end);
	printf("Difference  : %llu\n", (end - start));
	printf("PID: %d  %d\n", pid, getpid());
	uint diff = (uint)((ull)end - (ull)start);
	int file = open("output.txt", O_APPEND | O_RDWR | O_CREAT, 0644);
	if(file < 0) printf("BAD FILE!\n");
	char numbuffer[512];
	snprintf(numbuffer, 512, "%lu\n", diff);
	write(file, numbuffer, strlen(numbuffer));
	close(file);

	exit(0);
}
