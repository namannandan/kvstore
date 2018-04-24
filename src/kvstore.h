#ifndef KVSTORE_H_INCLUDED
#define KVSTORE_H_INCLUDED

#include <stddef.h>

//function prototypes
void kvstore_init();
int put(unsigned long key, char *value, size_t value_length);
char* get(unsigned long key, size_t* value_length);

#endif
