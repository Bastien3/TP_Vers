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

int peut_reprendre = 0;

static void handler_peut_reprendre (int signal) {

	peut_reprendre = 1;
}

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

	struct sigaction action;

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

	action.sa_flags = 0;
	action.sa_handler = handler_peut_reprendre;
	sigemptyset(&action.sa_mask);
	sigaction(SIGALRM, &action, NULL);

	pid_ver = getpid() ; 
	printf( "\n\n--- Debut ver [%d] ---\n\n" , pid_ver );   

	kill(pid_aire, SIGUSR1); /* Signal pour initialiser un ver */

	for( v=0 ; v < nb_lig * nb_col ; v++) { /* Un ver peut se déplacer au maximum nb_lig * nb_col fois */

		kill(pid_aire, SIGUSR2); /* Signal pour déplacer un ver */
		sleep(1);
		while (!peut_reprendre); /* fait patienter le ver tant que aire n'a pas fini d'écrire */
		peut_reprendre = 0;	 /* évite d'avoir plein de signaux lancés d'un coup */	
					/* !!! A DEBUGGER !!! */
	}

	printf( "\n\n--- Arret ver [%d] ---\n\n" , pid_ver );

	exit(0);
}
