#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "bench.h"

void* pthread_func(void* argument)
{
	printf("On the new thread!\n");

	uint val;
	ull start = read_tscp(&val);	
	pthread_t self = pthread_self();
	ull end = read_tscp(&val);

        printf("Start cycles: %llu\n", start);
        printf("End cycles  : %llu\n", end);
        printf("Difference  : %llu\n", (end - start));

	return NULL;
}

int main(int argc, char** argv)
{
	pthread_t thr;

	if(pthread_create(&thr, NULL, pthread_func, NULL ))
		perror("");

	pthread_join(thr, (void**)NULL);
	return 0;

}
