/*
 * ReaderInputWNoise.h
 *
 *  Created on: 1 de nov de 2017
 *      Author: belli
 */

#ifndef DECODE_READERINPUTWNOISE_H_
#define DECODE_READERINPUTWNOISE_H_

#include <string>
#include <vector>

#include <cctype>
#include <random>
#include <fstream>
class NormalDistributionGeneretor
{
public:
	NormalDistributionGeneretor(uint64_t seed, double noise);
	~NormalDistributionGeneretor();

	double getNewNumber();
private:
	std::normal_distribution<double> *normDist;
	std::mt19937 *marseneGen;

	void genMarseneGen(uint64_t seed);
	void genNormalDisp(double noise);

};

class ReaderInputWNoise {
public:

	struct Data_st
	{
		double noiseLevel;
		std::vector<double> levels;
		uint64_t seed;
	};

	ReaderInputWNoise(std::string filename, Data_st *data);
	virtual ~ReaderInputWNoise();

	/**
	 * Ja le os dados com o ruído adicionado
	 */
	virtual int readNewData(int quant, std::vector<double> *values);

	virtual int readNewData(std::vector<uint8_t> &entrada, std::vector<double> *values);

private:
	NormalDistributionGeneretor norm;
	Data_st data;

	std::ifstream fidIn;
};

#endif /* DECODE_READERINPUTWNOISE_H_ */
