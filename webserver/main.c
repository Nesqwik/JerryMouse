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
	int server_socket = creer_serveur(8080);
	while(1)
	{
		int client_socket = accept(server_socket, NULL, NULL);
		const char* welcoming_message = "zeriohhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\nhhhhhhhhhhhhhhhhhhhhhhhhhrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr\nrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr";
		
		if (client_socket == -1)
		{
			perror("accept socket client");
			return 1;
		}

		write(client_socket, welcoming_message, strlen(welcoming_message));
	}

	return 0;
}
