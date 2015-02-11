#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "strtool.h"


/* Parse la requÃªte HTTP pour Ãªtre conforme Ã  la forme voulue */
int parse_request(char* request, char* url, int* version_m)
{
	int ret = sscanf(request, "GET %s HTTP/1.%d", url, version_m);
	if(ret != 2) {
		return -1;
	}
	
	return 0;
}

/* VÃ©rifie si la requÃªte HTTP est valide (version supportÃ©e) et renvoie le statut de la réponse*/
int is_valid_request(char* request) {
	//char method[10];
	char url[255];
	//int version_M = 0;
	int version_m = 0;
	
	if(parse_request(request, url, &version_m) == -1)
		return 400;
	
	/*if(strcmp(method, "GET") != 0)
		return -1;
	
	if(version_M != 1)
		return -1;*/	

	if (strcmp("/", url) != 0) 
	  return 404;
	
	if(version_m != 1 && version_m != 0)
	  return 400;
	
	return 200;
}


/* Retourne le nombre de caractÃ¨res lus sur la ligne 
   ou -1 si la ligne ne se termine pas par \r\n */
int is_valid_line(char* line) {
	char buf[255];
	if (sscanf(line, "%s\r\n", buf) != 1) {
		return -1;
	}
	return strlen(buf);
}


/* Retourne la taille du fichier passÃ© en paramÃ¨tre */
int filelen(int fd) {
int len = 0;

	len = lseek(fd, 0, SEEK_END);
	
	lseek(fd, 0, SEEK_SET);
     
	return len;
}
