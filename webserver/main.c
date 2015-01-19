# include <stdio.h>
# include <string.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#include "socket.h"

// int main(int argc, char **argv)
int main(void)
{
	char buff[256];
	int client_socket, server_socket;
	const char* welcoming_message = "zeriohhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr\nrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr";
	
	
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
		
		write(client_socket, welcoming_message, strlen(welcoming_message));
		
		while (read(client_socket, &buff, sizeof(buff)) != 0)
		{
			write(client_socket, &buff, strlen(buff));
		}
	}

	return 0;
}
