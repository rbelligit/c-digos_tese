/*
 * teste2.cpp
 *
 *  Created on: 13 de fev de 2019
 *      Author: belli
 */


#include "calcexit.h"
#include <map>
#include <vector>
#include <cmath>
#include <iostream>

int main(int argc, char *argv[])
{
	Maptype_st map =
	{
		{
			{0,0,0,0},
			{0,0,0,1},
			{0,0,1,0},
			{0,0,1,1},
			{0,1,0,0},
			{0,1,0,1},
			{0,1,1,0},
			{0,1,1,1},
			{1,0,0,0},
		},
		{
			{1,0,0,1},
			{1,0,1,0},
			{1,0,1,1},
			{1,1,0,0},
		},
		{
			{1,1,0,1},
			{1,1,1,0},
		},
		{
			{1,1,1,1},
		}
	};
	std::vector<double> levels =
	{
		0.0, 0.05, 0.1, 0.15
	};
	int size = map.size();
	std::map<std::vector<bool>, int> invMap;
	for(int i=0;i<size;++i)
	{
		for(auto &mm : map[i])
		{
			invMap[mm] = i;
		}
	}

	double reception = 0.15;
	std::vector<double> simb_probs = calc_simb_probs(reception, levels, .01);

	/*
double calc_message_simb_to_bit(int qBit, std::map<std::vector<bool>, int> &mapInv, std::vector<double> &simb_probs,
		std::vector<Probs_st> &probs)
	 */
	std::vector<double> arrivingMessages = {1,1,1,1};
	int nBits = 4;
	std::vector<Probabilidades1_st> probs1(arrivingMessages.size());
	for(int i=0;i<nBits;++i)
	{
		std::cout << "msg[" << i << "]=" << arrivingMessages[i] << "  ";
		probs1[i].prob_0 = 1 / (1+exp(arrivingMessages[i]));
		probs1[i].prob_1 = 1 - probs1[i].prob_0;
	}

	std::cout << std::endl;
	for(int i=0;i<4;++i)
	{
		std::cout << "simb[" << i << "]=" << simb_probs[i] << "  ";
	}
	std::cout << std::endl;
	int qualBit = 0;
	for(int i=0;i<nBits;++i)
	{
		double ret = calc_message_simb_to_bit(i, invMap, simb_probs, probs1);

		std::cout << "bit=" << i << " - ret=" << ret << std::endl;
	}
	return 0;
}
