#include "kvstore.h"
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>

int main(int argc, char*argv[]) {
	struct timeval start;
	struct timeval stop;
	int i = 0;
	size_t length = 0;
	char *ret = NULL;
	//time elapsed in seconds
	double time = 0.0;
	if (argc != 2) {
		printf("enter exactly one argument (node_id)\n");
		exit(1);
	}
	kvstore_init(atoi(argv[1]));
	
	//local put
	gettimeofday(&start, NULL);
	for (i=0; i<1024; i=i+2) {
		put(i, "hello world", 11);	
	}
	gettimeofday(&stop, NULL);
	time += (double)((stop.tv_usec-start.tv_usec)/1000000.0);
	time += (double)(stop.tv_sec-start.tv_sec);
	printf("Local put: %lf transactions per second\n", 512.0/time);

	//remote put
	time = 0.0;
	gettimeofday(&start, NULL);
	for (i=1; i<1024; i=i+2) {
		put(i, "hello world", 11);	
	}
	gettimeofday(&stop, NULL);
	time += (double)((stop.tv_usec-start.tv_usec)/1000000.0);
	time += (double)(stop.tv_sec-start.tv_sec);
	printf("Remote put: %lf transactions per second\n", 512.0/time);

	//local get
	time = 0;
	gettimeofday(&start, NULL);
	for (i=0; i<1024; i=i+2) {
		get(i, &length);
	}
	gettimeofday(&stop, NULL);
	time += (double)((stop.tv_usec-start.tv_usec)/1000000.0);
	time += (double)(stop.tv_sec-start.tv_sec);
	printf("Local get: %lf transactions per second\n", 512.0/time);

	//remote get
	time = 0;
	gettimeofday(&start, NULL);
	for (i=1; i<1024; i=i+2) {
		get(i, &length);
	}
	gettimeofday(&stop, NULL);
	time += (double)((stop.tv_usec-start.tv_usec)/1000000.0);
	time += (double)(stop.tv_sec-start.tv_sec);
	printf("Remote get: %lf transactions per second\n", 512.0/time);
	
	return (0);
}
