#!/bin/bash
. ~/g4setting.sh > /dev/null
cd $1
#SATIF_BNL ${TARG}.${ENERGY}.in ${PL} ${XS} ${n} > ${PL}_${XS}.${n}job.${ENERGY}.${TARG}.log 
$2 $3 $4 $5 $6 > $7 &
