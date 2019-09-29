/*
 * exit1_dec.cpp
 *
 *  Created on: 6 de fev de 2019
 *      Author: belli
 */
#include "exit1_dec.h"

#include "gsl/gsl_rng.h"
#include "gsl/gsl_randist.h"
#include <cinttypes>
#include <iostream>
#include <map>
#include <cmath>

constexpr double PI =  3.141592653589793238L;

static gsl_rng * rngtt = nullptr;



double calc_invJ(double val)
{

	if(val < 0)
	{
		std::cerr << "calc_invJ val < 0" << std::endl;
		exit(0);
	}
	if(val < 0.3646)
	{
		return 1.09542*val*val+0.214217*val+2.33727*sqrt(val);
	}
	else if(val <= 1)
	{
		return  -0.706692*log(0.386013*(1-val)) + 1.75017*val;
	}
	std::cerr << "calc_invJ val > 1" << std::endl;
	exit(0);
}

static double calc_poly_val(std::vector<double> &coefs, double val)
{
	int n = coefs.size();
	double at = 1;
	double res = 0;
	for(int i=0;i<n;++i)
	{
		res += at * coefs[n-i-1];
		at *= val;
	}
	return res;
}

double calc_J(double val)
{
	if(val < 0)
	{
		std::cerr << "calc_J val < 0" << std::endl;
		exit(0);
	}
	if(val <= 1.6361)
	{
		static std::vector<double> coefs =
		{
			-0.0421061, 0.209252, -0.00640081, 0
		};
		return calc_poly_val(coefs, val);
	}
	else if(val < 10)
	{
		static std::vector<double> coefs =
		{
			0.00181491, -0.142675, -0.0822054, 0.0549508
		};
		double t = calc_poly_val(coefs, val);
		return 1-exp(t);
	}
	else
	{
		return 1.0;
	}
}


static void initializeRng()
{
	if(rngtt != nullptr)
	{
		return;
	}
	rngtt = gsl_rng_alloc (gsl_rng_mt19937);
}

static double getInputRandNorm(double sigma)
{
	return gsl_ran_gaussian(rngtt, sigma);
}

static uint32_t getInputRanduint32()
{
	unsigned long int val = gsl_rng_get(rngtt);
	return static_cast<uint32_t>(val);
}

static void fillSimbProbs(std::vector<double> *probs, int nSimbs,
		double noise_c, const std::vector<double> &levels,
		int simbTransmit)
{
	double nivelt = levels[simbTransmit] + getInputRandNorm(noise_c);
	double val1 = 1/(sqrt(2*PI) * noise_c);
	double val2 = 1/(2 * noise_c*noise_c);
	for(int i=0;i<nSimbs;++i)
	{
		double d1 = (levels[i] - nivelt);
		d1=d1*d1;
		(*probs)[i] = val1 * exp( -d1 * val2 );
	}
}

double calcNextExit(MapEntry_t &map, Levels_t &levels, double noise_dB, double La, int n_simuls)
{
	initializeRng();

	int nSimbs = map.size();
	if(nSimbs == 0)
	{
		std::cerr << "Erro, número de símbolos = 0" << std::endl;
		exit(0);
	}
	if(nSimbs != levels.size())
	{
		std::cerr << "Erro, número de símbolos no mapeamento != número de levels" << std::endl;
		exit(0);
	}
	std::map<std::vector<bool>, int> mapBitToSimb;
	int nBits = -1;
	{
		int simb = 0;
		for(auto &val : map)
		{
			for(auto &val1 : val)
			{
				if(nBits == -1)
				{
					nBits = val1.size();
				}
				else
				{
					if(nBits != static_cast<int>(val1.size()))
					{
						std::cerr << "Entradas com número diferente de bits" << std::endl;
						exit(0);
					}
					mapBitToSimb[val1] = simb;
				}
			}
			simb++;
		}
	}

	if(nBits < 0)
	{
		std::cerr << "Erro no mapeamento" << std::endl;
		exit(0);
	}
	double sig_msg = calc_invJ(La);
	double mi_l = 2*sig_msg*sig_msg;
	double noise_c = exp10(-noise_dB/10);

	for(int i=0;i<n_simuls;++i)
	{
		if((i+1)%10000 == 0)
		{
			std::cout << "i=" << i << std::endl;
		}

		std::vector<bool> bits(nBits);
		uint32_t vall = getInputRanduint32();
		for(int k=0;k<nBits;++k)
		{
			bits[k] = (vall >> k) & 0x1;
		}

		int simbTransmit = mapBitToSimb[bits];

		std::vector<double> LL(nBits);
		std::vector<double> probs_msg_0;
		std::vector<double> probs_msg_1;
		for(int k=0;k<nBits;++k)
		{
			if(bits[k])
			{
				LL[k] = mi_l+getInputRandNorm(sig_msg);
			}
			else
			{
				LL[k] = -mi_l+getInputRandNorm(sig_msg);
			}

			probs_msg_0[k] = exp(LL[k]);
			probs_msg_1[k] = 1 - probs_msg_0[k];
		}
		std::vector<double> simb_probs(nSimbs);
		fillSimbProbs(&simb_probs, nSimbs, noise_c, levels, simbTransmit);

		/**
		 * For nos bits
		 */
		for(int nb=0;nb<nBits;++nb)
		{

		}


	}
	return 0.1;
}
