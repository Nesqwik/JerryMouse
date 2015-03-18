#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "const.h"
#include "config.h"

/*
Ajoute le couple (key, value) dans la map
retourne -1 en cas d'échec et 0 sinon
*/
int put(config_map *map, char* key, char* value) 
{
	int index = map->size;
	
	if (index >= MAP_SIZE)
	{
		return -1;
	}

	strcpy(map->array[index].key, key);
	strcpy(map->array[index].value, value);
	map->size += 1;

	return 0;
}

/*
Retourne la valeur associée à la clé key dans map
ou NULL si la clé est introuvable
*/
char* get(config_map *map, char* key)
{
	int size = map->size;
	int i;

	for (i = 0; i < size; i++) 
	{
		if (strcmp(key, map->array[i].key) == 0) 
		{
			return map->array[i].value;
		}
	}

	return NULL;
}

/*
Charge la configuration du serveur dans une structure map
en lisant les infos dans le fichier .jmrc
*/
void load_config(config_map *map) 
{
	char line[512];
	char key[256];
	char value[256];
	FILE* fd;
	map->size = 0;

	fd = fopen(".jmrc", "r");

	while (fgets(line, sizeof(line), fd) != NULL)
	{
		sscanf(line, "%s : %s", key, value);
		put(map, key, value);
	}
}

/*
Retourne la valeur du port lue dans la map de config
passée en paramètre ou le port par défaut (8080)
si le fichier de config ne spécifiait pas de port valide
*/
void get_port(config_map *map, int *port)
{
	char *port_str = get(map, "server_port");

	if (port_str == NULL)
	{
		*port = PORT;
		return;
	}
	
	if (sscanf(port_str, "%d", port) == 0)
	{	
		*port = PORT;
		return;
	}

	return;
}
