#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "socket.h"


int creer_serveur (int port )
{
	int socket_server = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_server == -1)
	{
		perror("socket server");
	}
	
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(socket_server, (struct sockaddr *) &saddr, sizeof(saddr)) == -1)
	{
		perror("bind socket_server");
	}
	
	return socket_server;
}
