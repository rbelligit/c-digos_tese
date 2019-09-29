#!/bin/ksh
#
# @ class = serial
# @ restart = no
# @ output = saida2.out
# @ error =  saida2.err
# @ queue


localdir=`pwd`

echo "Iniciando"

export LD_LIBRARY_PATH=${localdir}/../..:${localdir}/../../src_orig/lib/:${LD_LIBRARY_PATH}

echo "${LD_LIBRARY_PATH}"

../../encode_decode options1.txt
