if( test $# -ne 4 ) then 
    echo "Usage: $0 <pid_aire> <longueur> <largeur> <nb_vers>"
    exit 1
fi 

pid_aire=$1
nb_vers=$4
longueur=$2
largeur=$3

./../Terrain/init_fich_terrain terrain $longueur $largeur
i=0
while( test $i -lt $nb_vers ) do 
 echo "lancement ver $i"
 ./verDeTerre $pid_aire $longueur $largeur &
 i=`expr $i + 1`
done 
