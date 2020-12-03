#!/bin/bash

rm host 

n=0

#Checking A Zone
nmax=29
while [ $n -le $nmax ] 
do 
   if [ $n -lt 10 ] ; then
      nn=0$n
   else 
      nn=$n
   fi
   rsh a${nn}u uptime > /dev/null
   if [ $? = 0 ] ; then
     la15m=`rsh a${nn}u uptime | cut -d"," -f5`
     #echo a${nn}u $la15m
     if [ `echo "$la15m < 1.0" | bc` = 1 ]; then
        echo a${nn} >> host
        #echo OK
     fi
   fi 
   n=` expr  $n + 1 ` 
done

#Checking B Zone
n=0
nmax=34
while [ $n -le $nmax ] 
do 
   if [ $n -lt 10 ] ; then
      nn=0$n
   else 
      nn=$n
   fi
#   la15m=`rsh b${nn}u uptime | cut -d"," -f5`
#   #echo a${nn}u $la15m
#   if [ `echo "$la15m < 1.0" | bc` = 1 ]; then
#      echo b${nn} >> host
#      #echo OK
#   fi
   n=` expr  $n + 1 ` 
done


#Checking C Zone
n=0
nmax=29
while [ $n -le $nmax ] 
do 
   if [ $n -lt 10 ] ; then
      nn=0$n
   else 
      nn=$n
   fi
   rsh c${nn}u uptime
   if [ $? = 0 ] ; then
      la15m=`rsh c${nn}u uptime | cut -d"," -f5`
      #echo a${nn}u $la15m
      if [ `echo "$la15m < 1.0" | bc` = 1 ]; then
         echo c${nn} >> host
         #echo OK
      fi
   fi
   n=` expr  $n + 1 ` 
done

