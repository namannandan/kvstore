#include "network.h"
#include "transaction.h"
#include "client.h"
#include "server.h"
#include "nw_config.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

//define the global variables
unsigned long node_id = 0;
unsigned long server_port_numbers[NUM_NODES] = {8080, 8081/*, 8082, 8083, 8084*/};
char *server_ip_addresses[NUM_NODES] = {"128.46.4.62", "128.211.140.138"/*, "127.0.0.1", "127.0.0.1", "127.0.0.1"*/}; 

pthread_t nw_init(pthread_mutex_t *storage_lock, unsigned long id) {
	node_id = id;
	pthread_t s_tid;
	//start the server thread here
	//pass storage lock as argument	
	s_tid = pthread_create(&s_tid, NULL, server, storage_lock);
	return (s_tid);
}

int nw_put(unsigned long key, char *value, size_t value_length, pthread_mutex_t *network_lock) {
	transaction *request = build_transaction(1, 0, key, value_length, value);
	char *request_char_buffer = serialize_transaction(request);
	char *response_char_buffer = NULL;
	transaction *response = NULL;
	//grab lock to access client
	pthread_mutex_lock(network_lock);
	response_char_buffer = client(request_char_buffer, key%NUM_NODES);
	pthread_mutex_unlock(network_lock);
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

char *nw_get(unsigned long key, size_t *value_length, pthread_mutex_t *network_lock) {
	transaction *request = build_transaction(0, 0, key, 0, NULL);
	char *request_char_buffer = serialize_transaction(request);
	char *response_char_buffer = NULL;
	transaction *response = NULL;
	char *ret = NULL;
	pthread_mutex_lock(network_lock);
	response_char_buffer = client(request_char_buffer, key%NUM_NODES);
	pthread_mutex_unlock(network_lock);
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
