#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "bench.h"

void* pthread_func(void* argument)
{
	return NULL;
}

void time_pthread(){
	pthread_t thr;

	uint val;
	ull start = read_tscp(&val);	
	if(pthread_create(&thr, NULL, pthread_func, NULL ))
		perror("");
	
	pthread_join(thr, (void**)NULL);
	ull end = read_tscp(&val);
	printf("Start cycles: %llu\n", start);
    printf("End cycles  : %llu\n", end);
    printf("Difference  : %llu\n", (end - start));



	return;
}

int main(int argc, char** argv)
{
	time_pthread();
	return 0;

}
