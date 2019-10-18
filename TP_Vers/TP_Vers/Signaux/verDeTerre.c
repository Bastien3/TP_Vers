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
	char fich_terrain[128] ;
	case_t marque = CASE_LIBRE ;
	int fd_terrain ;
	int no_err = 0 ;
	coord_t * liste_voisins = (coord_t *)NULL ;
	int nb_voisins = 0 ;
	int ind_libre = 0 ;
	int v ;
	ver_t* ver = malloc(sizeof(ver_t));

	int nb_lig;
	int nb_col;

	/* Capture des parametres */
	if (nb_arg != 5) {
		fprintf(stderr , "Usage : %s <pid aire> <longueur> <largeur>\n", tab_arg[0]);
		exit(-1);
	}

	if (strlen(tab_arg[4]) !=1) {
		fprintf(stderr, "%s : erreur marque <%s> incorrecte \n", tab_arg[0], tab_arg[2]);
		exit(-1) ;
	}

	strcpy(nomprog, tab_arg[0] );
	pid_aire = atoi( tab_arg[1] ) ;
	nb_lig = atoi(tab_arg[2]);
	nb_col = atoi(tab_arg[3]);	

	/* Initialisation de la generation des nombres pseudo-aleatoires */
	srandom((unsigned int)getpid());

	pid_ver = getpid() ; 
	printf( "\n\n--- Debut ver [%d] ---\n\n" , pid_ver );   

	kill(pid_aire, SIGRTMIN);
	exit(0);

	if((fd_terrain = open( fich_terrain , O_RDWR , 0644 )) == -1) {
		fprintf( stderr, "%s : Pb open sur %s\n", nomprog , fich_terrain);
		exit(-1);
	}

	//printf(" Terrain initialise: \n");

	/*if ((no_err = terrain_afficher(fd_terrain))) 
	{
		fprintf( stderr, "%s : erreur %d dans terrain_afficher\n", nomprog , no_err );
		exit(no_err) ;
	}*/


	if ((no_err = jeu_ver_initialiser(fd_terrain, nb_lig, nb_col, ver))) {
		fprintf(stderr, "%s : erreur %d dans jeu_ver_initialiser\n", nomprog, no_err );
		exit(no_err) ;
	}

	//printf(" Ecriture de la case[%d,%d]\n", ver->tete.y, ver->tete.x );

	if ((no_err = terrain_marque_ecrire(fd_terrain, ver->tete, marque))) {
		fprintf( stderr, "%s : erreur %d dans terrain_marque_ecrire\n", nomprog, no_err);
		exit(no_err) ;
	}

	/*if( (no_err = terrain_afficher(fd_terrain) )) 
	{
	fprintf( stderr, "%s : erreur %d dans terrain_afficher\n",
	   nomprog , no_err );
	exit(no_err) ;
	}*/
	for( v=0 ; v < nb_lig * nb_col ; v++) { /* Un ver peut se déplacer au maximum nb_lig * nb_col fois */

		//printf("v = %d\n", v);
		//printf(" Recherche des voisins de la case[%d,%d]\n", ver->tete.y, ver->tete.x );

		if((no_err = terrain_voisins_rechercher(fd_terrain, ver->tete, &liste_voisins, &nb_voisins))) {
	 		fprintf(stderr, "%s : erreur %d dans terrain_voisins_rechercher\n", nomprog, no_err);
	   		exit(no_err) ;
		}

		/*printf(" Affichage des voisins de la case[%d,%d]\n", ver->tete.y, ver->tete.x);

		terrain_voisins_afficher( fd_terrain, liste_voisins, nb_voisins) ;

		printf(" Recherche d'une case libre dans la liste des voisins\n"); */


		if ((no_err = terrain_case_libre_rechercher( fd_terrain, liste_voisins, nb_voisins, &ind_libre))) {
	  	 	fprintf(stderr, "%s : erreur %d dans terrain_case_libre_rechercher\n", nomprog, no_err);
		   	exit(no_err) ;
		}

		/*if (ind_libre == -1)
		   	printf( "Pas de case voisine libre\n");
		else
	  	 	printf("Ecriture dans la case libre choisie ==> %d\n", ind_libre );*/

		/* Si on a trouvé une case libre */

		/* Ecriture de la tête du ver à la nouvelle position */
	   	if ((no_err = terrain_marque_ecrire(fd_terrain, liste_voisins[ind_libre], marque))) {
			fprintf(stderr, "%s : erreur %d dans terrain_marque_ecrire\n", nomprog, no_err);
			exit(no_err) ;
	  	 }

		/* Mise à jour de la tête du ver */
		ver->tete = liste_voisins[ind_libre];	
	}

	/*if( (no_err = terrain_afficher(fd_terrain) )) 
	{
	   fprintf( stderr, "%s : erreur %d dans terrain_afficher\n",
		nomprog , no_err );
	   exit(no_err) ;
	}*/	  

	free(liste_voisins); 
	close(fd_terrain); 


	printf( "\n\n--- Arret ver [%d] ---\n\n" , pid_ver );

	exit(0);
}
