#ifndef __STRTOOL_H__
#define __STRTOOL_H__

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

int is_valid_request(const char* request);
int is_valid_line(char* line);
int filelen(int fd);
int parse_http_request(const char *request_line , http_request *request);

#endif
