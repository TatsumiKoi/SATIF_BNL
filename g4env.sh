#!/bin/bash

version=geant4.10.06.p03
version2=Geant4-10.6.3

gcc_ver=gcc-10.2.0

#CentOS = 1
#Ubuntu = 2
#MacOS  = 3
which lsb_release > /dev/null
if [ $? == 0 ]; then
   name=`lsb_release --id | cut -f2`
   if [ $name = CentOS ] ; then 
      OS=1
   fi
   if [ $name = Ubuntu ] ; then 
      OS=2
   fi 
fi
which sw_vers > /dev/null
if [ $? = 0 ]; then
   OS=3
fi
#echo $OS

if [ $OS = 1 -o $OS = 2 ] ; then
   export CC=gcc
   export CXX=g++
fi 

if [ $OS = 1 ] ; then
   source /home/tkoi/ChubuU/gcc/$gcc_ver/setup-gcc.sh
   BASEDIR=/home/tkoi/ChubuU/geant4/release
   alias cmake="/home/tkoi/ChubuU/cmake/cmake-3.18.2-Linux-x86_64/bin/cmake"
fi
if [ $OS = 2 ] ; then
   . /home/usr4/dc/tkoi/Simulation/Gcc/$gcc_ver/setup-gcc.sh
   BASEDIR=/home/usr4/dc/tkoi/Simulation/Geant4/Release
   alias cmake="/home/usr4/dc/tkoi/Simulation/Cmake/cmake-3.18.2-Linux-x86_64/bin/cmake"
   export ROOTSYS=/home/usr4/dc/tkoi/Simulation/Root/root-6.22.02
   export LD_LIBRARY_PATH=$ROOTSYS/lib:${LD_LIBRARY_PATH}
   export PATH=$ROOTSYS/bin:${PATH}
fi
if [ $OS = 3 ] ; then
   BASEDIR=/Users/tkoi/Simulation/Geant4/Release
fi
source $BASEDIR/$version-install/share/$version2/geant4make/geant4make.sh
export Geant4_DIR=$BASEDIR/$version-install/lib/$version2

#export -n G4VIS_USE_OIX

#if test "x$LD_LIBRARY_PATH" = "x" ; then
#  export LD_LIBRARY_PATH="/afs/slac.stanford.edu/package/geant4/vol52/xercesc/xerces-c-3.1.1/$linux/lib:/afs/slac.stanford.edu/package/geant4/vol52/coin/Coin-3.1.3/$linux/lib"
#else
#  export LD_LIBRARY_PATH="/afs/slac.stanford.edu/package/geant4/vol52/xercesc/xerces-c-3.1.1/$linux/lib:/afs/slac.stanford.edu/package/geant4/vol52/coin/Coin-3.1.3/$linux/lib":${LD_LIBRARY_PATH}
#fi
