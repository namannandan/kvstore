#include <stdio.h>
#include <stdlib.h>
#include "kvstore.h"
#include <stddef.h>

int main()
{
	char *buf;
	size_t buf_len;
	kvstore_init();
	put(1234, "cool stuff", 10);
	buf = get(1234, &buf_len);
	fwrite(buf, sizeof(char), buf_len, stdout);
	printf("\n");
	//free(buf);
	exit(0);
}
