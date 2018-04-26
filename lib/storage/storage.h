#ifndef STORAGE_H_INCLUDED
#define STORAGE_H_INCLUDED

#include <stddef.h>

void st_init();
int st_put(unsigned long key, char *value, size_t value_length);
char *st_get(unsigned long key, size_t *value_length);
/*int st_delete(unsigned long key);
*/
#endif
