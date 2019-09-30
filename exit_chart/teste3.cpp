/*
 * teste3.cpp
 *
 *  Created on: 4 de abr de 2019
 *      Author: belli
 */
#include "calc_exit_bits_checks.h"
#include "calcexit.h"

#include <math.h>
#include <iostream>

int conv_to_num(std::vector<bool> number)
{
	int val = 0;
	int n = number.size();
	for(int i=0;i<n;++i)
	{
		if(number[i])
			val |= 1 << (n-1-i);
	}
	return val;
}



void func1(void)
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
	std::vector<int> invMap;
	invMap.resize(16);
	for(int i=0;i<size;++i)
	{
		for(auto &mm : map[i])
		{

			invMap[conv_to_num(mm)] = i+1;
		}
	}
	for(int i=0;i<16;++i)
	{
		std::cout << i << " - " << invMap[i] << std::endl;
	}
	DataCalcExit_st data;
	data.mapInv2 = invMap;
	data.levels = levels;
	data.nBits = 4;
	data.nSimuls = 100000;
	data.pesosBit = { {2, 0.5},{3, 0.5} };
	// Matrix H - 50000 x 70000
	// 70000 * (2 * 0.5 + 3 * 0.5) / 50000 = 3,5
	data.pesosCheck = { {3, 0.5}, {4, 0.5}};
	data.sigma = pow(10.0, -17.0/10.0);
	data.incremento = 0.02;

	DataCalcAnswer_st answer;

	calc_curv_exit_dem_to_bit_and_bit_to_dem(&data, &answer);

}

void func2()
{
	std::map<int, double> pesos =
	{
			{3, 0.5}, {4, 0.5}
	};

	double res = calc_exit_check_to_bits(0.99, pesos, 10000);
	std::cout << " res = " << res << std::endl;
}

int main(int argc, char *argv[])
{
	func1();
	return 0;
}
