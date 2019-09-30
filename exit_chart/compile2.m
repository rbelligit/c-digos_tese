%%%%
%calcexit.o genFuncs.o calc_exit_bits_checks.o 
mex mex_calc_exit_comp.cpp calc_exit_bits_checks.cpp calcexit.cpp  genFuncs.cpp  -lgsl -lgslcblas -lm
mex mex_calc_exit_comp_bit_exit.cpp calc_exit_bits_checks.cpp calcexit.cpp  genFuncs.cpp  -lgsl -lgslcblas -lm

