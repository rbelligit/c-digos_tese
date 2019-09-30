
#include "calcexit.h"
#include <vector>
#include <map>
#include <iostream>
#include <cmath>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <cstdint>
#include "genFuncs.h"


/**
 * Considerando o bit[0] o mais significativo
 */

std::vector<bool> get_vector_bit_from_int(int value, int nBits)
{
	std::vector<bool> vv(nBits);

	for(int i=0;i<nBits;++i)
	{
		vv[i] = (value & (1<<(nBits-1-i)))!=0;
	}
	return vv;
}
#if 0
double calc_invJ(double val)
{
	/*
	if(II < 0)
	    error('II < 0, apenas valores entre 0 e 1 sao permitidos');
	end

	if(II <= 0.3646)
	    sigma = 1.09542*II^2+0.214217*II+2.33727*sqrt(II);
	elseif(II<=1)
	    sigma = -0.706692*log(0.386013*(1-II)) + 1.75017*II;
	else
	    error('II>1, apenas valores entre 0 e 1 sao permitidos');
	end
	 */
	if(val < 0)
	{
		std::cerr << "Erro calc_invJ val < 0" << std::endl;
		exit(0);
	}
/*
if(II <= 0.3646)
    sigma = 1.09542*II^2+0.214217*II+2.33727*sqrt(II);
elseif(II<=1)
    sigma = -0.706692*log(0.386013*(1-II)) + 1.75017*II;
else
    error('II>1, apenas valores entre 0 e 1 sao permitidos');
end

 */
	if(val <= 0.3646)
	{
		double res = 0;
		double v = val;
		res = v * 0.214217;
		v *= val;
		res += v * 1.09542;
		res += sqrt(val)*2.33727;
		return res;
	}
	else if(val <= 1)
	{
		return -0.706692*log(0.386013*(1-val)) + 1.75017*val;
	}
	else
	{
		std::cerr << "Erro calc_invJ val > 1" << std::endl;
		exit(0);
	}
}

static double calcPoly(std::vector<double> &coefs, double x)
{
	int nV = coefs.size();
	double res = 0;
	double val = 1;
	for(int i=0;i<nV;++i)
	{
		res += val * coefs[nV - 1 - i];
		val *= x;
	}
	return val;
}

double calc_J(double val)
{
	/*

end

if(sigma <= 1.6361)
    coefs = [-0.0421061 0.209252 -0.00640081 0];
    res = polyval(coefs, sigma);
elseif(sigma < 10)
    coefs = [0.00181491 -0.142675 -0.0822054 0.0549508];
    res = polyval(coefs, sigma);
    res = 1-exp(res);
else
    res = 1;
end
	 */
	if(val < 0)
	{
		std::cerr << "calc_j erro - val < 0" << std::endl;
		exit(0);
	}
	if(val <= 1.6361)
	{
		static std::vector<double> coefs1 =
		{
			-0.0421061, 0.209252, -0.00640081, 0
		};
		double res = calcPoly(coefs1, val);
		return res;
	}
	else if(val < 10)
	{
		static std::vector<double> coefs1 =
		{
			0.00181491, -0.142675, -0.0822054, 0.0549508
		};
		double res = calcPoly(coefs1, val);
		return 1.0 - exp(res);
	}
	else
	{
		return 1.0;
	}
}
#endif

constexpr double PI = 3.14159265359;

std::vector<double> calc_simb_probs(double levelRec, std::vector<double> &levels, double dev)
{

	int np = levels.size();
	std::vector<double> answ(np);
	constexpr double temp1 = sqrt(2 * PI);
	double temp2 = 1 / (temp1 * dev * dev);
	double temp3 = 2*dev*dev;
	for(int i=0;i<np;++i)
	{
		double diff = levels[i] - levelRec;
		diff = diff * diff;
		answ[i] = temp2 * exp(-diff / temp3);
	}
	return answ;
}
#include <cstring>


double calc_message_simb_to_bit(int qBit, std::map<std::vector<bool>, int> &mapInv, std::vector<double> &simb_probs,
		std::vector<Probabilidades1_st> &probs)
{

	int nBits = probs.size();

	int nCombs = 1 << (nBits);
	std::vector<bool> bitsV(nBits);
	double sum[2];
	for(int i=0;i<2;++i)
	{
		sum[i] = 0.0;
	}
	for(int i = 0;i<nCombs;++i)
	{
		for(int k=0;k<nBits;++k)
		{
			bitsV[k] = (i & (1 << (nBits - 1 -k))) > 0;
		}
		int ss = mapInv[bitsV];
		double pSib = simb_probs[ss];

		double prod = 1;
		double sumTemp[2];
		memcpy(sumTemp, sum, sizeof(sum));

		for(int k = 0;k<nBits;++k)
		{
			if(k != qBit)
			{
				if(bitsV[k])
				{
					prod *= probs[k].prob_1;
				}
				else
				{
					prod *= probs[k].prob_0;
				}
				if(std::isnan(prod))
				{
					std::cout << "prod is nan - " << probs[k].prob_1 << " e " << probs[k].prob_0 << std::endl;
				}
			}
		}

		//std::cout << "bit=" << (int)bitsV[qBit] << " - i=" << i << " - prod=" << prod << " - psimb=" << pSib << " - simb=" << ss << std::endl;
		prod *= pSib;
		if(std::isnan(prod))
		{
			std::cout << "prod is nan 2 - ss=" << ss  << "simb_probs[]=" << simb_probs[ss] << std::endl;
		}
		if(bitsV[qBit])
		{
			sum[1]+=prod;
		}
		else
		{
			sum[0]+=prod;
		}
		if(std::isnan(sum[0]) || std::isnan(sum[1]))
		{
			std::cout << "Estranho isNan agora " << sum[0] << ", " << sum[1] << ",," << sumTemp[0] << "," << sumTemp[1] << std::endl;

		}
	}
	if(sum[0] <= 1e-40)
	{
		return 30;
	}
	else if(sum[1] <= 1e-40)
	{
		return -30;
	}
	double res = log(sum[1] / sum[0]);
	if(std::isnan(res))
	{
		std::cout << "Sum[1]=" << sum[1] << " - sum[0]=" << sum[0] << std::endl;
	}
	return res;
}

std::ostream &operator<<(std::ostream &str, std::vector<bool> &vec)
{
	int nPos = vec.size();
	for(int i=0;i<nPos;++i)
	{
		if(vec[i])
		{
			str << '1';
		}
		else
		{
			str << '0';
		}
	}
	return str;
}

double calcExitDecInc2(std::vector<int> &mapInv2, int nBits, std::vector<double>& levels, double la,
		double sigma, int nSimuls, RandDataGsl &rand)
{
	int nSimbs = 0;
	for(const auto &v : mapInv2)
	{
		if(nSimbs < v)
		{
			nSimbs=v;
		}
	}
	++nSimbs;
	int sizeMapp1 = mapInv2.size();
	std::map<std::vector<bool>, int> mapInv;

	for(int i=0;i<sizeMapp1;++i)
	{
		std::vector<bool> vec1 = get_vector_bit_from_int(i, nBits);
		mapInv[get_vector_bit_from_int(i, nBits)] = mapInv2[i]-1;
	}

	double sig_la = calc_invJ(la);

	double mi_l = sig_la*sig_la / 2.0;
	//std::cout << "mi_l=" << mi_l << std::endl;


	struct Answer_st
	{
		uint8_t bit;
		double message;
	};

	std::vector<Answer_st> answer(nSimuls * nBits);

	for(int i=0;i<nSimuls;++i)
	{
		std::vector<bool> bits(nBits);
		std::vector<double> LL(nBits);
		for(int k=0;k<nBits;++k)
		{
			uint32_t value = rand.getRndUnsInt();
			//std::cout << "value=" << value << ", ";
			bits[k] = (value & 0x1);


			double rand1 = rand.getGaussian(sig_la);
			if(bits[k])
			{
				LL[k] = mi_l+rand1;
			}
			else
			{
				LL[k] = -mi_l+rand1;
			}
		}
		//std::cout << std::endl;
		int simbT = mapInv[bits];
		double level_rec = levels[simbT] + rand.getGaussian(sigma);
		std::vector<double> sim_probs =
				calc_simb_probs(level_rec, levels, sigma);

		for(int ii=0;ii<sim_probs.size();++ii)
		{
			if(std::isnan(sim_probs[ii]))
			{
				std::cout << "Simb prob " << ii << " is nan" << std::endl;
			}
		}

		std::vector<Probabilidades1_st> probs(LL.size());
		for(int i=0;i<nBits;++i)
		{
			if(std::isnan(LL[i]) || LL[i] < -40 || LL[i] > 40)
			{
				std::cout << "ERRO LL[i] = " << LL[i] << std::endl;
			}
			probs[i].prob_0 = 1.0 / (1.0+exp(LL[i]));
			probs[i].prob_1 = 1.0 - probs[i].prob_0;
		}

		for(int bb=0;bb<nBits;++bb)
		{
			double message = calc_message_simb_to_bit(bb, mapInv, sim_probs, probs);
			//std::cout << "Message=" << message << std::endl;
			if(std::isnan( message ))
			{
				std::cout << "Message NAN - returning" << std::endl;
				return 0;
			}
			if(message < -10)
			{
				message = -10;
			}
			else if(message > 10)
			{
				message = 10;
			}
			Answer_st st;
			st.bit = bits[bb];
			st.message = message;
			answer[i*nBits+bb] = st;
		}
	}

	int ss = answer.size();
	std::vector<int> pos_0(101);
	std::vector<int> pos_1(101);
	int total0 = 0;
	int total1 = 0;
	for(int i=0;i<101;++i)
	{
		pos_0[i] = 0;
		pos_1[i] = 0;
	}
	for(int i=0;i<ss;++i)
	{
		int ans = floor((answer[i].message + 10) / 0.2);
		if(ans < 0) ans = 0;
		if(ans > 100) ans = 100;
		if(answer[i].bit)
		{
			++total1;
			pos_1[ans]++;
		}
		else
		{
			++total0;
			pos_0[ans]++;
		}

	}
	double sumvv = 0;
	for(int i=0;i<101;++i)
	{

		double temp1_0 = (double)pos_0[i] / ((double)total1 + total0);
		double temp1_1 = (double)pos_1[i] / ((double)total1 + total0);
		double temp2_0 = ((double)total0  / ((double)total0 + total1));
		double temp2_1 = ((double)total1  / ((double)total0 + total1));
		double temp3_0 = ((double)pos_0[i] + pos_1[i]) / ((double)total0 + total1);
		if(pos_0[i] > 0)
		{
			sumvv += temp1_0 * log2(temp1_0 / (temp2_0 * temp3_0));
		}
		if(pos_1[i] > 0)
		{
			sumvv += temp1_1 * log2(temp1_1 / (temp2_1 * temp3_0));
		}
//		std::cout << "pos_0[" << i << "]=" << pos_0[i] << " - pos_1[" << i << "]=" << pos_1[i]
//				  << " - total_0=" << total0 << " - total_1=" << total1 <<  " - " << sumvv <<
//				  " - temp1_0=" << temp1_0 << " - temp1_1=" << temp1_1  << " - temp2_0=" << temp2_0
//				  << " - temp2_1=" << temp2_1 << " - temp3_0=" << temp3_0 <<  std::endl;
	}

	//std::cout << " sumvv=" << sumvv << std::endl;
	return sumvv;
}

int calc_value_from_bits(std::vector<bool> &vec)
{
	int nn = vec.size();
	int value = 0;
	for(int i=0;i<nn;++i)
	{
		if(vec[i])
		{
			value += (1<<(nn-1-i));
		}
	}
	return value;
}

double calcExitDecInc(Maptype_st& map, std::vector<double>& levels, double la,
		double sigma, int nSimuls)
{
	int nSimbs = map.size();

	int nBits = map[0].at(0).size();

	RandDataGsl rand;

	//std::cout << "Identificado nBits=" << nBits << " - nSimbs = " << nSimbs << std::endl;


	std::vector<int> mapInv;
	mapInv.resize(1<<nBits);

	for(int i=0;i<nSimbs;++i)
	{
		for(auto &val : map[i])
		{
			int val1 = calc_value_from_bits(val);
			mapInv[val1] = i;
		}
	}

	return calcExitDecInc2(mapInv, nBits, levels, la,
			sigma, nSimuls, rand);

}
/*
 * calcexit.cpp
 *
 *  Created on: 10 de fev de 2019
 *      Author: belli
 */




