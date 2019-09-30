/*
 * genFuncs.h
 *
 *  Created on: 28 de mar de 2019
 *      Author: belli
 */

#ifndef EXIT_CHART_GENFUNCS_H_
#define EXIT_CHART_GENFUNCS_H_

#include <vector>
#include <cinttypes>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
double calc_invJ(double val);
double calc_J(double val);


class RandDataGsl
{
public:
	RandDataGsl();
	RandDataGsl(unsigned long int seed);
	~RandDataGsl();

	uint32_t getRndUnsInt();
	double getGaussian(double sig=1);
	double getUniform();
private:
	gsl_rng * r;
};

struct RespCalcInfMutua_st
{
	bool bit;
	double res;
};

double calc_infMutua(std::vector<RespCalcInfMutua_st> &resp);
double calc_inv_func(std::vector<double> func, double interval, double value);


#endif /* EXIT_CHART_GENFUNCS_H_ */
