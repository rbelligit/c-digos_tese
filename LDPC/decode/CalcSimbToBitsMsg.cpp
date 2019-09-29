/*
 * DecodeTemp.cpp
 *
 *  Created on: 20 de nov de 2017
 *      Author: belli
 */

#include "CalcSimbToBitsMsg.h"
#include <cmath>

CalcSimbToBitsMsg::CalcSimbToBitsMsg()
{
}

CalcSimbToBitsMsg::~CalcSimbToBitsMsg() {
	// TODO Auto-generated destructor stub
}

extern bool toPrint;

/**
 * probsSimbs -> Probabilidade de cada símbolo, dado o nível recebido.
 * msgsFromBits -> mensagens dos bits para este símbolo
 * bitN -> para qual bit esta sendo calculada a mensagem, de 0 até nBits-1 (nBits = msgsFromBits.size())
 */
double CalcSimbToBitsMsg::calcFuncsimbToBit(std::vector<double>& probsSimbs,
		std::vector<double>& msgsFromBits, int bitN)
{

 	int nBits = msgsFromBits.size();
	int sizeC = 1<<(nBits);
	int posCheck = 1<<bitN;

	//std::cout << "Processando bitN=" << bitN << " - nBits=" << nBits << " - sizeC=" << sizeC  << " posCheck=" << posCheck << std::endl;

	std::vector<Probs_st> msgs;
	msgs.resize(nBits);
	if(toPrint)
	{
		std::cout << "Bits: ";
	}
	for(int i=0;i<nBits;++i)
	{
		double t = exp(msgsFromBits[i]);
		// p[0] / (1-p[0]) = t
		// p[0] = t*1 - t*p[0]
		// p[0] + t*p[0] = t
		// p[0] = t / (1+t)
		msgs[i].probs[0] = t/(1.0+t);
		msgs[i].probs[1] = 1 - msgs[i].probs[0];
		if(toPrint)
		{
			std::cout << "(" << i << ")=" << msgs[i].probs[0] << "," << msgs[i].probs[1] << " [" << msgsFromBits[i] << "] - ";
		}
	}
	if(toPrint)
	{
		std::cout << std::endl;
	}

	double sum[2];
	sum[0]=0.0;
	sum[1]=0.0;

	for(int i=0;i<sizeC;++i)
	{
		int isBitSet = (i & posCheck)?1:0;
		double temp = calcFuncsimbToBitWBits(probsSimbs, msgs, bitN, i);
		if(toPrint)
		{
			std::cout << "bit " << i << " isSet= " << isBitSet << " - calc=" << temp << std::endl;
		}
		sum[isBitSet] += temp;
	}
	if(toPrint)
	{
		std::cout << "sum[0]=" << sum[0] << " - sum[1]=" << sum[1] << std::endl;
	}

	return log(sum[0] / sum[1]);

}

void CalcSimbToBitsMsg::setMapping(std::vector<uint32_t>* mapBitsToSimb)
{
	std::cerr << "Mapeamento: ";
	int i= 0;
	for(auto val : *mapBitsToSimb)
	{
		std::cerr << i << "->" << val << " || ";
		++i;
	}
	std::cerr << std::endl;
	this->mapBitsToSimb = *mapBitsToSimb;
}

double CalcSimbToBitsMsg::calcFuncsimbToBitWBits(std::vector<double> &probsSimbs,
		std::vector<Probs_st>& msgsFromBits, int bitN, uint32_t bits)
{
	int size = msgsFromBits.size();

	double prod = 1.0;
	uint32_t vv = 1;
	for(int i=0;i<size;++i)
	{
		if(i!=bitN)
		{
			prod *= msgsFromBits[i].probs[(bits & vv)?1:0];
		}

		vv <<=1;
	}

	prod *= probsSimbs[getSimbFromBits(bits)];

	//std::cout << "bits=" << bits << " - getSimbFromBits=" << getSimbFromBits(bits) << " prod=" << prod << std::endl;

	return prod;
}



