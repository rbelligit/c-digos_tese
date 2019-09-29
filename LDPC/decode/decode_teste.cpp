/*
 * decode_teste.cpp
 *
 *  Created on: 20 de jan de 2019
 *      Author: belli
 */

#include "LdpcDecodeProcessBase.h"

class Reader1 : public ReaderLdpcBase
{
public:
	Reader1(){};
	virtual ~Reader1(){};
	virtual int getMatrizHSize(int *rows, int *cols)
	{
		return 0;
	}
	virtual int getOnesInRow(int row, std::list<int> *ones)
	{
		return 0;
	}
	virtual int getOnesInCol(int col, std::list<int> *ones)
	{
		return 0;
	}
	virtual int getOnesInRow(int row, std::vector<int> *ones)
	{
		return 0;
	}
	virtual int getOnesInCol(int col, std::vector<int> *ones)
	{
		return 0;
	}
};

bool toPrint = true;

int main(int argc, char *argv[])
{
	std::vector<double> levels =
		{
			0, 0.0666, 0.1355, 0.2153
		};

	std::vector<std::vector<std::vector<bool>>> map =
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
				{1,0,0,0}
			},
			{
				{1,0,0,1},
				{1,0,1,0},
				{1,0,1,1},
				{1,1,0,0}
			},
			{
				{1,1,0,1},
				{1,1,1,0}
			},
			{
				{1,1,1,1}
			}
		};

	Reader1 readerLdpc;
	LdpcDecodeProcessBase::Params_st paramsDec;
	paramsDec.levels  = levels;
	paramsDec.mapOpts.map = map;
	paramsDec.mapOpts.nBits = 4;
	paramsDec.mapOpts.nLevels = 4;
	paramsDec.nRows   = 1;
	paramsDec.nSimbs  = 1;
	paramsDec.nbits   = 4;
	paramsDec.noise   = 0.001;
	paramsDec.reader  = &readerLdpc;


	/*LdpcDecodeProcessBase ldpcDecode(&paramsDec);
	std::vector<double> data = { 0.001 };
	std::vector<bool> bits = {0,0,1,0};
	ldpcDecode.insertNewData(&data, &bits);
	for(int i=0;i<4;++i)
	{
		std::cout << std::endl;
		ldpcDecode.calcMsg_VC_b(0,i);
	}*/

	std::vector<uint32_t> map2 =
		{0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,3};

	CalcSimbToBitsMsg calcSimbToBitsMsg;
	calcSimbToBitsMsg.setMapping(&map2);
	std::vector<double> probSimb = {1,0,0,0};
	std::vector<double> probs =
	{
		2,2,2,-10
	};
	for(int i=0;i<4;++i)
	{
		double temp  = calcSimbToBitsMsg.calcFuncsimbToBit(probSimb, probs,  i);
		std::cout << "resp = " << temp << "\n" << std::endl;
	}


}
