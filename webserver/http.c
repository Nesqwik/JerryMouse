#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "http.h"
#include "strtool.h"

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


void send_header(FILE* client, int code, const char* reason_phrase, int fd_message, char* type)
{
	int message_len = get_file_size(fd_message);
	char response[1024];

	sprintf(response, "Connection: close\r\n%sContent-Length: %d\r\n\r\n", type, message_len);
	send_status(client, code, reason_phrase);

	printf("%s", response);
	if (fwrite(response, strlen(response), 1, client) == 0)
	{
		perror("write status");
		exit(1);
	}
	fflush(client);
}
