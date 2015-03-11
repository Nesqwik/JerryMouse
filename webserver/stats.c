#include <sys/mman.h>
#include <stdio.h>
#include <semaphore.h>

#include "stats.h"

static web_stats* stats;
static sem_t* sem;

int init_stats(void) 
{
	stats = mmap(NULL, sizeof(web_stats), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	stats->served_connections = 0;
	stats->served_requests = 0;
	stats->ok_200 = 0;
	stats->ko_400 = 0;
	stats->ko_403 = 0;
	stats->ko_404 = 0;
	
	sem_init(sem, 42, 1);
	return 0;
}

web_stats *get_stats(void) 
{
	return stats;
}


void increment_stats(int code)
{
	sem_wait(sem);
	web_stats* stats = get_stats();
	switch(code)
	{
		case 200:
			stats->ok_200 += 1;
			break;

		case 400:
			stats->ko_400 += 1;
			break;

		case 403:
			stats->ko_403 += 1;
			break;

		case 404:
			stats->ko_404 += 1;
			break;

		case 0:
			stats->served_connections += 1;
			break;
		case 1:
			stats->served_requests += 1;
			break;
	}

	sem_post(sem);
}
