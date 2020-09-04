version=geant4.10.06.p02
version2=Geant4-10.6.2

gcc_ver=gcc-10.2.0

source /home/tkoi/ChubuU/gcc/$gcc_ver/setup-gcc.sh

export CC=gcc
export CXX=g++

echo "Setting up ChubuU standard installation of ${version2} libraries."
echo "The libraries are built by $gcc_ver and now your \"gcc\" command is binding to"
echo "/home/tkoi/ChubuU/gcc/$gcc_ver/bin/gcc"
echo "Environment variables CC and CXX are also set to the gcc and g++, respectively"

source /home/tkoi/ChubuU/geant4/release/$version-install/share/$version2/geant4make/geant4make.sh
export Geant4_DIR=/home/tkoi/ChubuU/geant4/release/$version-install/lib/$version2

#export -n G4VIS_USE_OIX

#if test "x$LD_LIBRARY_PATH" = "x" ; then
#  export LD_LIBRARY_PATH="/afs/slac.stanford.edu/package/geant4/vol52/xercesc/xerces-c-3.1.1/$linux/lib:/afs/slac.stanford.edu/package/geant4/vol52/coin/Coin-3.1.3/$linux/lib"
#else
#  export LD_LIBRARY_PATH="/afs/slac.stanford.edu/package/geant4/vol52/xercesc/xerces-c-3.1.1/$linux/lib:/afs/slac.stanford.edu/package/geant4/vol52/coin/Coin-3.1.3/$linux/lib":${LD_LIBRARY_PATH}
#fi
