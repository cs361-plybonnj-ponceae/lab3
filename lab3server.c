/*
    CS 361 Lab 3
    lab3server.c
    Name: Nic Plybon & Adrien Ponce
    Honor Code Statement: This code complies with the JMU Honor Code
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include "network.h"

int main(int argc, char *argv[]) {

    uint16_t req_listen_port;           // port number to listen for requests

    // check for valid number of arguments
    if (argc != 2) {
        printf("Usage: %s listen_port\n", argv[0]);
        exit(0);
    }
    // try to convert argv[1] to requests port number
    req_listen_port = (uint16_t) (strtol(argv[1], NULL, 0) & 0xffff);
    if (req_listen_port == 0) {
        printf("Invalid requests port: %s\n", argv[1]);
        exit(1);
    }

    // netstat -l -p | less

    // creating socket file descriptor
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Error creating socket: %s\n", strerror(errno));
        exit(1);
    }

    // populating server information
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(req_listen_port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind the socket with the server address
    if(bind(sockfd, (struct sockaddr *) &server_address, sizeof(struct sockaddr_in)) < 0) {
        printf("Error binding to socket: %s\n", strerror(errno));
        exit(1);
    }

    // socket address used to store client address
	struct sockaddr_in client_address;
	int client_address_len = 0;

	// receive messages endlessly
	while (1) {
        struct dr_short_msg message;
		// receive content into message struct from client
		int len = recvfrom(sockfd, &message, sizeof(message), 0, (struct sockaddr *)&client_address, &client_address_len);
        if (len < 0) {
            printf("Error receiving: %s\n", strerror(errno));
            exit(1);
        }
		// print log messages following format .. size .. ip:port
		printf("Received message of size %d bytes from %s:%d\n", len,
		       inet_ntoa(client_address.sin_addr), req_listen_port);
        
        // check for valid magic and type 
        if (message.dr_magic == DR_MAGIC && message.dr_type == MSG_REQUEST) {
            printf("\tReceived valid message\n");
            // construct reply back to the client
            struct dr_short_msg reply;
            reply.dr_magic = DR_MAGIC;
            reply.dr_type = MSG_ACCEPT;
            memcpy(reply.dr_map_entry, message.dr_map_entry, sizeof(message.dr_map_entry));
            // send the reply to the client
		    sendto(sockfd, &reply, len, 0, (struct sockaddr *)&client_address,
		        sizeof(client_address));
        } else {
            // output for invalid magic and types
            if (message.dr_magic != DR_MAGIC) {
                printf("Received invalid magic value: %x", message.dr_magic);
            } else if (message.dr_type != MSG_REQUEST) {
                printf("Received invalid message type: %d", message.dr_type);
            }
        }	
    }
    return 0;
}
