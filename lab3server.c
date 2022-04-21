/*

    CS 361 Lab 3
    lab3server.c

    Name: 

    Honor Code Statement: 

*/

#include <stdio.h>
#include <stdlib.h>

#include "network.h"

int main(int argc, char *argv[]) {

    uint16_t req_listen_port;           // port number to listen for requests


    if (argc != 2) {
        printf("Usage: %s listen_port\n", argv[0]);
        exit(0);
    }

    /* Try to convert argv[1] to requests port number */
    req_listen_port = (uint16_t) (strtol(argv[1], NULL, 0) & 0xffff);
    if (req_listen_port == 0) {
        printf("Invalid requests port: %s\n", argv[1]);
        exit(1);
    }

    /* Implement the remaining server code here */

    return 0;

}
