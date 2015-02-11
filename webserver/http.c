#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "http.h"
#include "strtool.h"

void send_400_error(int fd_socket) {
	
	const char* error_400 = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n";
	if (write(fd_socket, error_400, strlen(error_400)) == -1)
	{
		perror("write message");
		exit(1);
	}
}



void send_200_response(int fd_socket) {
	
	char buff[255];
	
	int nb_read = 0;
	
	
	int fd_message = open("message", O_RDONLY);
	if (fd_message == -1)
	{
		perror("open message");
		exit(1);
	}
	
	
	int message_len = filelen(fd_message);
	sprintf(buff, "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: %d\r\n\r\n", message_len);
	if (write(fd_socket, buff, strlen(buff)) == -1)
	{
		perror("write message");
		exit(1);
	}
	
	
	
	while ((nb_read = read(fd_message, buff, sizeof(buff))) > 0)
	{
		printf("%d\n", nb_read);
		if (write(fd_socket, buff, nb_read) == -1)
		{
			perror("write message");
			exit(1);
		}
	}
	
}