/*
 * genInputFile.cpp

 *
 *  Created on: 8 de fev de 2017
 *      Author: belli
 */


#include "genInputFile.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/discrete_distribution.hpp>

void printUsage()
{
	std::cerr << "genInputFile - generate input file for function ldpcmpamenctrans" << std::endl;
	std::cerr << "Usage:" << std::endl;
	std::cerr << "genInputFile <inpFile> <nBytes (nBits/8)> <seed>" << std::endl;
    std::cerr << "se seed == 0 será gerado apenas valores 0" << std::endl;
	exit(1);
}


int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		printUsage();
	}

	uint32_t seed = strtol(argv[3], NULL, 10);
	int size = strtol(argv[2], NULL, 10);
	std::string fileName = std::string(argv[1]);

	boost::random::mt19937 rand;
    
    FILE *fid = fopen(fileName.c_str(), "wb");
	if(fid==NULL)
	{
		std::cerr << "Erro abrindo arquivo " << fileName << std::endl << std::endl;

		printUsage();
	}

    if(seed == 0)
    {
        // gerar apenas zeros
        
        for(int i=0;i<size;++i)
        {
            uint8_t data;
            data = 0;
            fwrite(&data, 1, 1, fid);
        }
        return 0;
    }
	rand.seed(seed);

	boost::random::uniform_int_distribution<> dist(0, 255);


	


	for(int i=0;i<size;++i)
	{
		uint8_t data;
		data = dist(rand);
		fwrite(&data, 1, 1, fid);
	}

	fclose(fid);
	return 0;
}



