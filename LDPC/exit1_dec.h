/*
 * exit1.h
 *
 *  Created on: 6 de fev de 2019
 *      Author: belli
 */

#ifndef EXIT1_DEC_H_
#define EXIT1_DEC_H_

#include <vector>


using MapEntry_t = std::vector<std::vector<std::vector<bool>>>;

using Levels_t = std::vector<double>;

double calcNextExit(MapEntry_t &map, Levels_t &levels, double noise_dB, double La, int n_simuls);


#endif /* EXIT1_DEC_H_ */
