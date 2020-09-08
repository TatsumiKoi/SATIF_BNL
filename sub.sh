#!/bin/sh

sub_()
{
   PL=$1
   XS=DEFAULT
   #XS=JENDL
   #XS=BARASHENKOV
   #XS=JB

   n=1
   nmax=16

   while [ $n -le $nmax ]
   do

      echo $n
      rm ${PL}_${XS}.${n}job.*.log.gz
      gzip ${PL}_${XS}.${n}job.*.log 

      ./SATIF_BNL Iron.2.83GeV.in ${PL} ${XS} ${n} > ${PL}_${XS}.${n}job.2.83GeV.Iron.log &
      ./SATIF_BNL Iron.24GeV.in ${PL} ${XS} ${n} > ${PL}_${XS}.${n}job.24GeV.Iron.log &
      ./SATIF_BNL Concrete.2.83GeV.in ${PL} ${XS} ${n} > ${PL}_${XS}.${n}job.2.83GeV.Concrete.log &
      ./SATIF_BNL Concrete.24GeV.in ${PL} ${XS} ${n} > ${PL}_${XS}.${n}job.24GeV.Concrete.log &

      wait
      n=`expr $n + 1` 
   done
}

sub_ Shielding
#sub_ QGSP_BERT


