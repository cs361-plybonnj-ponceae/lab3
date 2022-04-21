/*

    CS 361 Lab 3
    lab3client.c

    Name: 

    Honor Code Statement: 

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "network.h"

int main(int argc, char *argv[]) {

    uint16_t server_port;
    int message_length;

    if (argc != 4) {
        printf("Usage: %s server_ip_address server_port message\n", argv[0]);
        exit(0);
    }

    server_port = (uint16_t) (strtol(argv[2], NULL, 0) & 0xffff);
    if (server_port == 0) {
        printf("%s: Invalid server port: %s\n", argv[0], argv[2]);
        exit(1);
    }

    message_length = strlen(argv[3]);
    if (message_length > 15) {
       printf("%s: Message length too big: %d characters. Please limit message to at most 15 characters\n", argv[0], message_length);
       exit(1);
    }

    /* Implement the remaining client code here */

    return 0;

}

