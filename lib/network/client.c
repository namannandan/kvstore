#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stddef.h>
#include "client.h"

#define PORT 8080

//client is responsible for sending and receiving byte streams
char *client(char *send_buffer) {
	struct sockaddr_in address;
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	size_t receive_buffer_size = 1024;
	char *receive_buffer;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("\n Socket creation error \n");
		return (NULL);
	}
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
		printf("\nInvalid address/ Address not supported \n");
		return (NULL);
	}
	//TODO:(fix this) retry connect until the connection goes through
	while (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		//printf("\nConnection Failed \n");
		//return (NULL);
	}
	send(sock , send_buffer, *(unsigned long *)send_buffer, 0);
	//allocate buffer space for incoming data
	if ((receive_buffer = malloc(receive_buffer_size*sizeof(char))) == NULL) {
		perror("malloc failed: ");
		return (NULL);
	}
	valread = read(sock, receive_buffer, receive_buffer_size);
	//close socket
	close(sock);
	return (receive_buffer);
}

