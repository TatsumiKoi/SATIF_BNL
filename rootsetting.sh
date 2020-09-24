#!/bin/sh
#export ROOTSYS=/home/tkoi/Simulations/root/6.12.06
#export ROOTSYS=/opt/root/6.14.06
export ROOTSYS=/home/tkoi/ChubuU/root/root-6.22.02_install
#export ROOTSYS=/home/usr4/dc/tkoi/Simulation/Root/root-6.22.02
export LD_LIBRARY_PATH=$ROOTSYS/lib:${LD_LIBRARY_PATH}
export PATH=$ROOTSYS/bin:${PATH}
