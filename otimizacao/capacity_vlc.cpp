/*
 * capacity_vlc.cpp
 *
 *  Created on: 16 de out de 2016
 *      Author: belli
 */



#define PI 3.14159265359

#include "mex.h"
#include <cmath>


#include "calc_capacity_vlc.h"


/**
 * capacity_vlc(levels, probs, sig)
 */
void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
	if(nrhs != 3 || (nlhs != 1 && nlhs != 0))
	{
        char temp[1024];
        snprintf(temp, sizeof(temp), "Wrong number of parameters: answer = capacity_vlc(levels, probs, sig); nlhs=%d - nrhs=%d", nlhs, nrhs);
		mexErrMsgTxt(temp);
	}

	int nLevels = mxGetM(prhs[0])*mxGetN(prhs[0]);
	int nProbs = mxGetM(prhs[1])*mxGetN(prhs[1]);
	if(nLevels != nProbs)
	{
        char temp[1024];
        snprintf(temp, sizeof(temp), "Erro, tamanho diferente entre levels e probs: capacity_vlc(levels, probs, sig); nlevels=%d - nProbs=%d", nLevels, nProbs);
        
		mexErrMsgTxt(temp);
	}

	double *levels, *probs, *sig;
	levels= mxGetPr(prhs[0]);
	probs = mxGetPr(prhs[1]);
	sig   = mxGetPr(prhs[2]);
	
	std::vector<double> levels_v;
	levels_v.resize(nLevels);
	std::vector<double> probs_v;
	probs_v.resize(nLevels);
	for(int i=0;i<nLevels;++i)
	{
		levels_v[i]=levels[i];
		probs_v[i] = probs[i];
	}

	double res = calc_capacity_vlc(*sig, &levels_v, &probs_v);

	plhs[0] = mxCreateDoubleScalar(res);

}



