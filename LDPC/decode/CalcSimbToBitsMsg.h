/*
 * DecodeTemp.h
 *
 *  Created on: 20 de nov de 2017
 *      Author: belli
 */

#ifndef DECODE_CALCSIMBTOBITSMSG_H_
#define DECODE_CALCSIMBTOBITSMSG_H_

#include <vector>
#include <map>
#include <cstdint>
#include <exception>
#include <iostream>


class CalcSimbToBitsMsg {
public:
	CalcSimbToBitsMsg();
	virtual ~CalcSimbToBitsMsg();

	void setMapping(std::vector<uint32_t> *mapBitsToSimb);

private:
	struct Probs_st
	{
		double probs[2];
	};


public:
	double calcFuncsimbToBit(std::vector<double> &probsSimbs, std::vector<double> &msgsFromBits, int bitN);

	double calcFuncsimbToBitWBits(std::vector<double> &probsSimbs, std::vector<Probs_st> &msgsFromBist, int bitN, uint32_t bits);

private:
	std::vector<uint32_t> mapBitsToSimb;

	inline uint32_t getSimbFromBits(uint32_t val)
	{
		if(val >= mapBitsToSimb.size())
		{
			std::cout << "Erro na função " << __FUNCTION__ << " - arquivo : " << __FILE__ << " - linha " << __LINE__ << std::endl;
			exit(0);
		}
		return mapBitsToSimb[val];
	}
};

#endif /* DECODE_CALCSIMBTOBITSMSG_H_ */
