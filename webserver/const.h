#ifndef __CONST_H__
#define __CONST_H__


#define TRUE 1
#define FALSE 0
#define PORT 8080
#include "config.h"

int* get_in_maintenance();
config_map* get_config_map();
void traitement_commande(char* command, pid_t pid);
void maintenance(int* in_maintenance);
void maintenance_on(int* in_maintenance);
void maintenance_off(int* in_maintenance);
void help();
void forNoob();

#endif