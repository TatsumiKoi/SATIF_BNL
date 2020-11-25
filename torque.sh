#!/bin/sh
#PBS -q batch
#PBS -l ncpus=1 
#PBS -l nodes=1
#PBS -N batch_job
##PBS -V
##PBS -j oe
#PBS -o pbs_out
#PBS -M tkoi@isc.chubu.ac.jp
cd $PBS_0_WORKDIR
cd $WORKDIR
#source ../g4env.sh 
#mpiexec -hostfile ./host ./SATIF_BNL ${TARG}.${ENERGY}.in ${PL} ${XS} ${n} > ${PL}_${XS}.${n}job.${ENERGY}.${TARG}.log
mpiexec -hostfile ./host -np 1 bash -x run.sh $TARG $ENERGY $PL $XS $n $OUTPUTFILE
exit 0
