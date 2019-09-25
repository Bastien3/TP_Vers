#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include <commun.h>
#include <terrain.h>
#include <vers.h>
#include <signaux.h>


/* 
 * VARIABLES GLOBALES (utilisees dans les handlers)
 */

/*
 * HANDLERS
 */

int
main( int nb_arg , char * tab_arg[] )
{
     char nomprog[128] ;
     pid_t pid_aire ;
     pid_t pid_ver ;

     /*----------*/

     /* 
      * Capture des parametres 
      */
     /* - nom du programme */
     strcpy( nomprog , tab_arg[0] );

     if( nb_arg != 2 )
     {
	  fprintf( stderr , "Usage : %s <pid aire>\n", 
		   nomprog );
	  exit(-1);
     }

     /* - parametres */
     pid_aire = atoi( tab_arg[1] ) ;


     /* Initialisation de la generation des nombres pseudo-aleatoires */
     srandom((unsigned int)getpid());

     pid_ver = getpid() ; 
     printf( "\n\n--- Debut ver [%d] ---\n\n" , pid_ver );   

     /***********
      * A FAIRE *
      ***********/
     
     printf( "\n\n--- Arret ver [%d] ---\n\n" , pid_ver );
  
     exit(0);
}
