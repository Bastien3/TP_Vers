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

/* Crée un ver avec le pid du processus qui a créé le signal, une marque et l'ajoute à la liste des vers */
static void handler_initialiser_ver (int signal, siginfo_t *info_signal, void *contexte) {

	pid_t pid_ver = info_signal->si_pid;
	ver_t * ver = malloc(sizeof(ver_t));
	ver->marque = marque;
	marque++;
	
	/* Ouverture du terrain */
	if((fd_terrain = open( fich_terrain , O_RDWR , 0644 )) == -1) {
		fprintf( stderr, "%s : Pb open sur %s\n", Nom_Prog , fich_terrain);
		exit(-1);
	}
	
	/* Initialisation du ver */
	if ((jeu_ver_initialiser(fd_terrain, nb_lig, nb_col, ver) == -1)) {
		fprintf(stderr, "%s : erreur %d dans jeu_ver_initialiser\n", Nom_Prog, -1);
		exit(-1) ;
	}

	/* Attribution du pid au ver */
	ver_pid_set(ver, pid_ver);
	
	/* Ajout du vers à la liste */
	if (vers_ver_add(liste_vers, *ver) == -1) {
		fprintf( stderr, "Problème d'ajout du ver %d", (int)pid_ver);
		exit(-1);
	}
	vers_printf(liste_vers);
	close(fd_terrain);
}

/* Récupère le ver de la liste des vers en fonction de son pid, recherche une case vide, déplace le vers à cette case et affiche le terrain */
static void handler_terrain_rechercher (int signal, siginfo_t *info_signal, void *contexte) {

	/* Identification du ver qui accède au terrain par son pid */
	pid_t pid_ver = info_signal->si_pid;
	printf("pid vers : %d\n", (int)pid_ver);
	int numero_ver = vers_pid_seek(liste_vers, pid_ver);
	printf("numero ver : %d\n", numero_ver);
	ver_t ver = vers_ver_get(liste_vers, numero_ver);
	coord_t * liste_voisins = (coord_t *)NULL ;
	int nb_voisins = 0 ;
	int ind_libre = 0 ;

	/* Ouverture du terrain */
	if((fd_terrain = open( fich_terrain , O_RDWR , 0644 )) == -1) {
		fprintf( stderr, "%s : Pb open sur %s\n", Nom_Prog , fich_terrain);
		exit(-1);
	}

	/* Liste des voisins */
	if((terrain_voisins_rechercher(fd_terrain, ver.tete, &liste_voisins, &nb_voisins) == -1)) {
	 	fprintf(stderr, "%s : erreur %d dans terrain_voisins_rechercher\n", Nom_Prog, -1);
	   	exit(-1) ;
	}

	/* Recherche d'une case libre */
	if ((terrain_case_libre_rechercher( fd_terrain, liste_voisins, nb_voisins, &ind_libre) == -1)) {
	  	 fprintf(stderr, "%s : erreur %d dans terrain_case_libre_rechercher\n", Nom_Prog, -1);
		 exit(-1) ;
	}

	/* Ecriture dans cette case libre */
	if ((terrain_marque_ecrire(fd_terrain, ver.tete, ver.marque) == -1)) {
		fprintf( stderr, "%s : erreur %d dans terrain_marque_ecrire\n", Nom_Prog, -1);
		exit(-1) ;
	}

	/* Mise à jour du ver */
	ver.tete = liste_voisins[ind_libre];
	if (vers_ver_set(liste_vers, numero_ver, ver) == -1) {
		fprintf( stderr, "Problème de mise à jour du ver %d", (int)pid_ver);
		exit(-1);
	}

	/* Affichage du terrain */
	if ((terrain_afficher(fd_terrain) == -1)) {
		fprintf( stderr, "%s : erreur %d dans terrain_afficher\n", Nom_Prog, -1);
		exit(-1) ;
	}
	close(fd_terrain);
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

	/* Handler initialiser_vers pour signal SIGUSR1 */
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = handler_initialiser_ver;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGUSR2);
	sigaction(SIGUSR1, &action, NULL);

	/* Handler initialiser_vers pour signal SIGUSR1 */
	action.sa_flags = SA_SIGINFO | SA_NODEFER;
	action.sa_sigaction = handler_terrain_rechercher;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGUSR1);
	sigaction(SIGUSR2, &action, NULL);

	while(1);

	printf("\n\n\t----- %s : Fin du jeu -----\n\n" , Nom_Prog );

	vers_destroy(&liste_vers);

	exit(0);
}

