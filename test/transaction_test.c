#include <stdio.h>
#include <stdlib.h>
#include "transaction.h"

int main (void) {
	//create a transaction
	printf("building a transaction\n");
	char *message = "hello world";
	transaction *t = build_transaction(2, 1, 1234, 11, message);
	printf("op: %d\n", t->op);
	printf("ack: %d\n", t->ack);
	printf("key: %lu\n", t->key);
	printf("value_length: %lu\n", t->value_length);
	printf("value: ");
	fwrite(t->value, sizeof((t->value)[0]), t->value_length, stdout);
	printf("\n\n");
	//serialize the transaction
	printf("serializing the transaction\n\n");
	char *ser = serialize_transaction(t);
	//deserailize transaction
	printf("deserializing the transaction\n\n");
	transaction *d_t = deserialize_transaction(ser);
	//display the values
	printf("reconstructed transaction\n");
	printf("op: %d\n", d_t->op);
	printf("ack: %d\n", d_t->ack);
	printf("key: %lu\n", d_t->key);
	printf("value_length: %lu\n", d_t->value_length);
	printf("value: ");
	fwrite(d_t->value, sizeof((d_t->value)[0]), d_t->value_length, stdout);
	printf("\n");
	//free transactions
	free(t);
	free(d_t);
}
