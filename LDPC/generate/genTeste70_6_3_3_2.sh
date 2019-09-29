#!/bin/ksh
#
# @ class = serial
# @ restart = no
# @ output = saida_70_6_3_3.out
# @ error =  saida_70_6_3_3.err
# @ queue


localdir=`pwd`

echo "Iniciando"

export LD_LIBRARY_PATH=${localdir}/..:${localdir}/../src_orig/lib/:${LD_LIBRARY_PATH}

echo "${LD_LIBRARY_PATH}"

../encode_decode options_70_6_3_3_2.txt
