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

void send_404_error(int fd_socket) {
	
	const char* error_404 = "HTTP/1.1 404 File Not Found\r\nConnection: close\r\nContent-Length: 20\r\n\r\n404 File Not Found\r\n";
	if (write(fd_socket, error_404, strlen(error_404)) == -1)
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
	
	
	int message_len = get_file_size(fd_message);
	sprintf(buff, "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: %d\r\n\r\n", message_len);
	if (write(fd_socket, buff, strlen(buff)) == -1)
	{
		perror("write message");
		exit(1);
	}
	
	
	
	while ((nb_read = read(fd_message, buff, sizeof(buff))) > 0)
	{
		if (write(fd_socket, buff, nb_read) == -1)
		{
			perror("write message");
			exit(1);
		}
	}
	
}



void send_status(FILE* client, int code, const char* reason_phrase)
{
	char status[256];
	sprintf(status, "HTTP/1.1 %d %s \r\n", code, reason_phrase);
	printf("%s", status);
	if (fwrite(status, strlen(status), 1, client) == 0)
	{
		perror("write status");
		exit(1);
	}
}

void send_response(FILE* client, int code, const char* reason_phrase , const char* message_body)
{
	int message_len = strlen(message_body);
	char response[1024];
	sprintf(response, "Connection: close\r\nContent-Length: %d\r\n\r\n%s\r\n", message_len, message_body);
	send_status(client, code, reason_phrase);

	printf("%s", response);
	if (fwrite(response, strlen(response), 1, client) == 0)
	{
		perror("write status");
		exit(1);
	}

	exit(0);
}


void send_header(FILE* client, int code, const char* reason_phrase, int fd_message)
{
	int message_len = get_file_size(fd_message);
	char response[1024];
	sprintf(response, "Connection: close\r\nContent-Length: %d\r\n\r\n", message_len);
	send_status(client, code, reason_phrase);

	printf("%s", response);
	if (fwrite(response, strlen(response), 1, client) == 0)
	{
		perror("write status");
		exit(1);
	}
	fflush(client);
}