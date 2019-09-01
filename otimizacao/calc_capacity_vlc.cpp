/*
 * calc_capacity_vlc.cpp
 *
 *  Created on: 1 de ago de 2018
 *      Author: belli
 */

constexpr double PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062L;
#include <vector>
#include <cmath>
#include <algorithm>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_message.h>
#include <gsl/gsl_integration.h>

#include <iostream>

struct InternalData_st
{
	std::vector<double> *levels;
	std::vector<double> *probs;

	double sigma_noise;
	double sigma2_2;
	double inv_sigma2_2;
	double sqrpisigma2_2;
	int size;
	InternalData_st(double sigma_noise, std::vector<double> *levels, std::vector<double> *probs);
};

static std::ostream &operator<<(std::ostream &str, const InternalData_st &data)
{
	str <<" sigma=" << data.sigma_noise <<
			" - sigma2_2=" << data.sigma2_2 << " - inv_sigma2_2=" << data.inv_sigma2_2 <<
			" - sqrpisigma2_2=" << data.sqrpisigma2_2 << " - ";
	for(int i=0;i<data.size ; ++i)
	{
		str << " - prob(" << (*data.levels)[i] << ")=" << (*data.probs)[i] ;
	}
	return str;
}

static void errorgslhandler (const char * reason,
              const char * file,
              int line,
              int gsl_errno)
{
	printf("Erro %s - %s - %d - %d\n", reason, file, line, gsl_errno);
}



inline InternalData_st::InternalData_st(double sigma_noise,
		std::vector<double>* levels, std::vector<double>* probs) :
	levels(levels),
	probs(probs),
	sigma_noise(sigma_noise),
	sigma2_2(2.0*sigma_noise*sigma_noise),
	inv_sigma2_2(1.0/sigma2_2),
	sqrpisigma2_2(sqrt(sigma2_2*PI)),
	size(levels->size())
{
}

static double calc_func_prob(double x, InternalData_st *data)
{
	int n = data->size;
	double res = 0.0;
	for(int i=0; i<n;++i)
	{
		double val1 = x - (*data->levels)[i];
		val1=val1*val1 * data->inv_sigma2_2;
		if(val1 <= 140)
		{
			val1 = exp(-val1) * (*data->probs)[i];
			res += val1;
		}
	}
	return res / data->sqrpisigma2_2;
}

static double calc_integrand(double x, void *data1)
{
	InternalData_st *data = (InternalData_st *)data1;

	double res = calc_func_prob(x, data);

	if(res < 1e-130)
	{
		return 0.0;
	}
	else
	{
		return res * log2(res);
	}

}

static double calc_integral(double ini, double fim, InternalData_st *data, gsl_integration_workspace *w)
{

	gsl_function func1;
	func1.function = &calc_integrand;
	func1.params = data;
	double result, error;
	gsl_integration_qags (&func1, ini, fim, 0, 1e-11, 4000,
							w, &result, &error);

	return result;
}


double calc_capacity_vlc(double sigma_noise, std::vector<double> *levels, std::vector<double> *probs)
{
	InternalData_st data(sigma_noise, levels, probs);
    

    // precisa ordenar os levels na ordem crescente e reordenar os probs de acordo...
    std::vector<std::pair<double,double>> datafix;
    datafix.reserve(data.size);
    
    for(int i=0;i<data.size;++i)
    {
    	datafix.push_back(std::make_pair( (*data.levels)[i], (*data.probs)[i]) );
    }
    
    std::sort(datafix.begin(), datafix.end(), [&](const std::pair<double,double>& a, const std::pair<double,double>& b)
            {
                return a.first < b.first;
            });
    for(int i=0;i<data.size;++i)
	{
    	(*data.levels)[i] = datafix[i].first;
    	(*data.probs)[i] = datafix[i].second;
    	//std::cout << (*data.levels)[i] << ", ";
	}
    //std::cout << std::endl;

	//std::cout << data << std::endl;

	gsl_integration_workspace *w = gsl_integration_workspace_alloc (4000);
	gsl_set_error_handler (errorgslhandler);

	double ans = 0;
	int size = data.size;

	for(int i=0;i<size+1;++i)
	{
		double min, fim;
		if(i==0)
		{
			min = (*data.levels)[0] - 10*sigma_noise - 0.1;
		}
		else
		{
			min = (*data.levels)[i-1];
		}
		if(i==size)
		{
			fim = (*data.levels)[size-1] + 10*sigma_noise + 0.1;
		}
		else
		{
			fim = (*data.levels)[i];
		}
		//std::cout << "ini=" << min << " - fim = " << fim << std::endl;
		ans += calc_integral(min,fim, &data, w);
	}

	double hgauss = log2(sigma_noise * sqrt(2.0*PI*exp(1)));

	//std::cout << "hg=" << hgauss << "- ans=" << ans << std::endl;

	gsl_integration_workspace_free(w);

	return -ans - hgauss;
}



