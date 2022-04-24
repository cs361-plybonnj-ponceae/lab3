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

    // netstat -l -p | less
    // nc -u 127.0.0.1 port msg

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

	// run indefinitely
	while (1) {
		char buffer[500];

		// read content into buffer from an incoming client
		int len = recvfrom(sockfd, buffer, sizeof(buffer), 0,
		                   (struct sockaddr *)&client_address,
		                   &client_address_len);

		// inet_ntoa prints user friendly representation of the
		// ip address
		buffer[len] = '\0';
		printf("received: '%s' from client %s\n", buffer,
		       inet_ntoa(client_address.sin_addr));

		// send same content back to the client ("echo")
		sendto(sockfd, buffer, len, 0, (struct sockaddr *)&client_address,
		       sizeof(client_address));
    }
    return 0;

}
