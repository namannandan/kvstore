#ifndef TRANSACTION_H_INCLUDED
#define TRANSACTION_H_INCLUDED

#include <stddef.h>

//transaction data structure
struct transaction {
	//operation
	//0 - get
	//1 - put
	//2 - delete
	char op;
	//acknowledgement
	//0 - not an acknowledgement transaction
	//1 - acknowledgement transaction
	char ack;
	//key
	unsigned long key;
	//length of the value char stream
	size_t value_length;
	//value
	char *value;
};

typedef struct transaction transaction;

//functions
//function to build a transaction
transaction *build_transaction (char op, char ack, unsigned long key, size_t  value_length, char *value);
//function to free memory occupied by a transaction
void free_transaction (transaction *t);
//function to convert a transaction into a byte stream
char *serialize_transaction (transaction *t);
//function to construct a transaction from a byte stream
transaction *deserialize_transaction (char *buffer);

#endif
