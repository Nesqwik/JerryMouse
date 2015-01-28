#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
# include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "socket.h"


int creer_serveur (int port)
{
	int optvalue = 1;
	struct sockaddr_in saddr;
	int socket_server = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_server == -1)
	{
		perror("socket server");
		return -1;
	}
	
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = INADDR_ANY;
	
	
	if(setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(int)) == -1)
	{
		perror("socket option error");
		return -1;
	}
	
	
	if (bind(socket_server, (struct sockaddr *) &saddr, sizeof(saddr)) == -1)
	{
		perror("bind socket_server");
		return -1;
	}
	
	if(listen(socket_server, 10) == -1)
	{
		perror("listen socket_server");
		return -1;
	}
	
	return socket_server;
}


int traitement_requete(int client_socket) {
	char buff[256];
	int fd_message, length;
	
	
	fd_message = open("message", O_RDONLY);
	if (fd_message == -1)
	{
		perror("open message");
		return -1;
	}
	
	while ((length = read(fd_message, &buff, sizeof(buff))) != 0)
	{
		if (write(client_socket, &buff, length) == -1)
		{
			perror("write message");
			return -1;
		}
	}
	
	while ((length = read(client_socket, &buff, sizeof(buff))) != 0)
	{
		if (write(client_socket, &buff, length) == -1)
		{
			perror("write message");
			return -1;
		}
	}
	
	return 0;
}
