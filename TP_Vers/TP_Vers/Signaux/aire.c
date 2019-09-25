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




/*
 * HANDLER
 */



int
main( int nb_arg , char * tab_arg[] )
{
  char fich_terrain[128] ;
  pid_t pid_aire ; 

  /*----------*/
  
  /* 
   * Capture des parametres 
   */
  strcpy( Nom_Prog , tab_arg[0] );

  if( nb_arg != 2 )
    {
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
     
  /***********
   * A FAIRE * 
   ***********/
  
  printf("\n\n\t----- %s : Fin du jeu -----\n\n" , Nom_Prog );
  
  exit(0);
}

