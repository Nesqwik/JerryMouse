#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
# include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "strtool.h"
#include "socket.h"


int creer_serveur (int port)
{
	int optvalue = 1;
	struct sockaddr_in saddr;
	
	/* CrÃ©ation de la socket server */
	int socket_server = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_server == -1)
	{
		perror("socket server");
		return -1;
	}
	
	/* Remplissage de la structure de socket server */
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = INADDR_ANY;
	
	/* option de la socket serveur (pour le rebind sur le mÃªme port) */
	if(setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(int)) == -1)
	{
		perror("socket option error");
		return -1;
	}
	
	/* Bind de la socket */
	if (bind(socket_server, (struct sockaddr *) &saddr, sizeof(saddr)) == -1)
	{
		perror("bind socket_server");
		return -1;
	}
	
	/* Mise en Ã©coute de la socket server */
	if(listen(socket_server, 10) == -1)
	{
		perror("listen socket_server");
		return -1;
	}
	
	return socket_server;
}


void traitement_requete(int client_socket) {
	char buff[256];
	//int fd_message;
	FILE* file;
	int ret;
		
	/*fd_message = open("message", O_RDONLY);
	if (fd_message == -1)
	{
		perror("open message");
		exit(1);
	}

	file = fdopen(fd_message, "r");
	if (file == NULL)
	{
		perror("fdopen message");
		exit(1);
	}
	
	while (fgets(buff, sizeof(buff), file) != NULL)
	{
		if (fwrite(buff, sizeof(buff), 1, file) != 1)
		{
			perror("write message");
			exit(1);
		}
	}*/
	
	/* On ouvre la socket et on associe son contenu à file */
	file = fdopen(client_socket, "w+");
	if (file == NULL)
	{
		perror("fdopen socket");
		exit(1);
	}

	/* On lit l'en-tête de la requête */
	if (fgets(buff, sizeof(buff), file) == NULL) {
	  perror("fgets initial");
	  exit(1);
	}

	printf("%s", buff);

	/* On vérifie la validité de l'en-tête */
	if (is_valid_request(buff) == -1) {
	  printf("en-tête invalide \n");
	  /* retour 400 */
	  exit(1);
	}
	
	/* Lecture et renvoi des messages du client */
	while ((ret = is_valid_line(buff)) > 0)
	{
	  printf("%s", buff);

	  if (fgets(buff, sizeof(buff), file) == NULL) {
	    perror("fgets");
	    exit(1);
	  }
	}
	
	if (ret < 0) {
	  printf("requête invalide format ligne\n");
	  /* retour 400 */
	  exit(1);
	}

	printf("requête valide\n");
	/* retour 200 */
	  
	exit(0);
}
