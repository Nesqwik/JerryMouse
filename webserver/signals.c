#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "signals.h"




/* Traitement du signal de fin de fils */
void traitement_signal()
{
	//printf("Signal %d reçu \n", sig);
	wait(NULL);
}


void initialiser_signaux (void)
{
	
	struct sigaction sa;
	
	
	/* Initialisation du signal de broken pipe */
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	{
		perror ( " signal " );
	}
	
	
	/* Remplissage de la structure de traitement du signal de fin de fils */
	sa . sa_handler = traitement_signal ;
	sigemptyset (& sa . sa_mask );
	sa . sa_flags = SA_RESTART ;
	
	/* Initialisation du signal de fin de fils */
	if ( sigaction ( SIGCHLD , & sa , NULL ) == -1)
	{
		perror ( " sigaction ( SIGCHLD ) " );
	}
}