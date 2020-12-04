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

   ls ${PL}_${XS}.*.${ENERGY}.${TARG}.log.gz &> /dev/null
   if [ $? = 0 ] ; then
      rm ${PL}_${XS}.*.${ENERGY}.${TARG}.log.gz
   fi
   ls ${PL}_${XS}.*.${ENERGY}.${TARG}.log &> /dev/null
   if [ $? = 0 ] ; then
      gzip ${PL}_${XS}.*.${ENERGY}.${TARG}.log 
   fi

   n=1
   nmax=16

   while [ $n -le $nmax ]
   do

#      ./SATIF_BNL ${TARG}.${ENERGY}.in ${PL} ${XS} ${n} > ${PL}_${XS}.${n}job.${ENERGY}.${TARG}.log &
       qsub -v WORKDIR=$PWD,TARG=$TARG,ENERGY=$ENERGY,PL=$PL,XS=$XS,n=$n,OUTPUTFILE=${PL}_${XS}.${n}job.${ENERGY}.${TARG}.log ./torque.sh 
       #sh ./torque.sh ${TARG}.${ENERGY}.in ${PL} ${XS} ${n} ${PL}_${XS}.${n}job.${ENERGY}.${TARG}.log
       sleep 1

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

ls batch_job.* &> /dev/null
if [ $? = 0 ] ; then
   rm batch_job.*
fi
#targ_energy_ Shielding
targ_energy_ QGSP_BERT
#sub_ QGSP_BERT
