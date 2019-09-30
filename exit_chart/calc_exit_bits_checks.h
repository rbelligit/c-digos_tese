/*
 * calc_exit_bits_checks.h
 *
 *  Created on: 28 de mar de 2019
 *      Author: belli
 */

#ifndef EXIT_CHART_CALC_EXIT_BITS_CHECKS_H_
#define EXIT_CHART_CALC_EXIT_BITS_CHECKS_H_

#include <vector>
#include <map>
#include "genFuncs.h"

struct DataCalcExit_st
{
	int nSimuls;
	std::vector<double> resp;
	std::map<int, double> pesosBit;
	std::map<int, double> pesosCheck;

	std::vector<int> mapInv2;
	int nBits;
	std::vector<double> levels;
	double sigma;
	double incremento;
};

struct DataCalcAnswer_st
{
	std::vector<double> j_dem_to_bits;
	std::vector<double> j_bits_to_dem;
};

struct DataCalcAnswer2_st
{
	std::vector<double> j_bits_to_check; // em função de j_checks_to_bits
	std::vector<double> j_checks_to_bits; // em função de j_bits_to_check
};

double calc_exit_bits_to_dem(double j_check_to_bits, std::map<int, double> &pesos, int nSimuls, RandDataGsl &rand);
double calc_exit_bits_to_check(double j_check_to_bits, double j_dem_to_bits, std::map<int, double> &pesos, int nSimuls, RandDataGsl &rand);
double calc_exit_check_to_bits(double j_bits_to_checks, std::map<int, double> &pesos, int nSimuls, RandDataGsl &rand);


void calc_curv_exit_dem_to_bit_and_bit_to_dem(DataCalcExit_st *data, DataCalcAnswer_st *answer);

void calc_curv_exit_bits_to_check_and_check_to_bits(DataCalcExit_st *data, DataCalcAnswer2_st *answer);


#endif /* EXIT_CHART_CALC_EXIT_BITS_CHECKS_H_ */
