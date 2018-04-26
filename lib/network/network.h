#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <pthread.h>

pthread_t nw_init(pthread_mutex_t *storage_lock);
int nw_put(unsigned long key, char *value, size_t value_length, pthread_mutex_t *network_lock);
char *nw_get(unsigned long key, size_t *value_length, pthread_mutex_t *network_lock);

#endif
