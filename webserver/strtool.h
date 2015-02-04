#ifndef __STRTOOL_H__
#define __STRTOOL_H__

int parse_request(char* request, char* url, int* version_m);
int is_valid_request(char* request);
int is_valid_line(char* line);
#endif