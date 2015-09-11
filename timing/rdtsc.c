#include <stdio.h>
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

int main(int argc, char** argv)
{
	uint val;
	ull start = read_tscp(&val);
	int x;
	for(x = 0;x < 1;x++)
	{
		//getpid();
		asm volatile("movl $20, %%eax\r\t" : "=a" (val));
		asm volatile("int $0x80" : "=a" (val));
	}

	ull end = read_tscp(&val);
	printf("Start cycles: %llu\n", start);
	printf("End cycles  : %llu\n", end);
	printf("Difference  : %llu\n", (end - start));

	exit(0);
}
