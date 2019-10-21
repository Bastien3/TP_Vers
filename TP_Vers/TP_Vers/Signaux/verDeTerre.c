#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include <commun.h>
#include <terrain.h>
#include <vers.h>
#include <signaux.h>

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <jeu.h>
#include <errno.h>


/* 
 * VARIABLES GLOBALES (utilisees dans les handlers)
 */

/*
 * HANDLERS
 */

int
main( int nb_arg , char * tab_arg[] )
{
	/* Paramètres */
	char nomprog[128] ;
	pid_t pid_aire ;
	pid_t pid_ver ;
	int v ;

	int nb_lig;
	int nb_col;

	/* Capture des parametres */
	if (nb_arg != 4) {
		fprintf(stderr , "Usage : %s <pid aire> <longueur> <largeur>\n", tab_arg[0]);
		exit(-1);
	}

	strcpy(nomprog, tab_arg[0] );
	pid_aire = atoi( tab_arg[1] ) ;
	nb_lig = atoi(tab_arg[2]);
	nb_col = atoi(tab_arg[3]);

	/* Initialisation de la generation des nombres pseudo-aleatoires */
	srandom((unsigned int)getpid());

	pid_ver = getpid() ; 
	printf( "\n\n--- Debut ver [%d] ---\n\n" , pid_ver );   

	kill(pid_aire, SIGUSR1); /* Signal pour initialiser un ver */
	kill(pid_aire, SIGUSR2);
	kill(pid_aire, SIGUSR1); 
	kill(pid_aire, SIGUSR2);
	kill(pid_aire, SIGUSR2);

	for( v=0 ; v < nb_lig * nb_col ; v++) { /* Un ver peut se déplacer au maximum nb_lig * nb_col fois */

		kill(pid_aire, SIGUSR2); /* Signal pour déplacer un ver */	
	}

	printf( "\n\n--- Arret ver [%d] ---\n\n" , pid_ver );

	exit(0);
}
