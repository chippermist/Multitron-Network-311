#define _POSIX_C_SOURCE 200809L
#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "nettron.h"

int socket_fd = -1;
/*
 * Networked tronctl.  Uses the MultiTron message protocol to communicate with a
 * MultiTron server.
 *
 * Parameters:
 *   sock     File descriptor of a socket which is connected to the server.
 *            This parameter is passed by reference.  If the descriptor is
 *            negative, this function should connect to the default server/port
 *            and store the socket descriptor here before sending any data.
 *
 *   instr    MultiTron instruction (the parameter formerly known as "op").  The
 *            format of this parameter is exactly the same as in previous
 *            assignments.
 *
 *   data     Data buffer for the operation.  This parameter should function
 *            exactly as in previous assignments.
 *
 *   datalen  Size in bytes of the data buffer.
 *
 * Return value: 0 for success, nonzero for failure
 */


struct message_format
{
	/* payload initial data */
	uint32_t instr;
	uint16_t no_of_bytes;
	uint16_t return_value;
};

int tronctl_net(int *sock, uint32_t instr, void *data, int datalen)
{
	
	// printf("value: %x\n", *sock);

	struct message_format *message = NULL;

	// printf("value: %x\n", *sock);

	// printf("Data Length: %d bytes\n", datalen);

	if(*sock < 0)
	{
		// printf("Socket not set \n");

		struct sockaddr_in caddr;

		//setting the family to IPv4
		caddr.sin_family = AF_INET;

		//setting the address for the socket to the default address		
		if(inet_aton(MULTITRON_DEFAULT_IP, &(caddr.sin_addr)) == 0)
		{
			perror("Setting IP\n");
			return -1;
		}
		
		// printf("Port: %d\n", (MULTITRON_DEFAULT_PORT));

		//setting the port for the socket to the default port
		caddr.sin_port = htons(MULTITRON_DEFAULT_PORT);

		//setting the socket file descriptor
		socket_fd = socket(AF_INET, SOCK_STREAM, 0);

		//checking if the file descriptor is set
		if(socket_fd == -1)
		{
			perror("Socket File Descriptor\n");
			return -1;
		}

		//connecting the socket to the file descriptor
		if(connect(socket_fd, (const struct sockaddr *)&caddr, sizeof(struct sockaddr)) != 0)
		{
			perror("Connecting Socket\n");
			return -1;
		}

		//assigning the new socket file descriptor to the socket for later use
		*sock = socket_fd;
	}

	// printf("Socket Connected\n");

	uint16_t value_return = 0x00;

	// printf("Values Defined\n");

	message = malloc(8);

	message->instr = htonl(instr);
	message->no_of_bytes = htons(datalen);
	message->return_value = htons(value_return);

	// printf("Values Converted\n");

	//writing the instruction header to the socket
	//writing it into the struct
	if(write(socket_fd, message, 8) != 8)
	{
		perror("Writing instruction header\n");
		return -1;
	}
	
	//writing the data if the datalength of the data is more than 0
	//checking if the written data is of datalen
	if(datalen > 0)
	{
		if(write(socket_fd, data, datalen) != datalen)
		{
			perror("Writing data\n");
			return -1;
		}
	}

	// printf("Payload Written to socket\n");
	// printf("Data Length: %d\n", datalen);

	//reading in the intructor header from the socket
	//pass error if not read equal to 8 bytes
	if(read(socket_fd, message, 8) != 8)
	{
		perror("Reading instruction header\n");
		return -1;
	}


	// printf("Payload initialization read\n");

	//reconverting the values from 
	message->instr = ntohl(message->instr);
	message->no_of_bytes = ntohs(message->no_of_bytes);
	message->return_value = ntohs(message->return_value);

	// printf("Values Reconverted\n");
	// printf("Data Length Recieved: %d\n", message->no_of_bytes);

	// printf("Return Value: %d\n", message->return_value);

	//checking the return value
	//0 if successful
	//anything else if unsucessful
	if(message->return_value != 0)
	{
		// perror("Return value is not zero. The program did not succeed.\n");
		return -1;
	}

	//checking the number of bytes to be read.
	//then reading the data form the socket file descriptor
	if(message->no_of_bytes > 0)
	{
		//reading the data from the socket
		if(read(socket_fd, data, message->no_of_bytes) != (message->no_of_bytes))
		{
			perror("Reading data recieved\n");
			return -1;
		}
	}

	// printf("Payload data read\n");

	//freeing the memory allocated for the struct
	free(message);

	return 0;
}
