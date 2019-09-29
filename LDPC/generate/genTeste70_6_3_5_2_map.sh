#!/bin/ksh
#
# @ class = serial
# @ restart = no
# @ output = saida_70_6_5_3_map.out
# @ error =  saida_70_6_5_3_map.err
# @ queue


localdir=`pwd`

echo "Iniciando"

export LD_LIBRARY_PATH=${localdir}/..:${localdir}/../src_orig/lib/:${LD_LIBRARY_PATH}

echo "${LD_LIBRARY_PATH}"

../encode_decode options_70_6_3_5_2_map.txt
