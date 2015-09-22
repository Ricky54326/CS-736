#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "bench.h"

pthread_mutex_t mut1, mut2;
ull high;
ull low;
ull start;
ull end;
unsigned int best;

void *pthread_func1(void *argument)
{
	best = (unsigned int)-1;
	pthread_mutex_unlock(&mut2);
	/* Block for the thread to run */
	pthread_mutex_lock(&mut1);
	/* The other thread is now guarenteed to be blocked! */
	int x;
	for(x = 0;x < 10000;x++)
	{
		RDTSC(start);
		/* The other thead should be blocked now. */
		pthread_mutex_unlock(&mut2);

		/**
		 * Now once we are able to acquire this lock, the
		 * other thread was allowed to run for one cycle		
		 */
		pthread_mutex_lock(&mut1);

		/* The other thread unlocked us! */
		RDTSC(end);

		unsigned int diff = end - start;
		if(diff < best) best = diff;
	}

	return NULL;
}

void *pthread_func2(void *argument)
{
	for(;;)
	{
		pthread_mutex_lock(&mut2);
		pthread_mutex_unlock(&mut1);
	}
	return NULL;
}

void time_pthread(){
	pthread_t thr1, thr2;

	pthread_mutex_init(&mut1, NULL);
	pthread_mutex_init(&mut2, NULL);
	pthread_mutex_lock(&mut1);
	pthread_mutex_lock(&mut2);

	pthread_create(&thr1, NULL, pthread_func1, NULL);
	pthread_create(&thr2, NULL, pthread_func2, NULL);
	pthread_join(thr1, NULL);
	pthread_cancel(thr2);

	unsigned int diff = best;
	printf("Best run: %lu\n", diff);

	int file = open("output.txt", O_APPEND | O_RDWR | O_CREAT, 0644);
	if(file < 0) printf("BAD FILE!\n");
	char numbuffer[512];
	snprintf(numbuffer, 512, "%lu\n", diff);
	write(file, numbuffer, strlen(numbuffer));
	close(file);
}

int main(int argc, char** argv)
{
	time_pthread();
	return 0;

}
