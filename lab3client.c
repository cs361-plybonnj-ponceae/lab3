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
#include <errno.h>



int main(int argc, char *argv[]) {

    uint16_t server_port;
    int message_length;

    // check for valid number of arguments
    if (argc != 4) {
        printf("Usage: %s server_ip_address server_port message\n", argv[0]);
        exit(0);
    }

    // error checking for port
    server_port = (uint16_t) (strtol(argv[2], NULL, 0) & 0xffff);
    if (server_port == 0) {
        printf("%s: Invalid server port: %s\n", argv[0], argv[2]);
        exit(1);
    }

    // ERROR CHECK FOR IP ADDRESS HERE

    // error checking for message length 
    message_length = strlen(argv[3]);
    if (message_length > 15) {
       printf("%s: Message length too big: %d characters. Please limit message to at most 15 characters\n", argv[0], message_length);
       exit(1);
    }

    // populating server information
    struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &server_address.sin_addr);
    server_address.sin_port = htons(server_port);

    // bind the socket with the server address
    int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Error creating socket: %s\n", strerror(errno));
        exit(1);
	}

    // construct a message to send to the server
    struct dr_short_msg message;
    message.dr_magic = DR_MAGIC;
    message.dr_type = MSG_REQUEST;
    char echo_message[message_length];
    // make the char array into a string
    strncpy(echo_message, argv[3], message_length);
    // append a null terminator
    echo_message[message_length] = '\0';
    memcpy(message.dr_map_entry, echo_message, message_length);

    // send message to the server
	int len = sendto(sockfd, &message, sizeof(message), 0, (struct sockaddr*)&server_address, sizeof(server_address));
    if (len < 0) {
        printf("Error sending message: %s\n", strerror(errno));
        exit(1);
    }
	// received echoed data back
    struct dr_short_msg reply;
	int rec = recvfrom(sockfd, &reply, len, 0, NULL, NULL);
    if (rec < 0) {
        printf("Error receiving message: %s\n", strerror(errno));
        exit(1);    
    }
    // error check for invalid magic or invalid type
    if (reply.dr_magic != DR_MAGIC || reply.dr_type != MSG_ACCEPT) {
        printf("Invalid reply\n");
    } else {
        printf("%s\n", reply.dr_map_entry);
    }   
	// close the socket
	close(sockfd);
    return 0;
}