#include <stdio.h>

#include "strtool.h"


int parse_request(char* request, char* method, char* url, int* version_M, int* version_m)
{
	return sscanf(request, "%s %s HTTP/%d.%d", method, url, version_M, version_m);
}

/* Retourne le nombre de caractères lus sur la ligne 
   ou -1 si la ligne ne se termine pas par \r\n */
int is_valid_line(char* line) {
  char buf[255];
  if (sscanf(line, "%s\r\n", buf) != 1) {
    return -1;
  }
  return strlen(buf);
}
