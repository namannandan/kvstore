#include "storage.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#define INITIAL_SIZE 1024

//record
typedef struct {
	unsigned long key;
	size_t value_length;
	char *value;
} record;

//hash table (array of record pointers)
static record **hash_table = NULL;

//offsets for pseudo random probing (array on integers)
static unsigned long *offset = NULL;

//size of the hash table
static size_t table_size = INITIAL_SIZE;

void st_init() {
	unsigned long i = 0;
	unsigned long index1 = 0;
	unsigned long index2 = 0;
	//initalize hash table
	if ((hash_table = (record **)malloc(sizeof(record *)*table_size))==NULL) {
		perror("malloc failed: ");
		exit(1);
	}
	//initialize offset indices
	if ((offset = (unsigned long *)malloc(sizeof(unsigned long)*table_size))==NULL) {
		perror("malloc failed: ");
		exit(1);
	}
	//initialize the offset array and hash_table array
	for(i=0; i<table_size; i++) {
		offset[i] = i;
		hash_table[i] = NULL;
	}
	//shuffle the elements of the offset array
	for(i=0; i<100000; i++) {
		index1 = rand()%table_size;
		index2 = rand()%table_size;
		//swap the values
		offset[index1] = offset[index1] ^ offset[index2];
		offset[index2] = offset[index1] ^ offset[index2];
		offset[index1] = offset[index1] ^ offset[index2];
	}
}

int st_put(unsigned long key, char *value, size_t value_length) {
	char *val = NULL;
	unsigned long i = 0;
	unsigned long index = -1;
	//allocate space to store the value
	if((val = malloc(sizeof(char)*value_length))==NULL) {
		perror("malloc failed: ");
		return(1);
	}
	//copy the value
	memcpy(val, value, value_length);
	//look for a place in the hash table to store the data
	for (i=0; i<table_size; i++) {
		//hash function to compute index
		index = (key + offset[i])%table_size;
		//check if slot is empty 
		if (hash_table[index] == NULL) {
			//allocate space for a new record
			record *r;
			if ((r = (record *)malloc(sizeof(record)))==NULL) {
				perror("malloc failed: ");
				return (0);
			}
			//store the new record
			r->key = key;
			r->value_length = value_length;
			r->value = val;
			//add the record into the hash table
			hash_table[index] = r;
			//return
			return(1);
		}
		//check if the existing element has the same key
		else if (hash_table[index]->key == key) {
			//replace the existing value
			free(hash_table[index]->value);
			hash_table[index]->value = val;
			return (1);
		}
		else {
			//there is some other record that is already present
			continue;
		}
	}
	//if we reached this point, it means that the hash table is full
	return (0);
}

char *st_get(unsigned long key, size_t *value_length) {
	char *val = NULL;	
	unsigned long i = 0;
	unsigned long index = -1;
	//check the hash table to see if the key exists
	for (i=0; i<table_size; i++) {
		//compute the index
		index = (key + offset[i])%table_size;
		//check if there is no enty in the table
		if (hash_table[index] == NULL) {
			//the record we are looking for is not present in the table
			*value_length = 0;
			return (NULL);
		}
		//check hash table to see if the key we are looking for is present
		else if(hash_table[index]->key == key) {
			//we have found the record we are looking for
			//copy the value and return
			if ((val = malloc(sizeof(char)*(hash_table[index]->value_length)))==NULL) {
				perror("malloc failed: ");
				return (NULL);
			}
			//copy the data to val
			memcpy(val, hash_table[index]->value, hash_table[index]->value_length);
			//set length
			*value_length = hash_table[index]->value_length;
			return (val);
		}
		else {
			//some other record is present in this slot
			continue;
		}
	}
	//if we reach this point, it means that there are no matching entry in the table
	*value_length = 0;
	return (NULL);
}

/*
int st_delete() {

} */
