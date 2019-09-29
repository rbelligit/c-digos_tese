/*
 * teste_exit1.cpp
 *
 *  Created on: 6 de fev de 2019
 *      Author: belli
 */
#include <iostream>

#include "exit1_dec.h"

int main(int argc, char *argv[])
{
	MapEntry_t map1 =
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

	Levels_t levs = {0, .1, .2, .3};
	double n = calcNextExit(map1, levs, 17, 0.2, 100000);
	std::cout << "n=" << n << std::endl;
	return 0;
}



