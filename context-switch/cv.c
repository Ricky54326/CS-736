#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "bench.h"


pthread_cond_t cv1, cv2;
pthread_mutex_t mut1, mut2;



void *pthread_func1(void *argument)
{
	printf("Pthread1\n");
	pthread_cond_signal(&cv1);
	pthread_cond_wait(&cv2, &mut2);
	pthread_exit(NULL);
	return NULL;
}

void *pthread_func2(void *argument)
{
	printf("Pthread2\n");
	pthread_cond_wait(&cv1, &mut1);
	pthread_cond_signal(&cv2);
	pthread_exit(NULL);
	return NULL;
}

void time_pthread(){

	pthread_t thr1, thr2;

	uint val;
	ull high;
	ull low;
	unsigned long diff;
	ull start;
	ull end;
	int i, min = 99999999;


	/* initialize CVs */
	pthread_cond_init(&cv1, NULL);
	pthread_cond_init(&cv2, NULL);

    /* initialize mutexes */
	pthread_mutex_init(&mut1, NULL);
	pthread_mutex_init(&mut2, NULL);

	for(i = 0; i < 10000; i++){
		/* TODO: Better way to initialize them as locked? */
		pthread_cond_signal(&cv1);
		pthread_cond_signal(&cv2);
		// pthread_cond_wait(&cv1, &mut1);
		// pthread_cond_wait(&cv2, &mut2);

		RDTSC(start);
		pthread_create(&thr1, NULL, pthread_func1, NULL);
		pthread_create(&thr2, NULL, pthread_func2, NULL);
		pthread_join(thr1, NULL);
		RDTSC(end);
		diff = end - start;

		//save min timing
		if(diff < min)
			min = diff;
	}		


	printf("Min cycles: %d\n", min);

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