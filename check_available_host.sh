#!/bin/bash

n=0
nmax=29

rm host 
while [ $n -le $nmax ] 
do 
   if [ $n -lt 10 ] ; then
      nn=0$n
   else 
      nn=$n
   fi
   la15m=`rsh a${nn}u uptime | cut -d"," -f5`
   #echo c${nn}u $la15m
   if [ `echo "$la15m < 1.0" | bc` == 1 ]; then
      echo c${nn} >> host
      #echo OK
   fi
   n=` expr  $n + 1 ` 
done

