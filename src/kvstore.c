#include "kvstore.h"
#include "storage.h"
#include "network.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

//locks
//mutex lock for accessing storage and network on this node
static pthread_mutex_t storage_lock;
static pthread_mutex_t network_lock;

void kvstore_init() {
	//server thread id
	pthread_t s_tid;
	//initialize the locks
	pthread_mutex_init(&storage_lock, NULL);
	pthread_mutex_init(&network_lock, NULL);
	//initialize network
	s_tid = nw_init(&storage_lock);
	//initialize storage
	st_init();
}

int put(unsigned long key, char *value, size_t value_length) {
	int ret = 0;
	//check if this key maps to some other node
	if(key%2 == 0) {
		//store on this node
		pthread_mutex_lock(&storage_lock);
		ret = st_put(key, value, value_length);
		pthread_mutex_unlock(&storage_lock);
	}
	else {
		//send key and value to the mapped node
		ret = nw_put(key, value, value_length, &network_lock);
	}
	return (ret);
}

char *get(unsigned long key, size_t *value_length) {
	char *ret = NULL;
	//check if key maps to a different node
	if(key%2 == 0) {
		//get value from this node
		ret = st_get(key, value_length);
	}
	else {
		//send key and request value to the mapped node
		ret = nw_get(key, value_length, &network_lock);
	}
	return (ret);
}
