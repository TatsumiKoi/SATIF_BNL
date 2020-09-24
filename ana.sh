#!/bin/sh

. ./rootsetting.sh

XS=DEFAULT

g++ calc.cc -o calc.x
g++ calc_cpu.cc -o calc_cpu.x
g++ calc_track.cc -o calc_track.x

result()
{
   EandT=2.83GeV.Concrete
   grep BNL_RESULT $1.*job.$EandT.log > data ; ./calc.x >> dummy
   EandT=2.83GeV.Iron
   grep BNL_RESULT $1.*job.$EandT.log > data ; ./calc.x >> dummy
   EandT=24GeV.Concrete
   grep BNL_RESULT $1.*job.$EandT.log > data ; ./calc.x >> dummy
   EandT=24GeV.Iron
   grep BNL_RESULT $1.*job.$EandT.log > data ; ./calc.x >> dummy
   cat dummy >> result_flux
   rm dummy
}


cpu()
{
   EandT=2.83GeV.Concrete
   echo $1.$EandT > data; grep "User=" $1.*.$EandT.log | cut -d"=" -f2 | cut -d"s" -f1  >> data ; ./calc_cpu.x >> dummy
   #grep "CPU time" $1.*.$EandT.log | grep -v Total > data ; ./calc_cpu.x >> dummy
   EandT=2.83GeV.Iron
   echo $1.$EandT > data; grep "User=" $1.*.$EandT.log | cut -d"=" -f2 | cut -d"s" -f1  >> data ; ./calc_cpu.x >> dummy
   #grep "CPU time" $1.*.$EandT.log | grep -v Total > data ; ./calc_cpu.x >> dummy
   EandT=24GeV.Concrete
   echo $1.$EandT > data; grep "User=" $1.*.$EandT.log | cut -d"=" -f2 | cut -d"s" -f1  >> data ; ./calc_cpu.x >> dummy
   #grep "CPU time" $1.*.$EandT.log | grep -v Total > data ; ./calc_cpu.x >> dummy
   EandT=24GeV.Iron
   echo $1.$EandT > data; grep "User=" $1.*.$EandT.log | cut -d"=" -f2 | cut -d"s" -f1  >> data ; ./calc_cpu.x >> dummy
   #grep "CPU time" $1.*.$EandT.log | grep -v Total > data ; ./calc_cpu.x >> dummy

   cat dummy >> result_cpu
   rm dummy
}


track()
{
   i=1
   imax=16 

   EandT=2.83GeV.Concrete
   if [ -f data ] 
   then
      rm data
   fi
   while [ $i -le $imax ] 
   do 
      grep \| $1.${i}job.$EandT.log >> data
      i=`expr $i + 1`
   done
   ./calc_track.x >> dummy

   i=1
   EandT=2.83GeV.Iron
   rm data
   while [ $i -le $imax ] 
   do 
      grep \| $1.${i}job.$EandT.log >> data
      i=`expr $i + 1`
   done
   ./calc_track.x >> dummy

   i=1
   EandT=24GeV.Concrete
   rm data
   while [ $i -le $imax ] 
   do 
      grep \| $1.${i}job.$EandT.log >> data
      i=`expr $i + 1`
   done
   ./calc_track.x >> dummy

   i=1
   EandT=24GeV.Iron
   rm data
   while [ $i -le $imax ] 
   do 
      grep \| $1.${i}job.$EandT.log >> data
      i=`expr $i + 1`
   done
   ./calc_track.x >> dummy
#
    cat dummy >> result_track
    rm dummy
}


ana_()
{
   PL=$1

   if [ -f dummy ] 
   then 
      rm dummy
   fi
   if [ -f result_flux ] 
   then 
      rm result_flux
   fi
   result ${PL}_${XS}
   mv result_flux result_flux_${PL}

   if [ -f result_cpu ] 
   then 
      rm result_cpu
   fi
   cpu ${PL}_${XS}
   mv result_cpu result_cpu_${PL}
   track ${PL}_${XS}
   mv result_track result_track_${PL}

   sh make_plots.sh ${PL} | grep CHI2 | tee result_chi2_${PL}
}

do_all_physics_list_()
{
   VERSION=$1
#   # "#" indicates comment out
   PLs=(`cat physics_lists/v$VERSION.dat | grep -v '#'`) 
   #PLs={`cat physics_lists/v$VERSION.dat | grep -v '#'`} 
   nPL=${#PLs[*]}

   n=0
   while [ $n -lt $nPL ]
   do 
      ana_ ${PLs[$n]}
      n=`expr $n + 1`
   done
}

#do_all_physics_list_ 10.00

#ana_ Shielding
#ana_ ShieldingLEND
ana_ QGSP_BERT
#ana_ FTFP_BERT
#ana_ FTFP_INCLXX
#ana_ QGSP_INCLXX
#ana_ FTF_BIC
#ana_ QGS_BIC
#ana_ LBE
#ana_ QBBC
#ana_ QGSP_BIC
#ana_ QGSP_FTFP_BERT

