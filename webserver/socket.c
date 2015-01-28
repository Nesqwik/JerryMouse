#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
# include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "socket.h"


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


void traitement_requete(int client_socket) {
	char buff[256];
	int fd_message, length;
	
	/* Ouverture du message de bienvenue */
	fd_message = open("message", O_RDONLY);
	if (fd_message == -1)
	{
		perror("open message");
		exit(1);
	}
	
	/* Lecture du message de bienvenue */
	while ((length = read(fd_message, &buff, sizeof(buff))) != 0)
	{
		if (write(client_socket, &buff, length) == -1)
		{
			perror("write message");
			exit(1);
		}
	}
	
	/* Lecture et renvoie des messages du client */
	while ((length = read(client_socket, &buff, sizeof(buff))) != 0)
	{
		if (write(client_socket, &buff, length) == -1)
		{
			perror("write message");
			exit(1);
		}
	}
	
	exit(0);
}
