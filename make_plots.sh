#!/bin/sh

#source ~/rootsetting.sh 	

make_plots_() {
   PL=$1
   root -l "plot3.C( $PL )"
}

#make_plots_ \"QGSP_BERT\"
#make_plots_ \"FTFP_BERT\"
#make_plots_ \"FTF_BIC\"
#make_plots_ \"QBBC\"
#make_plots_ \"QGSP_BIC\"
#make_plots_ \"Shielding\"
make_plots_ \"$1\"

