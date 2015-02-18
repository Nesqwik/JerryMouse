#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include "strtool.h"

/* Parse la requête HTTP pour être conforme à la forme voulue  
retourne -1 si elle est invalide et 0 sinon */
/*int parse_request(char* request, char* url, int* version_m)*/
int parse_http_request(const char *request_line , http_request *request)
{
	char method[256];
	char* url = malloc(256);
	int ret = sscanf(request_line, "%s %s HTTP/%d.%d", method, url, &(request->major_version), &(request->minor_version));

	request->url = url;

	if(ret != 4) 
	{
		return -1;
	}
	if (strcmp(method, "GET") == 0) 
	{
		request->method = HTTP_GET;
	}
	else 
	{
		request->method = HTTP_UNSUPPORTED;
	}
	
	return 0;
}

/* Vérifie si la requête HTTP est valide (version supportée) et renvoie le statut de la réponse*/
int is_valid_request(const char* request, char* url) 
{
	http_request req;
	
	if(parse_http_request(request, &req) == -1)
		return 400;
	
	if(req.method != 0)
		return 405;
	
	if(req.major_version != 1)
		return 505;	

	strcpy(url, req.url);

	free(req.url);
	
	if(req.minor_version != 1 && req.minor_version != 0)
	  return 505;
	
	return 200;
}

int is_valid_file(char* url) 
{
	return access(url, F_OK);
}


/* Retourne le nombre de caractères lus sur la ligne 
   ou -1 si la ligne ne se termine pas par \r\n */
int is_valid_line(char* line) 
{
	char buf[255];
	if (sscanf(line, "%s\r\n", buf) != 1) 
	{
		return -1;
	}
	return strlen(buf);
}


/* Retourne la taille du fichier passé en paramètre */
int get_file_size(int fd)
{
	struct stat stats;
	fstat(fd, &stats);
    
	return stats.st_size;
}


int copy(int in, int out)
{
	char buff[256];
	int size = 0;
	while((size = read(in, buff, sizeof(buff)))!= 0)
	{

		if(size == -1)
		{
			perror("cpy-read");
			return -1;
		}

		if(write(out, buff, size) == -1) 
		{
			perror("wirte");
			return -1;
		}
	}


	char* crlf = "\r\n\r\n";
	if(write(out, crlf, strlen(crlf)) == -1) 
	{
		perror("wirte");
		return -1;
	}


	return 0;
}

char *rewrite_url(char *url)
{
	int i = 0;
	
	while (url[i] != 0) 
	{
		if (url[i] == '?') 
		{		
			url[i] = 0;
			break;
		}
		i++;
	}
	return url;
}	
