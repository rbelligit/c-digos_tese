/*
 * genFuncs.cpp
 *
 *  Created on: 28 de mar de 2019
 *      Author: belli
 */
#include "genFuncs.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <cstring>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

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

RandDataGsl::RandDataGsl():RandDataGsl(2002)
{

}

RandDataGsl::RandDataGsl(unsigned long int seed)
{
	r  = gsl_rng_alloc (gsl_rng_mt19937);
	gsl_rng_set(r, seed);
}

RandDataGsl::~RandDataGsl()
{
	gsl_rng_free(r);
}

uint32_t RandDataGsl::getRndUnsInt()
{
	return gsl_rng_get(r);
}

double RandDataGsl::getGaussian(double sig)
{
	return gsl_ran_gaussian(r, sig);
}


double RandDataGsl::getUniform()
{
	return gsl_rng_uniform(r);
}

constexpr int nPoints = 801;
constexpr double valLow = -20;
constexpr double valHigh = 20;
constexpr double diffValue = (valHigh - valLow);
constexpr double incrValue = diffValue / (static_cast<double>(nPoints) - 1.0);


static int calcIndex(double val)
{
	if(val < valLow)
	{
		return 0;
	}
	if(val > valHigh)
	{
		val = valHigh;
	}
	int v = floor((val - valLow) / incrValue);
	if(v >= nPoints)
	{
		return nPoints - 1;
	}
	if(v < 0)
	{
		v=0;
	}
	return v;
}

double calc_infMutua(std::vector<RespCalcInfMutua_st> &resp)
{
	int count[2][nPoints];
	int countTot[2];
	memset(count, 0, sizeof(count));
	memset(countTot, 0, sizeof(countTot));
	for(auto &val : resp)
	{
		int pos = calcIndex(val.res);
		count[val.bit?0:1][pos]++;
		countTot[val.bit?0:1]++;
	}

	//for(int i=0;i<nPoints;++i)
	//{
	//	std::cout << "count[0][" << (i-nPoints/2) << "] = " << count[0][i] << " - " << "count[1][" << (i-nPoints/2) << "]=" << count[1][i] << std::endl;
	//}

	double sumvv = 0.0;
	for(int i=0;i<nPoints;++i)
	{
		double totc = (double)countTot[0] + countTot[1];
		double temp1_0 = (double)count[0][i] / (totc);
		double temp1_1 = (double)count[1][i] / (totc);
		double temp2_0 = ((double)countTot[0]  / (totc) );
		double temp2_1 = ((double)countTot[1]  / (totc) );
		double temp3_0 = ((double)count[0][i] + count[1][i]) / (totc);
		if(count[0][i] > 0)
		{
			sumvv += temp1_0 * log2(temp1_0 / (temp2_0 * temp3_0));
		}
		if(count[1][i] > 0)
		{
			sumvv += temp1_1 * log2(temp1_1 / (temp2_1 * temp3_0));
		}
	}
	return sumvv;
}

double calc_inv_func(std::vector<double> func, double interval, double value)
{
	double valx = 0.0;
	double valb = 0.0;
	for(auto &v : func)
	{
		if(v > value)
		{
			if(valx < (interval / 2.0))
			{
				return valx;
			}
			return ((value - valb) / (v - valb)) * interval + valx;
		}

		valb = v;
		valx += interval;
	}
	return 0.98;
}

