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

   while [ $n -le $nmax ]
   do

      ./SATIF_BNL ${TARG}.${ENERGY}.in ${PL} ${XS} ${n} > ${PL}_${XS}.${n}job.${ENERGY}.${TARG}.log &

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

targ_energy_ Shielding
#sub_ QGSP_BERT
