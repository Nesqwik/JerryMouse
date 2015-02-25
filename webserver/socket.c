#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
# include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "strtool.h"
#include "socket.h"
#include "http.h"
#include "filehandler.h"
#include "stats.h"


int creer_serveur (int port)
{
	int optvalue = 1;
	struct sockaddr_in saddr;
	
	/* Création de la socket server */
	int socket_server = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_server == -1)
	{
		perror("socket server");
		return -1;
	}
	
	/* Remplissage de la structure de socket server */
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = INADDR_ANY;
	
	/* option de la socket serveur (pour le rebind sur le même port) */
	if(setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(int)) == -1)
	{
		perror("socket option error");
		return -1;
	}
	
	/* Bind de la socket */
	if (bind(socket_server, (struct sockaddr *) &saddr, sizeof(saddr)) == -1)
	{
		perror("bind socket_server");
		return -1;
	}
	
	/* Mise en écoute de la socket server */
	if(listen(socket_server, 10) == -1)
	{
		perror("listen socket_server");
		return -1;
	}
	
	return socket_server;
}

char* fgets_or_exit (char* buffer, int size, FILE* stream)
{
	if (fgets(buffer, size, stream) == NULL)
	{
		perror("fgets");
		exit(1);
	}
	return buffer;
}

void traitement_requete(int client_socket, char* root_directory) 
{
	char buff[256];
	char url[256];
	FILE* client;
	int status;
	int fd_ressource = 0;
	char* type;


	increment_stats(0);

	
	/* On ouvre la socket et on associe son contenu a client */
	client = fdopen(client_socket, "w+");
	if (client == NULL)
	{
		perror("fdopen socket");
		exit(1);
	}

	/* On lit l'en-tete de la requete */
	fgets_or_exit(buff, sizeof(buff), client);

	/* On passe les headers non supportés */
	skip_headers(client);
	
	printf("%s", buff);

	status = is_valid_request(buff, url);
	
	if (status == 200)
	{
		if(strcmp(rewrite_url(url), "/stats") == 0) {
			increment_stats(1);
			increment_stats(status);
			send_stats(client);
		}

		fd_ressource = check_and_open(rewrite_url(url), root_directory);

		if(fd_ressource == -1)
			status = 404;
		type = get_type(url);
	}
	

	increment_stats(1);
	increment_stats(status);

	/* On vérifie la validité de l'en-tête */
	if (status == 505) {
		send_response(client, status, "HTTP Version Not Supported", "HTTP Version Not Supported\r\n");
	}
	if (status == 404) {
		send_response(client, status, "Not Found", "Not Found\r\n");
	} 
	if (status == 405) {
		send_response(client, status, "Method Not Allowed", "Method Not Allowed\r\n" );
	}
	if(status == 200) {
		send_header(client, status, "OK", fd_ressource, type);
		copy(fd_ressource, client_socket);
		exit(0);
	}

	send_response(client, 400, "Bad Request", "Bad Request\r\n");
}

void skip_headers(FILE* client)
{
	char buff[256];
	while (strcmp(buff, "\r\n") != 0 && strcmp(buff, "\n") != 0)
	{
		//printf("%s", buff);
		fgets_or_exit(buff, sizeof(buff), client);
	}
}
