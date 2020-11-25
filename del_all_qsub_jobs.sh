#bin/bash
list=(`qstat | grep tkoi | cut -d" " -f1`)

n=0
nmax=${#list[*]}

while [ $n -lt $nmax ]
do
   echo qdel ${list[$n]}
   n=`expr $n + 1`
done 

