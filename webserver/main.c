#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/mman.h>
#include <stdlib.h>


#include "config.h"
#include "socket.h"
#include "const.h"
#include "signals.h"
#include "strtool.h"
#include "filehandler.h"
#include "stats.h"


static int* in_maintenance = NULL;
static config_map* map;


config_map* get_config_map() {
	return map;
}

int* get_in_maintenance() {
	return in_maintenance;
}

//int main(int argc, char** argv)
int main()
{
	int port;
	map = mmap(NULL, sizeof(map), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	load_config(map);
	get_port(map, &port);

	char *root_dir = get(map, "root_directory");
	
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

	in_maintenance = mmap(NULL, sizeof(in_maintenance), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	init_stats();
	/* Création de la socket serveur sur le port (8080) */
	server_socket = creer_serveur(port);
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
				traitement_requete(client_socket, root_dir);
			}
			else
			{
				close(client_socket);
			}
		}
	} 
	else 
	{
		close(server_socket);
		while(1) 
		{
			/* Traitement des commandes sur le serveur */
			char command[255];
			printf("> ");
			scanf("%s", command);
			
			traitement_commande(command, pid);

		}
	}

	return 0;
}


void traitement_commande(char* command, pid_t pid) {
	if(strcmp(command, "quit") == 0)
	{
		kill(pid, 1);
		exit(0);
	}
	else if(strcmp(command, "maintenance_on") == 0)
	{
		maintenance_on(in_maintenance);
	}
	else if(strcmp(command, "maintenance_off") == 0)
	{
		maintenance_off(in_maintenance);
	}
	else if(strcmp(command, "maintenance") == 0)
	{
		maintenance(in_maintenance);
	}
	else if(strcmp(command, "help") == 0)
	{
		help();
	} else {
		forNoob();
	}
}


void maintenance_on(int* in_maintenance) {
	if(*in_maintenance != 1)
	{
		printf("Server is now under maintenance\n");
		*in_maintenance = 1;
	}
	else
	{
		printf("Server is already under maintenance\n");
	}
}

void maintenance_off(int* in_maintenance) {
	if(*in_maintenance != 0)
	{
		printf("Server is no longer under maintenance\n");
		*in_maintenance = 0;
	}
	else
	{
		printf("Server is not under maintenance\n");
	}
}

void maintenance(int* in_maintenance) {
	if(*in_maintenance == 0)
	{
		printf("Server is not under maintenance\n");
	}
	else
	{
		printf("Server is under maintenance\n");
	}
}

void forNoob() {
	printf("Unrecognized command, type help to display the list of available commands\n");
}

void help() {
	printf("maintenance : display maintenance status\nmaintenance_on : put the server under maintenance\nmaintenance_off : put the server off maintenance\nquit : quit the server\n");
}