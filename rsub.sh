#!/bin/sh
sub_()
{
   PL=$1
   TARG=$2
   ENERGY=$3
   XS=DEFAULT
   #XS=JENDL
   #XS=BARASHENKOV
   #XS=JB

   echo $1 $2 $3

   rm ${PL}_${XS}.*.${ENERGY}.${TARG}.log.gz
   gzip ${PL}_${XS}.*.${ENERGY}.${TARG}.log 

   n=1
   nmax=16

   hosts=()
   while [ ${#hosts[*]} -le $nmax ]
   do 
      if [ $n -lt 10 ]; then 
         nm=0$n
      else
         nm=$n
      fi
      rsh b$nm w > /dev/null
      if [ $? -eq 0 ]; then
         hosts+=( $nm )
      fi 
      n=`expr $n + 1` 
   done
    
   n=1
   while [ $n -le $nmax ]
   do
      #./SATIF_BNL ${TARG}.${ENERGY}.in ${PL} ${XS} ${n} > ${PL}_${XS}.${n}job.${ENERGY}.${TARG}.log
      nn=`expr $n - 1`
      nm=${hosts[$nn]}
      rsh b$nm $PWD/rsubx.sh $PWD SATIF_BNL ${TARG}.${ENERGY}.in ${PL} ${XS} ${n} ${PL}_${XS}.${n}job.${ENERGY}.${TARG}.log 
      n=`expr $n + 1` 
   done
   wait
}

targ_energy_()
{
   PL=$1
   sub_ $PL Iron 2.83GeV
   sub_ $PL Iron 24GeV
   sub_ $PL Concrete 2.83GeV
   sub_ $PL Concrete 24GeV
}

#targ_energy_ Shielding
targ_energy_ QGSP_BERT
#sub_ QGSP_BERT
