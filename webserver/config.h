#ifndef __CONFIG__
#define __CONFIG__

#define MAP_SIZE 100

typedef struct {
	char key[256];
	char value[256];
} config_node;

typedef struct {
	int size;
	config_node array[MAP_SIZE];
} config_map;

int put(config_map *map, char* key, char* value);
char* get(config_map *map, char* key);
void load_config(config_map *map);
void get_port(config_map *map, int *port);

#endif
