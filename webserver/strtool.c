#include <stdio.h>

#include "strtool.h"


/* Parse la requête HTTP pour être conforme à la forme voulue */
int parse_request(char* request, char* url, int* version_m)
{
	int ret = sscanf(request, "GET %s HTTP/1.%d", url, version_m);
	if(ret != 2) {
		return -1;
	}
	
	return 0;
}

/* Vérifie si la requête HTTP est valide (version supportée) */
int is_valid_request(char* request) {
	//char method[10];
	char url[255];
	//int version_M = 0;
	int version_m = 0;
	
	if(parse_request(request, url, &version_m) == -1)
		return -1;
	
	/*if(strcmp(method, "GET") != 0)
		return -1;
	
	if(version_M != 1)
		return -1;*/
	
	if(version_m != 1 && version_m != 0)
		return -1;
	
	return 0;
}