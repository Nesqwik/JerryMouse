#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "http.h"
#include "strtool.h"
#include "stats.h"

void send_status(FILE* client, int code, const char* reason_phrase)
{
	char status[256];
	sprintf(status, "HTTP/1.1 %d %s \r\n", code, reason_phrase);
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
	sprintf(response, "Connection: close\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n%s\r\n", message_len, message_body);
	send_status(client, code, reason_phrase);

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

	if (fwrite(response, strlen(response), 1, client) == 0)
	{
		perror("write status");
		exit(1);
	}
	fflush(client);
}

void send_stats(FILE* client)
{

	char buff[256];
	web_stats* stats = get_stats();
	sprintf(buff, "Connexions au serveur : %d\nReponses aux requetes : %d\nOK 200 : %d\nErreur 400 : %d\nErreur 403 : %d\nErreur 404 : %d\n",
		stats->served_connections, stats->served_requests, stats->ok_200, stats->ko_400, stats->ko_403, stats->ko_404);
	send_response(client, 200, "OK", buff);

	exit(0);
}
