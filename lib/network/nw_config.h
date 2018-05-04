#ifndef NW_CONFIG_H_INCLUDED
#define NW_CONFIG_H_INCLUDED

//number of supported nodes
#define NUM_NODES 2
//variable to hold the node id
extern unsigned long node_id;
//server ip addresses
extern char *server_ip_addresses[NUM_NODES];
//server port numbers
extern unsigned long server_port_numbers[NUM_NODES];

#endif
