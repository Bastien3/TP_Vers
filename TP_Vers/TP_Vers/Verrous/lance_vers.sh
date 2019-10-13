if( test $# -ne 4 ) then 
    echo "Usage: $0 <fichier terrain> <longueur> <largeur> <nb_vers>"
    exit 1
fi 

fich_terrain=$1
nb_vers=$4
longueur=$2
largeur=$3
i=0
marque='A'
./../Terrain/init_fich_terrain $fich_terrain $longueur $largeur
while( test $i -lt $nb_vers ) do 
   for marque in A B C D E F G H I J K L M N O P Q R S T U V W X Y Z ; do 
       if( test $i -ge $nb_vers ) then
           exit 0
       fi
       echo "lancement ver $i $marque"
       ./verDeTerre $fich_terrain $longueur $largeur $marque & 
	./vision $fich_terrain
       i=`expr $i + 1`
   done 
done 
