#ifndef __HTTP_H__
#define __HTTP_H__

void send_400_error(int fd_socket);
void send_200_response(int fd_socket) ;
void send_404_error(int fd_socket);
void send_status(FILE* client, int code, const char* reason_phrase);
void send_response(FILE* client, int code, const char* reason_phrase , const char* message_body);

enum http_method {
	HTTP_GET ,
	HTTP_UNSUPPORTED ,
};

typedef struct
{
	enum http_method method;
	int major_version;
	int minor_version;
	char *url;
} http_request;


#endif


