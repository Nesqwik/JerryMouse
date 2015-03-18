#include <stdio.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "socket.h"
#include "const.h"
#include "signals.h"
#include "strtool.h"
#include "filehandler.h"
#include "stats.h"
#include "config.h"

//int main(int argc, char** argv)
int main()
{
	int port;
	config_map map;
	load_config(&map);
	get_port(&map, &port);

	char *root_dir = get(&map, "root_directory");
	
	if (root_dir == NULL) 
	{
		root_dir = ".";
		printf("Couldn't find a root directory in configuration file, lauching server with default root directory .\n");
	}

	if (check_root_dir(root_dir) == -1) 
	{
		printf("Couldn't launch server with provided root directory\n");
		return 1;
	}
	else
	{
		printf("Launched server with root directory %s\n", root_dir);
	}

	int client_socket, server_socket;

	init_stats();
	/* Création de la socket serveur sur le port (8080) */
	server_socket = creer_serveur(port);
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
			traitement_requete(client_socket, root_dir);
		}
		else
		{
			close(client_socket);
		}
	}

	return 0;
}
