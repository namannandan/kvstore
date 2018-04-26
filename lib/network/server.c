#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stddef.h>
#include "server.h"
#include "transaction.h"
#include "storage.h"

#define PORT 8080

void *server(void *arg)
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	transaction *request;
	transaction *response;
	char *response_char_buffer;
	pthread_mutex_t *storage_lock = (pthread_mutex_t *)arg;
	char *val;
	size_t val_length = 0;
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	} 
	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	address.sin_port = htons( PORT );
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	//create a loop - handle incoming connections
	while(1) {
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		valread = read(new_socket ,buffer, 1024);	
		//deserialize the requst transaction
		request = deserialize_transaction(buffer);
		//grab lock to access storage
		pthread_mutex_lock(storage_lock);
		//access storage
		if (request->op == 0) {
			//get data
			val = st_get(request->key, &val_length);
		}
		else if (request->op == 1) {
			//store the data
			st_put(request->key, request->value, request->value_length);
		}
		else if (request->op == 2) {
			//TODO:delete
		}
		else {
			//TODO:handle this
		}
		//release lock
		pthread_mutex_unlock(storage_lock);
		//construct response based on the type of request
		if (request->op == 0) {
			response = build_transaction(request->op, 1, request->key, val_length, val);
			free(val);
		}
		else if (request->op == 1) {
			response = build_transaction(request->op, 1, request->key, 0, NULL); 
		}
		else if(request->op == 2) {
			//TODO:handle this
		}
		else {
			//TODO:handle this
		}
		response_char_buffer = serialize_transaction(response);
		send(new_socket, response_char_buffer, *(size_t *)response_char_buffer, 0);
		//free up space
		free_transaction(request);
		free_transaction(response);	
		free(response_char_buffer);
		//close connection to client
		close(new_socket);
	}
}
