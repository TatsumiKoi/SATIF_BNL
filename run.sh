#!/bin/bash
source g4env.sh 
TARG=$1
ENERGY=$2
PL=$3
XS=$4
n=$5
OUTPUTFILE=$6
SATIF_BNL ${TARG}.${ENERGY}.in ${PL} ${XS} ${n} > ${PL}_${XS}.${n}job.${ENERGY}.${TARG}.log &
