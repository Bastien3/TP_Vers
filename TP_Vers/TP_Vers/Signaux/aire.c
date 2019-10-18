#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <commun.h>
#include <terrain.h>
#include <vers.h>
#include <jeu.h>
#include <signaux.h>

/* 
 * VARIABLES GLOBALES (utilisees dans les handlers)
 */
char Nom_Prog[256] ;
char fich_terrain[128] ;
int fd_terrain;
int nb_lig;
int nb_col;
vers_t * liste_vers;
case_t marque = 'A';

static void handler_initialiser_ver (int signal, siginfo_t *info_signal, void *contexte) {

	pid_t pid_ver = info_signal->si_pid;
	ver_t * ver = malloc(sizeof(ver_t));
	ver->marque = marque;
	marque++;
	
	if((fd_terrain = open( fich_terrain , O_RDWR , 0644 )) == -1) {
		fprintf( stderr, "%s : Pb open sur %s\n", Nom_Prog , fich_terrain);
		exit(-1);
	}
	printf(" Terrain initialise: \n");

	if ((jeu_ver_initialiser(fd_terrain, nb_lig, nb_col, ver) == -1)) {
		fprintf(stderr, "%s : erreur %d dans jeu_ver_initialiser\n", Nom_Prog, -1);
		exit(-1) ;
	}
	ver_pid_set(ver, pid_ver);
	vers_ver_add(liste_vers, *ver);
}

static void handler_terrain_ecrire (int signal, siginfo_t *info_signal, void *contexte) {

	printf("Terrain écrire\n");
}

static void handler_terrain_rechercher (int signal, siginfo_t *info_signal, void *contexte) {

	printf("Terrain rechercher");

}


int
main( int nb_arg , char * tab_arg[] )
{
	pid_t pid_aire ; 
	struct sigaction action;
	liste_vers = vers_new();

	/*----------*/

	/* 
	* Capture des parametres 
	*/
	strcpy( Nom_Prog , tab_arg[0] );

	if( nb_arg != 2 ) {
		fprintf( stderr , "Usage : %s <fichier terrain>\n", 
		       Nom_Prog );
		exit(-1);
	}

	strcpy( fich_terrain , tab_arg[1] );
	pid_aire = getpid() ; 

	/* Affichage du pid pour les processus verDeTerre */
	printf( "\n\t-----------------.\n") ; 
	printf( "--- pid %s = %d ---\n" , Nom_Prog , pid_aire ) ; 
	printf(   "\t-----------------.\n\n") ; 

	/* Initialisation de la generation des nombres pseudo-aleatoires */
	srandom((unsigned int)pid_aire);

	/*----------*/

	printf("\n\t----- %s : Debut du jeu -----\n\n" , Nom_Prog );

	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = handler_initialiser_ver;
	sigemptyset(&action.sa_mask);
	sigaction(SIGRTMIN, &action, NULL);

	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = handler_terrain_ecrire;
	sigemptyset(&action.sa_mask);
	sigaction(SIGRTMIN+1, &action, NULL);

	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = handler_terrain_rechercher;
	sigemptyset(&action.sa_mask);
	sigaction(SIGRTMIN+2, &action, NULL);

	sleep(60);

	printf("\n\n\t----- %s : Fin du jeu -----\n\n" , Nom_Prog );

	exit(0);
}

