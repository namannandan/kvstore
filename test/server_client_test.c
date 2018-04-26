#include <stdio.h>
#include <stdlib.h>
#include "kvstore.h"
#include <stddef.h>

int main()
{
	char *buf;
	size_t buf_len;
	printf("client-server test\n\n");
	kvstore_init();
	printf("storing data directly to hash table\n");
	printf("key:1234, value:hello world\n");	
	put(1234, "hello world", 11);
	printf("retrieving stored data directly from hash table\n");
	buf = get(1234, &buf_len);
	printf("key:1234, value:");
	fwrite(buf, sizeof(char), buf_len, stdout);
	printf("\n\n");
	free(buf);

	printf("loopback (client->server->hash_table, hash_table->server->client) test\n\n");
	printf("storing data to hash table via client->server->hash_table\n");
	printf("key:12345, value:cool stuff\n");	
	put(12345, "cool stuff", 10);
	printf("retrieving stored data from hash table (hash_table->server->client)\n");
	buf = get(12345, &buf_len);
	printf("key:12345, value:");
	fwrite(buf, sizeof(char), buf_len, stdout);
	printf("\n");
	free(buf);
	exit(0);
}
