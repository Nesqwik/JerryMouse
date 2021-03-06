#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include "strtool.h"

char* get_type(char* url)
{
	if (strstr(url, ".html") != NULL) 
		return "Content-Type: text/html\r\n";

	if (strstr(url, ".css") != NULL) 
		return "Content-Type: text/css\r\n";

	if (strstr(url, ".js") != NULL) 
		return "Content-Type: applicaton/javascript\r\n";

	if (strstr(url, ".png") != NULL) 
		return "Content-Type: image/png\r\n";

	if (strstr(url, ".jpg") != NULL) 
		return "Content-Type: image/jpeg\r\n";

	if (strstr(url, ".jpe") != NULL) 
		return "Content-Type: image/jpeg\r\n";

	if (strstr(url, ".jpeg") != NULL) 
		return "Content-Type: image/jpeg\r\n";

	if (strstr(url, ".gif") != NULL) 
		return "Content-Type: image/gif\r\n";

	if (strstr(url, ".bmp") != NULL) 
		return "Content-Type: image/x-mx-bmp\r\n";

	if (strstr(url, ".avi") != NULL) 
		return "Content-Type: video/x-msvideo\r\n";

	if (strstr(url, ".mp4") != NULL) 
		return "Content-Type: video/mp4\r\n";

	if (strstr(url, ".mp3") != NULL) 
		return "Content-Type: audio/mpeg\r\n";

	if (strstr(url, ".wav") != NULL) 
		return "Content-Type: audio/x-wav\r\n";

	if (strstr(url, ".ogg") != NULL) 
		return "Content-Type: audio/ogg\r\n";

	if (strstr(url, ".json") != NULL) 
		return "Content-Type: applicaton/json\r\n";

	if (strstr(url, ".xml") != NULL) 
		return "Content-Type: applicaton/xml\r\n";

	return "Content-Type: text/plain\r\n";
}

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

	if (strstr(req.url, "../") != NULL)
		return 400;
	
	free(req.url);
	
	if(req.minor_version != 1 && req.minor_version != 0)
	  return 505;
	
	return 200;
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
			return url;
		}
		i++;
	}

	if (url[i - 1] == '/')
	{
		strcat(url, "index.html");
	}

	return url;
}	
