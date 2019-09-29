/*
 * encode_bit_count.cpp
 *
 *  Created on: 13 de set de 2019
 *      Author: belli
 */

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <random>
#include "EncodeProcessOnly.h"

static void generateRandomData(std::vector<bool> &data, std::uniform_int_distribution<> &dist, std::mt19937 &gen)
{
	int size = data.size();
	for(int i=0;i<size;++i)
	{
		data[i] = dist(gen);
	}
}

static int countOnes(std::vector<bool> &data)
{
	int count = 0;
	for(auto val : data)
	{
		if(val)
			++count;
	}
	return count;
}

static std::vector<double> levels =
	{0.0, 0.0666, 0.1355, 0.2153};

static std::vector<int> mapeamento =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1,
	2, 2,
	3
};

static double calcMeanLeval(std::vector<bool> &data)
{
	int nn = data.size();
	int sim = nn / 4;
	double levelMean = 0.0;
	for(int i=0;i<sim;++i)
	{
		int bitInd = 0;
		for(int k=0;k<4;++k)
		{
			bitInd <<= 1;
			if(data[i*4+k])
			{
				bitInd |=1;
			}
		}
		levelMean += levels[mapeamento[bitInd]];

	}
	return levelMean / static_cast<double>(sim);
}




int main(int argc, char *argv[])
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis(0,1);
	if(argc < 4)
	{
		std::cout << "erro" << std::endl;
		return 0;
	}

	EncodeProcessOnly encode(argv[2], argv[3]);

	int numSimul = std::stod(std::string(argv[1]));
	std::vector<bool> inputData;
	inputData.resize(encode.getSizeInput());
	std::vector<bool> outputData;
	outputData.resize(encode.getSizeOutput());
	std::vector<int> onesInput(inputData.size(), 0);
	std::vector<int> onesOutput(outputData.size(), 0);

	std::vector<int> levelsHist(10001, 0);
	for(int i=0;i<numSimul;++i)
	{

		// generate inputData ---
		generateRandomData(inputData, dis, gen);

		encode.doEncode(inputData,outputData);

		double level = calcMeanLeval(outputData);
		int numOnesInput = countOnes(inputData);
		int numOnesOutput = countOnes(outputData);
		onesInput[numOnesInput]++;
		onesOutput[numOnesOutput]++;

		int pos = static_cast<int>(floor(level / 0.0001));
		if(pos < 0)	pos = 0;
		if(pos > 10000) pos = 10000;
		levelsHist[pos]++;

		std::cout << "i=" << i << " - level=" << level << " - index=" << pos << " - numOnesInput = " << numOnesInput << " - numOnesOutput= " << numOnesOutput << std::endl;
	}


	FILE *fid = fopen("countOnesInput.txt", "w");
	for(auto &val : onesInput)
	{
		fprintf(fid,"%d\n", val);
	}
	fclose(fid);
	fid = fopen("countOnesOutput.txt", "w");
	for(auto &val : onesOutput)
	{
		fprintf(fid,"%d\n", val);
	}
	fclose(fid);
	fid = fopen("countHistLevel.txt", "w");
	for(auto &val : levelsHist)
	{
		fprintf(fid,"%d\n", val);
	}
	fclose(fid);


	return 0;
}


