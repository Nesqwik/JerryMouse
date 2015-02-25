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
	sprintf(response, "Connection: close\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n%s\r\n", message_len, message_body);
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

<<<<<<< HEAD

void increment_stats(int code) {
	web_stats* stats = get_stats();
	switch(code) {
		case 200:
			stats->ok_200 += 1;
			break;

		case 400:
			stats->ko_400 += 1;
			break;

		case 403:
			stats->ko_403 += 1;
			break;

		case 404:
			stats->ko_404 += 1;
			break;

		case 0:
			stats->served_connections += 1;

		case 1:
			stats->served_requests += 1;
	}
}
=======
void send_stats(FILE* client)
{
	send_response(client, 200, "OK", "Statstatstats");

	exit(0);
}
>>>>>>> 7c978bbf33af4178ae52a75803d0f1d43379e76e
