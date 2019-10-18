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

	/*pid_t pid_ver = info_signal->si_pid;
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
	
	if (vers_ver_add(liste_vers, *ver) == -1) {
		fprintf( stderr, "Problème d'ajout du ver %d", (int)pid_ver);
		exit(-1);
	}

	close(fd_terrain);*/
	printf("Initialiser ver\n");
}

static void handler_terrain_rechercher (int signal, siginfo_t *info_signal, void *contexte) {

	/*pid_t pid_ver = info_signal->si_pid;
	printf("pid vers : %d\n", (int)pid_ver);
	int numero_ver = vers_pid_seek(liste_vers, pid_ver);
	printf("numero ver : %d\n", numero_ver);
	ver_t ver = vers_ver_get(liste_vers, numero_ver);
	coord_t * liste_voisins = (coord_t *)NULL ;
	int nb_voisins = 0 ;
	int ind_libre = 0 ;

	if((fd_terrain = open( fich_terrain , O_RDWR , 0644 )) == -1) {
		fprintf( stderr, "%s : Pb open sur %s\n", Nom_Prog , fich_terrain);
		exit(-1);
	}

	if((terrain_voisins_rechercher(fd_terrain, ver.tete, &liste_voisins, &nb_voisins) == -1)) {
	 	fprintf(stderr, "%s : erreur %d dans terrain_voisins_rechercher\n", Nom_Prog, -1);
	   	exit(-1) ;
	}

	if ((terrain_case_libre_rechercher( fd_terrain, liste_voisins, nb_voisins, &ind_libre) == -1)) {
	  	 fprintf(stderr, "%s : erreur %d dans terrain_case_libre_rechercher\n", Nom_Prog, -1);
		 exit(-1) ;
	}

	if ((terrain_marque_ecrire(fd_terrain, ver.tete, ver.marque) == -1)) {
		fprintf( stderr, "%s : erreur %d dans terrain_marque_ecrire\n", Nom_Prog, -1);
		exit(-1) ;
	}

	ver.tete = liste_voisins[ind_libre];
	if (vers_ver_set(liste_vers, numero_ver, ver) == -1) {
		fprintf( stderr, "Problème de mise à jour du ver %d", (int)pid_ver);
		exit(-1);
	}
	close(fd_terrain);*/
	printf("Terrain rechercher\n");
	kill(getpid(), SIGRTMIN);

}

static void handler_terrain_afficher (int signal, siginfo_t *info_signal, void *contexte) {

	/*if((fd_terrain = open( fich_terrain , O_RDONLY , 0644 )) == -1) {
		fprintf( stderr, "%s : Pb open sur %s\n", Nom_Prog , fich_terrain);
		exit(-1);
	}
	
	printf("Affichage du terrain : ");
	if ((terrain_afficher(fd_terrain) == -1)) {
		fprintf( stderr, "%s : erreur %d dans terrain_afficher\n", Nom_Prog, -1);
		exit(-1) ;
	}
	sleep(1);
	close(fd_terrain);*/
	printf("Terrain afficher\n");
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

	if( nb_arg != 4 ) {
		fprintf( stderr , "Usage : %s <fichier terrain> <longueur> <largeur>\n", 
		       Nom_Prog );
		exit(-1);
	}

	strcpy( fich_terrain , tab_arg[1] );
	pid_aire = getpid() ;
	nb_lig = atoi(tab_arg[2]);
	nb_col = atoi(tab_arg[3]);
 

	/* Affichage du pid pour les processus verDeTerre */
	printf( "\n\t-----------------.\n") ; 
	printf( "--- pid %s = %d ---\n" , Nom_Prog , pid_aire ) ; 
	printf(   "\t-----------------.\n\n") ; 

	/* Initialisation de la generation des nombres pseudo-aleatoires */
	srandom((unsigned int)pid_aire);

	/*----------*/

	printf("\n\t----- %s : Debut du jeu -----\n\n" , Nom_Prog );

	action.sa_flags = SA_SIGINFO | SA_RESTART;
	action.sa_sigaction = handler_initialiser_ver;
	sigemptyset(&action.sa_mask);
	sigaction(SIGRTMIN+2, &action, NULL);

	action.sa_flags = SA_SIGINFO | SA_RESTART;
	action.sa_sigaction = handler_terrain_rechercher;
	sigemptyset(&action.sa_mask);
	sigaction(SIGRTMIN+1, &action, NULL);

	action.sa_flags = SA_SIGINFO | SA_RESTART;
	action.sa_sigaction = handler_terrain_afficher;
	sigemptyset(&action.sa_mask);
	sigaction(SIGRTMIN, &action, NULL);

	sleep(60);

	printf("\n\n\t----- %s : Fin du jeu -----\n\n" , Nom_Prog );

	exit(0);
}

