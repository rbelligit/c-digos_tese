/*
 * mex_calc_exit_comp.cpp
 *
 *  Created on: 8 de abr de 2019
 *      Author: belli
 */

#include "calcexit.h"
#include <math.h>
#include <matrix.h>
#include <iostream>
#include <mex.h>
#include <vector>
#include "calc_exit_bits_checks.h"

/**
 * 	int nSimuls;
	std::vector<double> resp;
	std::map<int, double> pesosBit;
	std::map<int, double> pesosCheck;

	std::vector<int> mapInv2;
	int nBits;
	std::vector<double> levels;
	double sigma;

	// [exit_dem_to_bit, exit_bit_to_dem] = mex_calc_exit_comp(nSimuls, mapInv, pesosBit, pesosCheck, levels, sigma, incremento)
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if(nlhs != 2)
	{
		mexErrMsgTxt("Erro.... não tem 2 saída");
		return;
	}
	if(nrhs < 6 || nrhs > 7)
	{
		mexErrMsgTxt("Erro, não tem 6 ou 7 entradas: nSimuls, mapInv, pesosBit, pesosCheck, levels, sigma, [incremento]");
		return;
	}
	DataCalcExit_st dataInp;
	DataCalcAnswer_st dataOut;

	int nSimuls = floor(mxGetScalar(prhs[0]) + 0.5);
	dataInp.nSimuls = nSimuls;

	int nMaps = mxGetNumberOfElements(prhs[1]);
	double *mapInv1 = mxGetPr(prhs[1]);

	std::vector<int> mapInv;
	mapInv.resize(nMaps);
	for(int i=0;i<nMaps;++i)
	{
		mapInv[i] = floor(mapInv1[i] + 0.5);
	}
	dataInp.mapInv2 = std::move(mapInv);

	int nPesoBit = mxGetNumberOfElements(prhs[2]);
	double *pesoBit1;
	pesoBit1 = mxGetPr(prhs[2]);
	int nPesoCheck = mxGetNumberOfElements(prhs[3]);
	double *pesoCheck1;
	pesoCheck1 = mxGetPr(prhs[3]);
	std::map<int, double> pesosBit;
	std::map<int, double> pesosChecks;
	for(int i=0;i<nPesoBit;++i)
	{
		if(pesoBit1[i] > 1e-9)
		{
			pesosBit[i+1] = pesoBit1[i];
		}
	}
	for(int i=0;i<nPesoCheck;++i)
	{
		if(pesoCheck1[i] > 1e-9)
		{
			pesosChecks[i+1] = pesoCheck1[i];
		}
	}
	dataInp.pesosBit = std::move(pesosBit);
	dataInp.pesosCheck = std::move(pesosChecks);

	int nLevels = mxGetNumberOfElements(prhs[4]);
	double *levels = mxGetPr(prhs[4]);
	dataInp.levels.resize(nLevels);
	for(int i=0;i<nLevels;++i)
	{
		dataInp.levels[i] = levels[i];
	}

	dataInp.sigma = mxGetScalar(prhs[5]);

	if(nrhs == 7)
	{
		dataInp.incremento = mxGetScalar(prhs[6]);
	}
	else
	{
		dataInp.incremento = 0.02;
	}
	dataInp.nBits = floor(log2(static_cast<double>(nMaps))+0.5);

	calc_curv_exit_dem_to_bit_and_bit_to_dem(&dataInp, &dataOut);

	int ntt = dataOut.j_bits_to_dem.size();

	plhs[0] = mxCreateDoubleMatrix(ntt, 1, mxREAL);
	plhs[1] = mxCreateDoubleMatrix(ntt, 1, mxREAL);
	double *ptr0 = mxGetPr(plhs[0]);
	double *ptr1 = mxGetPr(plhs[1]);


	for(int i=0;i<ntt;++i)
	{
		ptr0[i] = dataOut.j_dem_to_bits[i];
		ptr1[i] = dataOut.j_bits_to_dem[i];
	}

}

