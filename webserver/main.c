# include <stdio.h>
# include <string.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "socket.h"
#include "const.h"
#include "signals.h"

// int main(int argc, char **argv)
int main(void)
{
	
	int client_socket, server_socket;

	/* Création de la socket serveur sur le port (8080) */
	server_socket = creer_serveur(PORT);
	if(server_socket == -1)
	{
		return 1;
	}
	
	/* Initialisation des signaux de retour */
	initialiser_signaux();
	
	
	while(1)
	{
		/* Acceptation d'un client */
		client_socket = accept(server_socket, NULL, NULL);
		if (client_socket == -1)
		{
			perror("accept socket client");
			return 1;
		}
		
		/* Nouveau processus de traitement du client */
		if(fork() == 0) {
			traitement_requete(client_socket);
		}
		else
		{
			close(client_socket);
		}
	}

	return 0;
}
