#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "bench.h"

void* pthread_func(void* argument)
{
	uint high;
	uint low;
	ull start;
	ull end;
	unsigned long diff;

	pthread_t cmp_1 = 0x12345678;
	pthread_t cmp_2 = pthread_self();

	uint val;
	RDTSC(start);	
	pthread_equal(cmp_1, cmp_2);
	RDTSC(end);

	return (void*)diff;
}

int main(int argc, char** argv)
{
	pthread_t thr;

	uint best = (uint)-1;

	int x;
	for(x = 0;x < 10000;x++)
	{
		if(pthread_create(&thr, NULL, pthread_func, NULL ))
		{
			perror("");
			continue;
		}

		uint test;
		pthread_join(thr, (void**)&test);
		if(test < best) best = test;
	}
	return 0;
}
