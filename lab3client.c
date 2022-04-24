/*
    CS 361 Lab 3
    lab3client.c
    Name: Nic Plybon & Adrien Ponce 
    Honor Code Statement: This code complies with the JMU Honor Code
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "network.h"
#include <unistd.h>


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
    struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;


    inet_pton(AF_INET, argv[1], &server_address.sin_addr);
    server_address.sin_port = htons(server_port);

    int sockfd;
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("could not create socket\n");
		return 1;
	}

    const char* data_to_send = argv[3];
    // send data
	int len =
	    sendto(sockfd, data_to_send, strlen(data_to_send), 0,
	           (struct sockaddr*)&server_address, sizeof(server_address));

	// received echoed data back
	char buffer[100];
	recvfrom(sockfd, buffer, len, 0, NULL, NULL);

	buffer[len] = '\0';
	printf("recieved: '%s'\n", buffer);

	// close the socket
	close(sockfd);






    return 0;

}

