#!/bin/ksh
#
# @ class = serial
# @ restart = no
# @ output = saida_genldpc1.out
# @ error =  saida_genldpc1.err
# @ queue

localdir=`pwd`

export LD_LIBRARY_PATH="${localdir}/..";${LD_LIBRARY_PATH}

echo ${LD_LIBRARY_PATTH}

../make-ldpc pchkm_50000_70000_6_3_6_2.dat 50000 70000 1 evencol 6x3/6x2 no4cycle
../make-gen  pchkm_50000_70000_6_3_6_2.dat gen_50000_70000_6_3_6_2.dat sparse


#../make-ldpc pchkm_6000_8000.dat 6000 8000 1 evencol 3 no4cycle
#../make-gen  pchkm_6000_8000.dat gen_6000_8000.dat sparse


#../make-ldpc pchkm_6000_8000_c.dat 6000 8000 1 evencol 6x3/1.5x2/1x4/0.2x8/0.1x10 no4cycle
#../make-gen  pchkm_6000_8000_c.dat gen_6000_8000_c.dat sparse


#../make-ldpc pchkm_12000_17000_c.dat 12000 17000 1 evencol 3 no4cycle
#../make-gen  pchkm_12000_17000_c.dat gen_12000_17000_c.dat sparse

#../make-ldpc pchkm_20000_28000_c.dat 20000 28000 1 evencol 6x3/1.5x2/0.2x8/0.1x10 no4cycle
#../make-gen  pchkm_20000_28000_c.dat gen_20000_28000_c.dat sparse


#../make-ldpc pchkm_21000_28000_c.dat 21000 28000 1 evencol 6x3/1.5x2/0.2x8/0.1x10 no4cycle
#../make-gen  pchkm_21000_28000_c.dat gen_21000_28000_c.dat sparse


#../make-ldpc pchkm_30000_42000_c.dat 30000 42000 1 evencol 6x3/1.5x2/0.2x8/0.1x10 no4cycle
#../make-gen  pchkm_30000_42000_c.dat gen_30000_42000_c.dat sparse

#../make-ldpc pchkm_30000_43000_c.dat 30000 43000 1 evencol 6x3/1.5x2/0.2x8/0.1x10 no4cycle
#../make-gen  pchkm_30000_43000_c.dat gen_30000_43000_c.dat sparse

#../make-ldpc pchkm_30000_44000_c.dat 30000 44000 1 evencol 6x3/1.5x2/0.2x8/0.1x10 no4cycle
#../make-gen  pchkm_30000_44000_c.dat gen_30000_44000_c.dat sparse

#../make-ldpc pchkm_50000_70000_c.dat 50000 70000 1 evencol 6x3/1.5x2/0.2x8/0.1x10 no4cycle
#../make-gen  pchkm_50000_70000_c.dat gen_50000_70000_c.dat sparse

#../make-ldpc pchkm_50000_73000_c.dat 50000 73000 1 evencol 6x3/1.5x2/0.2x8/0.1x10 no4cycle
#../make-gen  pchkm_50000_73000_c.dat gen_50000_73000_c.dat sparse


#../make-ldpc pchkm_50000_70000_MM2.dat 50000 70000 1 evencol 6x3/2.5x2/0.2x8/0.1x10 no4cycle
#../make-gen  pchkm_50000_70000_MM2.dat gen_50000_70000_MM2.dat sparse


#../make-ldpc pchkm_50000_70000_m2.dat 50000 70000 1 evencol 6x3/1x2/0.2x8/0.1x10 no4cycle
#../make-gen  pchkm_50000_70000_m2.dat gen_50000_70000_m2.dat sparse


#../make-ldpc pchkm_50000_73000_c.dat 50000 73000 1 evencol 6x3/1.5x2/0.2x8/0.1x10 no4cycle
#../make-gen  pchkm_50000_73000_c.dat gen_50000_73000_c.dat sparse

#../make-ldpc pchkm_50000_70000_MMM2.dat 50000 70000 1 evencol 6x3/3.5x2/0.2x8/0.1x10 no4cycle
#../make-gen  pchkm_50000_70000_MMM2.dat gen_50000_70000_MMM2.dat sparse

#../make-ldpc pchkm_50000_70000_MMMM2.dat 50000 70000 1 evencol 6x3/4.5x2/0.2x8/0.1x10 no4cycle
#../make-gen  pchkm_50000_70000_MMMM2.dat gen_50000_70000_MMMM2.dat sparse

#../make-ldpc pchkm_50000_70000_MMMMM2.dat 50000 70000 1 evencol 6x3/6.5x2/0.2x8/0.1x10 no4cycle
#../make-gen  pchkm_50000_70000_MMMMM2.dat gen_50000_70000_MMMMM2.dat sparse

#../make-ldpc pchkm_50000_70000_6_3_65_2_03_8_02_10.dat 50000 70000 1 evencol 6x3/6.5x2/0.3x8/0.2x10 no4cycle
#../make-gen  pchkm_50000_70000_6_3_65_2_03_8_02_10.dat gen_50000_70000_6_3_65_2_03_8_02_10.dat sparse

#../make-ldpc pchkm_50000_70000_6_3_5_2.dat 50000 70000 1 evencol 6x3/5x2 no4cycle
#../make-gen  pchkm_50000_70000_6_3_5_2.dat gen_50000_70000_6_3_5_2.dat sparse



#../make-ldpc pchkm_50000_70000_6_3_7_2.dat 50000 70000 1 evencol 6x3/7x2 no4cycle
#../make-gen  pchkm_50000_70000_6_3_7_2.dat gen_50000_70000_6_3_7_2.dat sparse

#../make-ldpc pchkm_50000_70000_6_3_8_2.dat 50000 70000 1 evencol 6x3/8x2 no4cycle
#../make-gen  pchkm_50000_70000_6_3_8_2.dat gen_50000_70000_6_3_8_2.dat sparse

#../make-ldpc pchkm_50000_70000_6_3_9_2.dat 50000 70000 1 evencol 6x3/9x2 no4cycle
#../make-gen  pchkm_50000_70000_6_3_9_2.dat gen_50000_70000_6_3_9_2.dat sparse


#./make-ldpc pchkm_50000_70000_6_3_10_2.dat 50000 70000 1 evencol 6x3/10x2 no4cycle
#../make-gen  pchkm_50000_70000_6_3_10_2.dat gen_50000_70000_6_3_10_2.dat sparse

#../make-ldpc pchkm_50000_70000_6_3_12_2.dat 50000 70000 1 evencol 6x3/12x2 no4cycle
#../make-gen  pchkm_50000_70000_6_3_12_2.dat gen_50000_70000_6_3_12_2.dat sparse
