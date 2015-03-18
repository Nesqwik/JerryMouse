#include <stdio.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/mman.h>


#include "socket.h"
#include "const.h"
#include "signals.h"
#include "strtool.h"
#include "filehandler.h"
#include "stats.h"

static int* in_maintenance = NULL;


int* get_in_maintenance() {
	return in_maintenance;
}

int main(int argc, char** argv)
{
	if (argc < 2) 
	{
		printf("Couldn't launch without a root directory\n");
		return 1;
	}

	if (check_root_dir(argv[1]) == -1) 
	{
		printf("Couldn't launch server with provided root directory\n");
		return 1;
	}
	else
	{
		printf("Launched server with root directory %s\n", argv[1]);
	}

	int client_socket, server_socket;

	in_maintenance = mmap(NULL, sizeof(in_maintenance), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	init_stats();
	/* Création de la socket serveur sur le port (8080) */
	server_socket = creer_serveur(PORT);
	if(server_socket == -1)
	{
		return 1;
	}
	
	/* Initialisation des signaux de retour */
	initialiser_signaux();
	
	
	pid_t pid = fork();
	if(pid == 0) {
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
				traitement_requete(client_socket, argv[1]);
			}
			else
			{
				close(client_socket);
			}
		}
	} 
	else 
	{
		printf("pid : %d\n", pid);
		close(server_socket);
		while(1) 
		{
			/* Traitement des commandes sur le serveur */
			char command[255];
			scanf("%s", command);
			
			if(strcmp(command, "quit") == 0)
			{
				kill(pid, 1);
				return 0;
			}
			else if(strcmp(command, "maintenance_on") == 0)
			{
				printf("Le serveur à été mis en maintenance\n");
				*in_maintenance = 1;
			}
			else if(strcmp(command, "maintenance_off") == 0)
			{
				printf("Le serveur n'est plus en maintenance\n");
				*in_maintenance = 0;
			}

		}
	}

	return 0;
}
