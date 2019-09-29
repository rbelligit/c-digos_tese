/*
 * ReaderInputWNoise.cpp
 *
 *  Created on: 1 de nov de 2017
 *      Author: belli
 */

#include "ReaderInputWNoise.h"
#include <iostream>

NormalDistributionGeneretor::NormalDistributionGeneretor(uint64_t seed,
		double noise) :
		normDist(NULL), marseneGen(NULL)
{
	genMarseneGen(seed);
	genNormalDisp(noise);
}

NormalDistributionGeneretor::~NormalDistributionGeneretor()
{
	if(marseneGen)
	{
		delete marseneGen;
	}
	if(normDist)
	{
		delete normDist;
	}
}

double NormalDistributionGeneretor::getNewNumber()
{
	return (*normDist)(*marseneGen);
}

void NormalDistributionGeneretor::genMarseneGen(uint64_t seed)
{
	uint32_t seed1 = seed & 0xFFFFFFFF;
	uint32_t seed2 = (seed>>32) & 0xFFFFFFFF;


	std::seed_seq seed3{seed1, seed2, seed1 ^ seed2};

	marseneGen = new std::mt19937(seed3);

}

void NormalDistributionGeneretor::genNormalDisp(double noise)
{
	normDist = new std::normal_distribution<double>(0.0, noise);

}


//////////////////////////////////////////////////////////////////////////////////////


ReaderInputWNoise::ReaderInputWNoise(std::string filename, Data_st *data):
		norm(data->seed, data->noiseLevel), data(*data)
{
	// TODO Auto-generated constructor stub
	fidIn.open(filename, std::ifstream::in | std::ifstream::binary);

}

ReaderInputWNoise::~ReaderInputWNoise()
{
	// TODO Auto-generated destructor stub
}



int ReaderInputWNoise::readNewData(int quant, std::vector<double>* values)
{

	std::vector<uint8_t> leitura(quant, 0);

	fidIn.read((char *)&leitura[0], leitura.size());
	int ret = fidIn.gcount();

	std::cout << "Lendo " << quant << " símbolos - lidos=" << ret << std::endl;
	if(ret <= 0)
	{
		return ret;
	}
	if(values->size() < (size_t)ret)
	{
		values->resize(ret);
	}
	std::vector<double> *levels = &data.levels;
	int nLevels = levels->size();
	for(int i=0;i<ret;++i)
	{
		if(leitura[i] >= nLevels)
		{
			return -1000;
		}
		double levA = levels->at(leitura[i]);
		levA += norm.getNewNumber();
		(*values)[i] = levA;
	}
	return ret;
}

int ReaderInputWNoise::readNewData(std::vector<uint8_t>& entrada,
		std::vector<double>* values)
{
	if(values->size() != entrada.size())
	{
		values->resize(entrada.size());
	}
	std::vector<double> *levels = &data.levels;
	int nLevels = levels->size();
	int size = entrada.size();
	for(int i=0;i<size;++i)
	{
		if(entrada[i] >= nLevels)
		{
			return -1000;
		}
		double levA = levels->at(entrada[i]);
		levA += norm.getNewNumber();
		(*values)[i] = levA;
	}
	return 0;

}
