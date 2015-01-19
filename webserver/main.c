# include <stdio.h>
# include <string.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "socket.h"

// int main(int argc, char **argv)
int main(void)
{
	char buff[256];
	int client_socket, server_socket, fd_message, length;
	//const char* welcoming_message = "zeriohhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr\nrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr";
	
	
	server_socket = creer_serveur(8080);
	if(server_socket == -1)
	{
		return 1;
	}
	
	while(1)
	{
		client_socket = accept(server_socket, NULL, NULL);
				
		if (client_socket == -1)
		{
			perror("accept socket client");
			return 1;
		}
		sleep(1);
		
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

	return 0;
}
