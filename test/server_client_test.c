#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "server.h"
#include "client.h"
#include "transaction.h"

int main()
{
	pthread_t s_tid, c_tid;
	printf("Starting server and client\n");
	pthread_create(&s_tid, NULL, server_thread, NULL);
	pthread_create(&c_tid, NULL, client_thread, NULL);
	pthread_join(s_tid, NULL);
	pthread_join(c_tid, NULL);
	printf("Done\n");
	exit(0);
}
