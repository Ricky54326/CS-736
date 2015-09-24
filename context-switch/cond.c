#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "bench.h"


pthread_cond_t cv1, cv2;
pthread_mutex_t m;

uint val;
ull high;
ull low;
unsigned long diff;
ull start;
ull end;

int thread2_ready = 0;

void *pthread_func1(void *argument)
{
	pthread_mutex_lock(&m);
	if(!thread2_ready)
		pthread_cond_wait(&cv1, &m);

	unsigned int best = (unsigned int)-1;
	int x;
	for(x = 0;x < 100000;x++)
	{
		RDTSC(start);
		pthread_cond_signal(&cv2);
		pthread_cond_wait(&cv1, &m);
		RDTSC(end);
		unsigned int diff = end - start;
		if(diff < best) best = diff;
	}		

	diff = best;

	return NULL;
}

void *pthread_func2(void *argument)
{
	pthread_mutex_lock(&m);
	thread2_ready = 1;
	pthread_cond_signal(&cv1);

	for(;;)
	{
		pthread_cond_signal(&cv1);
		pthread_cond_wait(&cv2, &m);
	}

	return NULL;
}

void time_pthread()
{
	pthread_t thr1, thr2;

	/* initialize CVs */
	pthread_cond_init(&cv1, NULL);
	pthread_cond_init(&cv2, NULL);

	/* initialize mutexes */
	pthread_mutex_init(&m, NULL);

	pthread_create(&thr1, NULL, pthread_func1, NULL);
	pthread_create(&thr2, NULL, pthread_func2, NULL);
	pthread_join(thr1, NULL);
	pthread_cancel(thr2);

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
