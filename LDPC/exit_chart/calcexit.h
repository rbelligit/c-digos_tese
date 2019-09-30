/*
 * calcexit.h
 *
 *  Created on: 10 de fev de 2019
 *      Author: belli
 */

#ifndef EXIT_CHART_CALCEXIT_H_
#define EXIT_CHART_CALCEXIT_H_

#include <vector>
#include <map>
#include "genFuncs.h"
using BitsVec = std::vector<bool>;
using Maptype_st = std::vector<std::vector<BitsVec>>;

struct Probabilidades1_st
{
	double prob_0;
	double prob_1;
};

std::vector<bool> get_vector_bit_from_int(int value, int nBits);

double calcExitDecInc2(std::vector<int> &mapInv2, int nBits, std::vector<double>& levels, double la,
		double sigma, int nSimuls, RandDataGsl &rand);

std::vector<double> calc_simb_probs(double levelRec, std::vector<double> &levels, double dev);

double calc_message_simb_to_bit(int qBit, std::map<std::vector<bool>, int> &mapInv, std::vector<double> &simb_probs,
		std::vector<Probabilidades1_st> &probs);

double calcExitDecInc(Maptype_st &map, std::vector<double> &levels, double la, double sigma, int nSimuls);


#endif /* EXIT_CHART_CALCEXIT_H_ */
