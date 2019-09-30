/*
 * teste1.cpp
 *
 *  Created on: 10 de fev de 2019
 *      Author: belli
 */

#include "calcexit.h"

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
	for(double la = 0.0;la<=1;la+=0.05)
	{
		calcExitDecInc(map, levels, la, 0.005, 10000);
	}
}

