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

char* get_type(char* url);
int is_valid_request(const char* request, char* url);
int is_valid_line(char* line);
int get_file_size(int fd);
int copy(int in, int out);
int parse_http_request(const char *request_line , http_request *request);
char *rewrite_url(char *url);
#endif
