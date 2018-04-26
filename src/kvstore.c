#include "kvstore.h"
#include "transaction.h"
#include "client.h"
#include "server.h"
#include "storage.h"
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
	//start the server thread here
	//pass storage lock as argument	
	pthread_create(&s_tid, NULL, server, &storage_lock);
	//initialize storage
	st_init();
}

int put(unsigned long key, char *value, size_t value_length) {
	//check if this key maps to some other node
	//send key and value to the mapped node
	transaction *request = build_transaction(1, 0, key, value_length, value);
	char *request_char_buffer = serialize_transaction(request);
	char *response_char_buffer = NULL;
	transaction *response = NULL;
	//grab lock to access client
	pthread_mutex_lock(&network_lock);
	response_char_buffer = client(request_char_buffer);
	pthread_mutex_unlock(&network_lock);
	response = deserialize_transaction(response_char_buffer);
	//free allocated space
	free_transaction(request);
	free(request_char_buffer);
	free(response_char_buffer);
	if (response->ack == 1) {
		free_transaction(response);
		return (1);
	}
	else {
		free_transaction(response);
		return (0);
	}
}

char *get(unsigned long key, size_t *value_length) {
	//check if key maps to a different node
	//send key and request value to the mapped node
	transaction *request = build_transaction(0, 0, key, 0, NULL);
	char *request_char_buffer = serialize_transaction(request);
	char *response_char_buffer = NULL;
	transaction *response = NULL;
	char *ret = NULL;
	pthread_mutex_lock(&network_lock);
	response_char_buffer = client(request_char_buffer);
	pthread_mutex_unlock(&network_lock);
	response = deserialize_transaction(response_char_buffer);
	//free allocated space
	free_transaction(request);
	free(request_char_buffer);
	free(response_char_buffer);
	if (response->ack == 1) {
		//allocate space for the return buffer
		if ((ret = malloc(sizeof(char)*response->value_length)) == NULL) {
			*value_length = 0;
			perror("malloc failed: ");
			return (NULL);
		}
		else {
			//copy data to return buffer
			memcpy(ret, response->value, response->value_length);
			*value_length = response->value_length;	
			//free space
			free_transaction(response);
			return (ret);
		}
	}
	else {
		free_transaction(response);
		*value_length = 0;
		return (NULL);
	}	
}
