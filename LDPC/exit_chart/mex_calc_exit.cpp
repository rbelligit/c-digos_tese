/*
 * mex_calc_exit.cpp
 *
 *  Created on: 14 de fev de 2019
 *      Author: belli
 */

#include "calcexit.h"
#include <math.h>
#include <matrix.h>
#include <iostream>
#include <mex.h>

void printLevelsAndMap(std::vector<double> &inLevels, std::vector<int> &mapTo)
{
    std::cout << "inLevels=";
    for(double val : inLevels)
    {
        std::cout << val << ", ";
    }
    std::cout << " - map=";
    for(int i : mapTo)
    {
        std::cout << i << ", ";
    }
    std::cout << std::endl;
}
/**
 *Maptype_st& map, std::vector<double>& levels, double la,
		double sigma, int nSimuls
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	//mexPrintf("Hello World!\n");

	if(nlhs != 1)
	{
		mexErrMsgTxt("Erro.... não tem 1 saída");
		return;
	}

	// preciso passar o mapeamento
	if(nrhs != 5)
	{
		mexErrMsgTxt("Erro.... não tem 5 entradas");
		return;
	}

	//std::cout << "Aqui1" << std::endl;
	double *inMap = mxGetPr(prhs[0]);
	int sizeMap = mxGetNumberOfElements(prhs[0]);
	//std::cout << "Aqui2" << std::endl;
	double *inLevels = mxGetPr(prhs[1]);
	int inNLevels = mxGetNumberOfElements(prhs[1]);
	//std::cout << "Aqui3" << std::endl;

	double inLa = mxGetScalar(prhs[2]);

	double inSigma = mxGetScalar(prhs[3]);

	double inNSimuls = mxGetScalar(prhs[4]);
	//std::cout << "Aqui4" << std::endl;
	int nSimuls = floor(inNSimuls + 0.5);
	std::vector<int> MapTo(sizeMap);
	for(int i=0;i<sizeMap;++i)
	{
		MapTo[i] = floor(inMap[i]+0.5);
	}
	//std::cout << "Aqui5" << std::endl;
	std::vector<double> levels(inNLevels);
	for(int i=0;i<inNLevels;++i)
	{
		levels[i] = inLevels[i];
	}
	//std::cout << "Aqui6" << std::endl;
	int nBits = floor(log2(sizeMap)+0.5);

	//calcExitDecInc2(std::vector<int> &mapInv2, int nBits, std::vector<double>& levels, double la,
	//double sigma, int nSimuls);

	//std::cout << "Executando calcExitDecInc2 nBits=" << nBits << " - inLa=" << inLa << " - inSigma=" << inSigma << " - nSimuls=" << nSimuls << std::endl;

	double res = calcExitDecInc2(MapTo, nBits, levels, inLa, inSigma, nSimuls);
    
    //printLevelsAndMap(levels, MapTo);
	//std::cout << "Executando calcExitDecInc2 nBits=" << nBits << " - inLa=" << inLa << " - inSigma=" << inSigma << " - nSimuls=" << nSimuls << " - res=" << res << std::endl;

	plhs[0] = mxCreateDoubleScalar(res);

}

