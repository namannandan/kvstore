#include "transaction.h"
#include <stdlib.h>
#include <string.h>

transaction *build_transaction(char op, char ack, unsigned long key, size_t value_length, char *value) {
	//allocate space for the transaction	
	transaction *trans;
	if ((trans = (transaction *) malloc(sizeof(transaction)))==NULL) {
		perror ("malloc failed: ");
		return (NULL);
	}
	char *buffer;
	//allocate space to store a copy of the value
	if ((buffer = (char *)malloc(value_length)) == NULL) {
		perror("malloc failed: ");
		return (NULL);
	}
	memcpy(buffer, value, value_length);
	//initialize the transaction
	trans->op = op;
	trans->ack = ack;
	trans->key = key;
	trans->value_length =  value_length; 
	trans->value = buffer;
	return (trans);
}

void free_transaction(transaction *t) {
	//free value
	free(t->value);
	//free transaction
	free(t);
	return;
}

char *serialize_transaction (transaction *t) {
	//allocate space for the buffer
	char *buffer;
	unsigned long buffer_length = sizeof(t->op) 
				    + sizeof(t->ack)
				    + sizeof(t->key)
				    + sizeof(t->value_length)
				    + t->value_length;
	if ((buffer = (char *) malloc(buffer_length)) == NULL) {
		perror ("malloc failed: ");
		return (NULL);
	}
	//copy data into the buffer
	char *buffer_ptr = buffer;
	memcpy(buffer_ptr, &(t->op), sizeof(t->op));
	buffer_ptr = buffer_ptr + sizeof(t->op);
	memcpy(buffer_ptr, &(t->ack), sizeof(t->ack));
	buffer_ptr = buffer_ptr + sizeof(t->ack);
	memcpy(buffer_ptr, &(t->key), sizeof(t->key));
	buffer_ptr = buffer_ptr + sizeof(t->key);
	memcpy(buffer_ptr, &(t->value_length), sizeof(t->value_length));
	buffer_ptr = buffer_ptr + sizeof(t->value_length);
	memcpy(buffer_ptr, t->value, t->value_length);
	//return the buffer
	return (buffer);
}

transaction *deserialize_transaction (char *buffer) {
	//build a dummy transaction
	transaction *trans = build_transaction (0, 0, 0, 0, NULL);
	//fill in the details of the transaction
	char *buffer_ptr = buffer;
	trans->op = *(char *)buffer_ptr;
	buffer_ptr = buffer_ptr + sizeof(trans->op);
	trans->ack = *(char *)buffer_ptr;
	buffer_ptr = buffer_ptr + sizeof(trans->ack);
	trans->key = *(unsigned long *)buffer_ptr;
	buffer_ptr = buffer_ptr + sizeof(trans->key);
	trans->value_length = *(size_t *)buffer_ptr;
	buffer_ptr = buffer_ptr + sizeof(trans->value_length);
	if ((trans->value = malloc(trans->value_length)) == NULL) {
		perror("malloc failed: ");
		return (NULL);
	}
	memcpy(trans->value, buffer_ptr, trans->value_length);
	return (trans);
}
