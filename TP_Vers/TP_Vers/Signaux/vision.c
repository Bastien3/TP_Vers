
#include <commun.h>
#include <terrain.h>


/*--------------------* 
 * Main demon 
 *--------------------*/
int main( int nb_arg , char * tab_arg[] ) {

  	/* Parametres */
	char fich_terrain[128] ;
	char nomprog[256] ;
	struct stat info_terrain;
	time_t derniere_modif, dernier_acces;
	int fd_terrain;

  	/* Capture des parametres */
	if (nb_arg != 2) {
      		fprintf(stderr , "Usage : %s <fichier terrain>\n", tab_arg[0]);
      		exit(-1);
    	}
  	strcpy( nomprog , tab_arg[0] );
  	strcpy( fich_terrain , tab_arg[1] );



	/* On récupère la date de dernier accès et la date de dernière modification
	si elles sont identiques, le fichier terrain a été modifié, on l'affiche */
  	stat(fich_terrain, &info_terrain);
	derniere_modif = info_terrain.st_mtime;
	dernier_acces = info_terrain.st_atime;
	if (derniere_modif == dernier_acces) {
		fd_terrain = open(fich_terrain, O_RDONLY, 0666);
		printf("\n%s : ----- Debut de l'affichage du terrain ----- \n", nomprog );
		terrain_afficher(fd_terrain);
		printf("\n%s : --- Arret de l'affichage du terrain ---\n", nomprog );
		sleep(1);
		close(fd_terrain);
	}

  	exit(0);
}
