#include "kvstore.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("provide only one argument (node id)\n");
		exit(1);
	}
	unsigned long id = atoi(argv[1]);
	kvstore_init(id);
	//put to sleep indefinitely
	while(1) {
		sleep(10000);
	}
	return (0);
}
