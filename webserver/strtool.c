#include <stdio.h>

#include "strtool.h"


int parse_request(char* request, char* method, char* url, int* version_M, int* version_m)
{
	return sscanf(request, "%s %s HTTP/%d.%d", method, url, version_M, version_m);
}