# include <stdio.h>
# include <string.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "socket.h"
#include "const.h"
#include "signals.h"

// int main(int argc, char **argv)
int main(void)
{
	char buff[256];
	int client_socket, server_socket, fd_message, length;
	
	
	server_socket = creer_serveur(PORT);
	if(server_socket == -1)
	{
		return 1;
	}
	
	initialiser_signaux();
	
	while(1)
	{
		client_socket = accept(server_socket, NULL, NULL);
				
		if (client_socket == -1)
		{
			perror("accept socket client");
			return 1;
		}
		
		if(fork() == 0) {
			fd_message = open("message", O_RDONLY);
			if (fd_message == -1)
			{
				perror("open message");
				return -1;
			}
			
			
			while ((length = read(fd_message, &buff, sizeof(buff))) != 0)
			{
				write(client_socket, &buff, length);
			}
			
			while ((length = read(client_socket, &buff, sizeof(buff))) != 0)
			{
				write(client_socket, &buff, length);
			}
		}
		else
		{
			close(socket_client);
		}
	}

	return 0;
}
