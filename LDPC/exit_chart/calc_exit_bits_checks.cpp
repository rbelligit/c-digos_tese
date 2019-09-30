/*
 * calc_exit_bits_checks.cpp
 *
 *  Created on: 28 de mar de 2019
 *      Author: belli
 */


#include "calc_exit_bits_checks.h"
#include "genFuncs.h"
#include "calcexit.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <map>
#include <cmath>

#define DEBUG_T(x) x

double calc_exit_bits_to_dem(double j_check_to_bits, std::map<int, double> &pesos, int nSimuls, RandDataGsl &rand)
{
	double sig_l = calc_invJ(j_check_to_bits);
	double mi_l = sig_l * sig_l / 2.0;

	double sum_pesos = std::accumulate(pesos.begin(),pesos.end(), static_cast<double>(0.0), [](double v, std::map<int,double>::value_type &ptr)
			{
				return ptr.second + v;
			});

	//int nPesos = pesos.size();
	std::map<int,int> quantSimul;


	for(auto &val : pesos)
	{
		quantSimul[val.first] = static_cast<int>(val.second * static_cast<double>(nSimuls) / sum_pesos);
	}
	std::vector<RespCalcInfMutua_st> l_res;
	nSimuls = std::accumulate(quantSimul.begin(),quantSimul.end(), 0, [](int v, std::map<int,int>::value_type &ptr)
			{
				return ptr.second + v;
			});
	l_res.resize(nSimuls);
	int qualSimul = 0;


	for(auto ptr = quantSimul.begin(); ptr != quantSimul.end();++ptr)
	{
		for(int k = 0; k<ptr->second; ++k)
		{
			int i = ptr->first;
			bool bit = rand.getRndUnsInt() & 0x1;
			std::vector<double> lmsgs;
			lmsgs.resize(i+1);
			double media = bit?(mi_l):(-mi_l);
			for(int j=0;j<i+1;++j)
			{
				lmsgs[j] = media + rand.getGaussian(sig_l);
			}
			l_res[qualSimul].bit = bit;
			l_res[qualSimul].res = std::accumulate(lmsgs.begin(), lmsgs.end(), static_cast<double>(0.0));
			++qualSimul;
		}
	}

	double res = calc_infMutua(l_res);
	return res;
}


double calc_exit_bits_to_check(double j_check_to_bits, double j_dem_to_bits, std::map<int, double> &pesos, int nSimuls, RandDataGsl &rand)
{
	double sig_l = calc_invJ(j_check_to_bits);
	double mi_l = sig_l * sig_l / 2.0;
	double sig_l_db = calc_invJ(j_dem_to_bits);
	double mi_l_db = sig_l_db * sig_l_db / 2.0;

	double sum_pesos = std::accumulate(pesos.begin(),pesos.end(), static_cast<double>(0.0), [](double v, std::map<int,double>::value_type &ptr)
			{
				return ptr.second + v;
			});

	//int nPesos = pesos.size();
	std::map<int,int> quantSimul;


	for(auto &val : pesos)
	{
		quantSimul[val.first] = static_cast<int>(val.second * static_cast<double>(nSimuls) / sum_pesos);
	}
	std::vector<RespCalcInfMutua_st> l_res;
	nSimuls = std::accumulate(quantSimul.begin(),quantSimul.end(), 0, [](int v, std::map<int,int>::value_type &ptr)
			{
				return ptr.second + v;
			});
	l_res.resize(nSimuls);
	int qualSimul = 0;
	for(auto ptr = quantSimul.begin(); ptr != quantSimul.end();++ptr)
	{
		int i = ptr->first;
		for(int k = 0; k<ptr->second; ++k)
		{

			bool bit = rand.getRndUnsInt() & 0x1;

			std::vector<double> lmsgs;
			lmsgs.resize(i+1);
			double media = bit?(mi_l):(-mi_l);
			for(int j=0;j<i;++j)
			{
				lmsgs[j] = media + rand.getGaussian(sig_l);
			}
			double media2 = bit?(mi_l_db):(-mi_l_db);
			lmsgs[i] = media2 + rand.getGaussian(sig_l_db);
			l_res[qualSimul].bit = bit;
			l_res[qualSimul].res = std::accumulate(lmsgs.begin(), lmsgs.end(), static_cast<double>(0.0));
			++qualSimul;
		}
	}

	double res = calc_infMutua(l_res);
	return res;
}

double calc_exit_check_to_bits(double j_bits_to_checks, std::map<int, double> &pesos, int nSimuls, RandDataGsl &rand)
{
	//std::cerr << "aqui af -1" << std:: endl;
	double sig_l = calc_invJ(j_bits_to_checks);
	double mi_l = sig_l * sig_l / 2.0;

	//std::cerr << "aqui af 0" << std:: endl;

	double sum_pesos = std::accumulate(pesos.begin(),pesos.end(), static_cast<double>(0.0), [](double v, std::map<int,double>::value_type &ptr)
			{
				return ptr.second + v;
			});

	//std::cerr << "aqui af 1" << std:: endl;

	//int nPesos = pesos.size();
	std::map<int,int> quantSimul;

	std::cerr << "aqui af 2" << std:: endl;
	for(auto &val : pesos)
	{
		quantSimul[val.first] = static_cast<int>(val.second * static_cast<double>(nSimuls) / sum_pesos);
		//std::cout << val.first << " = " << quantSimul[val.first] << " --";
	}

	//std::cerr << "aqui af 3" << std:: endl;
	//std::cout << std::endl;
	std::vector<RespCalcInfMutua_st> l_res;
	nSimuls = std::accumulate(quantSimul.begin(),quantSimul.end(), 0, [](int v, std::map<int,int>::value_type &ptr)
			{
				return ptr.second + v;
			});
	l_res.resize(nSimuls);
	int qualSimul = 0;

	//std::cerr << "aqui af 4" << std:: endl;
	for(auto ptr = quantSimul.begin();ptr != quantSimul.end();++ptr)
	{
		int i = ptr->first;
		//std::cout << "i=" << i << std::endl;
		for(int k = 0; k<ptr->second; ++k)
		{


			std::vector<double> lmsgs;
			lmsgs.resize(i-1);
			bool bitFinal = true;
			for(int j=0;j<(i-1);++j)
			{
				bool bit = rand.getRndUnsInt() & 0x1;

				double media = bit?(-mi_l):(+mi_l);
				lmsgs[j] = media + rand.getGaussian(sig_l);
				bitFinal ^=bit;
				//std::cout << " - b " << (bit?"1":"0") << "-v=" << lmsgs[j];
			}
			//std::cout << " - bitfinal=" << (bitFinal?"1":"0") << std::endl;
			double vv = 1.0;

			for(const auto &val : lmsgs)
			{
				double tt = tanh(val / 2.0);

				vv *= tt;
				//std::cout << " - val=" << val << ":tt="<<tt<<":vv=" << vv;
			}
			//std::cout<<std::endl;
			if(vv < -0.99999999)
			{
				l_res[qualSimul].res = -20;
			}
			else if(vv > 0.99999999)
			{
				l_res[qualSimul].res = 20;
			}
			else
			{
				l_res[qualSimul].res = 2.0 * atanh(vv);
			}
			//std::cout << "Sim res vv=" << vv << " - l_r=" << l_res[qualSimul].res << std::endl;
			l_res[qualSimul].bit = bitFinal;
			++qualSimul;
		}
	}
	double res = calc_infMutua(l_res);
	return res;

}




void calc_curv_exit_dem_to_bit_and_bit_to_dem(DataCalcExit_st *data, DataCalcAnswer_st *answer)
{
	double incremento_;
	std::vector<double> j_bits_to_dem;
	std::vector<double> j_check_to_bits;
	std::vector<double> j_dem_to_bits;
	std::vector<double> j_bits_to_checks;
	std::vector<double> j_bits_to_dem_complete;
	incremento_ = data->incremento;
	int nSims = floor( (0.999 - 0) / incremento_ ) + 1;
	j_bits_to_dem.resize( nSims);
	j_check_to_bits.resize( nSims);
	j_dem_to_bits.resize(nSims);
	j_bits_to_checks.resize(nSims);
	j_bits_to_dem_complete.resize(nSims);

	RandDataGsl rand;


	DEBUG_T(std::cout << "###################################################################################" << std::endl);
	DEBUG_T(std::cout << "            INICIANDO CALCULO BITS_TO_DEM - fbits1 - em função de CHECK_TO_BITS" << std::endl);
	for(int i=0;i<nSims;++i)
	{
		double j = incremento_ * static_cast<double>(i);
		j_bits_to_dem[i] = calc_exit_bits_to_dem(j, data->pesosBit, data->nSimuls, rand);
		DEBUG_T(std::cout << "j=" << j << " - res = " << j_bits_to_dem[i] << std::endl);
	}



	DEBUG_T(std::cout << "###################################################################################" << std::endl);
	DEBUG_T(std::cout << "            INICIANDO CALCULO CHECKS_TO_BITS - fcheck - em função de BITS_TO_CHECK" << std::endl);
	for(int i=0;i<nSims;++i)
	{
		double j = incremento_ * static_cast<double>(i);
		j_check_to_bits[i] = calc_exit_check_to_bits(j, data->pesosCheck, data->nSimuls, rand);
		DEBUG_T(std::cout << "j=" << j << " - res = " << j_check_to_bits[i] << std::endl);
	}


	DEBUG_T(std::cout << "###################################################################################" << std::endl);
	DEBUG_T(std::cout << "            INICIANDO CALCULO DEM_TO_BITS - fdemmod - em função de BITS_TO_DEM" << std::endl);
	DEBUG_T(std::cout << "sigma=" << data->sigma << std::endl);
	for(int i=0;i<nSims;++i)
	{
		double j = incremento_ * static_cast<double>(i);
		j_dem_to_bits[i] = calcExitDecInc2(data->mapInv2, data->nBits, data->levels, j, data->sigma, data->nSimuls, rand);
		DEBUG_T(std::cout << "j=" << j << " - res = " << j_dem_to_bits[i] << std::endl);
	}

	DEBUG_T(std::cout << "###################################################################################" << std::endl);
	DEBUG_T(std::cout << "            INICIANDO CALCULO BITS_TO_CHECK em função de CHECK_TO_BITS" << std::endl);
	for(int i=0;i<nSims;++i)
	{
		double j_db = incremento_ * static_cast<double>(i);
		double j_bd = calc_inv_func(j_dem_to_bits, incremento_, j_db);
		double j_cb = calc_inv_func(j_bits_to_dem, incremento_, j_bd);
		//std::cout << "jdb=" << j_db << " - jbd=" << j_bd << " - j_cd=" << j_cb << std::endl;
		j_bits_to_checks[i] = calc_exit_bits_to_check(j_cb, j_db, data->pesosBit, data->nSimuls, rand);
		DEBUG_T(std::cout << "j=" << j_db << " - res=" << j_bits_to_checks[i] << std::endl);
	}

	DEBUG_T(std::cout << "###################################################################################" << std::endl);
	DEBUG_T(std::cout << "            INICIANDO CALCULO BITS_TO_DEM em função de DEM_TO_BITS - considerando todo o LDPC" << std::endl);
	for(int i=0;i<nSims;++i)
	{
		double j_db = incremento_ * static_cast<double>(i);
		double j_bc = j_bits_to_checks[i];
		double j_cb = calc_exit_check_to_bits(j_bc, data->pesosCheck, data->nSimuls, rand);
		j_bits_to_dem_complete[i] = calc_exit_bits_to_dem(j_cb, data->pesosBit, data->nSimuls, rand);
		DEBUG_T(std::cout << "j_db=" << j_db << " - j_bc=" << j_bc << " - j_cb = " << j_cb << " - j_bits_to_dem_complete=" << j_bits_to_dem_complete[i] << std::endl);
	}

	answer->j_bits_to_dem.resize(nSims);
	answer->j_dem_to_bits.resize(nSims);

	for(int i=0;i<nSims;++i)
	{
		answer->j_bits_to_dem[i] = j_bits_to_dem_complete[i];
		answer->j_dem_to_bits[i] = j_dem_to_bits[i];
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FillQuantCl
{
public:
	struct ValueInt_st
	{
		int n;
		double pos;
	};
	std::vector<ValueInt_st> values;
	int maxIndex;
	RandDataGsl &rand;
	FillQuantCl(const std::map<int, double> &pesos, RandDataGsl &rand) : rand(rand)
	{
		maxIndex = 0;
		double pesoTotal = 0;
		pesoTotal = std::accumulate(pesos.begin(), pesos.end(), 0.0, [](double v, const std::pair<const int,double> &d) mutable
				{
					return v+d.second;
				});

		maxIndex = (--pesos.end())->first;

		values.resize(pesos.size());
		double incAt = 0.0;
		int i = 0;
		for(auto &v : pesos)
		{
			incAt += v.second / pesoTotal;
			ValueInt_st vv;
			vv.n = v.first;

			vv.pos = incAt;
			values[i++] = vv;
		}
	}

	int getMaxIndex()
	{
		return maxIndex;
	}

	void fillQuantityPerBit(std::vector<int> *quant)
	{

		for(int i=0;i<quant->size();++i)
		{
			double v = rand.getUniform();
			int n = -1;
			for(auto &val : values)
			{
				if(v < val.pos)
				{
					n = val.n;
				}
			}
			if(n > 0)
			{
				(*quant)[i] = n;
			}
			else
			{
				(*quant)[i] = values[values.size()-1].n;
			}
		}
	}

};

struct MessageCheckToBits_st
{
	std::vector<double> messages;
	int size_messages;
	bool bit;
};

static void initializeValuesCheckToBits(std::vector<int> &quant, std::vector<MessageCheckToBits_st> *m_check_to_bits,  RandDataGsl &rand,
		double sig_l, double mi_l)
{
	int size1 = quant.size();
	//std::cerr << "Size m_check_to_bits=" << m_check_to_bits->size() << " - " << quant.size() << std::endl;
	for(int i=0;i<size1;++i)
	{
		//std::cerr << " i=" << i << " - size=" << (*m_check_to_bits)[i].messages.size() << " - " << quant[i] <<  std::endl;

		(*m_check_to_bits)[i].bit = rand.getRndUnsInt() & 0x1;
		(*m_check_to_bits)[i].size_messages = quant[i];

		//std::cerr << "a";
		for(int k=0;k<quant[i];++k)
		{
			//std::cerr << "b";
			double msg = ((*m_check_to_bits)[i].bit?(mi_l):(-mi_l)) + rand.getGaussian(sig_l);
			(*m_check_to_bits)[i].messages[k] = msg;

		}

		//std::cerr << std::endl;
	}
}

#define DEBUG_ON(x)

double calc_exit_bits_to_check_from_check_to_bits(double j_bits_to_checks, DataCalcExit_st *data, RandDataGsl &rand)
{
	//std::cerr << "Aqui 1" << std::endl;
	int sizeMapp1 = data->mapInv2.size();
	std::map<std::vector<bool>, int> mapInv;

	for(int i=0;i<sizeMapp1;++i)
	{
		std::vector<bool> vec1 = get_vector_bit_from_int(i, data->nBits);
		mapInv[vec1] = data->mapInv2[i]-1;
	}

	//std::cerr << "Aqui 2" << std::endl;
	std::vector<int> check_to_bit_size;
	check_to_bit_size.resize(data->nBits);

	FillQuantCl quant(data->pesosBit, rand);

	// preciso calcular o sigma....
	double sig_l = calc_invJ(j_bits_to_checks);
	double mi_l = sig_l * sig_l / 2.0;

	std::vector<MessageCheckToBits_st> m_check_to_bits;
	m_check_to_bits.resize(data->nBits);
	for(auto &val : m_check_to_bits)
	{
		val.messages.resize(quant.getMaxIndex());
	}

	//std::cerr << "Aqui 3" << std::endl;

	std::vector<double> m_bits_to_dem;
	m_bits_to_dem.resize(data->nBits);

	std::vector<double> m_dem_to_bits;
	m_dem_to_bits.resize(data->nBits);

	std::vector<MessageCheckToBits_st> m_bits_to_checks;
	m_bits_to_checks.resize(data->nBits);
	for(int i=0;i<data->nBits;++i)
	{
		m_bits_to_checks[i].size_messages = -1;
		m_bits_to_checks[i].messages.resize(quant.getMaxIndex());
	}

	std::vector<RespCalcInfMutua_st> response;
	response.resize(data->nSimuls * data->nBits * quant.getMaxIndex());
	int posResponse = 0;
	//std::cerr << "Aqui 4" << std::endl;

	for(int sim=0;sim<data->nSimuls;++sim)
	{
		/*
		 * Inicializa a quantidade e as mensagens dos check nodes para os bits, considerando a distribuição de pesos
		 */
		quant.fillQuantityPerBit(&check_to_bit_size);
		//std::cerr << "Aqui 4.5" << std::endl;
		initializeValuesCheckToBits(check_to_bit_size, &m_check_to_bits, rand, sig_l, mi_l);

		//std::cerr << "Aqui 5" << std::endl;

		// Como primeiro passo, preciso calcular as mensagens dos bits para o demodulador da mensagem do check to bit
		for(int i=0;i<data->nBits;++i)
		{
			double val = 0.0;
			for(int k=0;k<check_to_bit_size[i];++k)
			{
				val += m_check_to_bits[i].messages[k];
			}
			m_bits_to_dem[i] = val;
			DEBUG_ON(std::cout << "m_bits_to_dem[" << i << "]=" << val << std::endl);
		}

		//std::cerr << "Aqui 6" << std::endl;

		// calcular as mensagens do dem para os bits...
		//double calc_message_simb_to_bit(int qBit, std::map<std::vector<bool>, int> &mapInv, std::vector<double> &simb_probs,
		//		std::vector<Probabilidades1_st> &probs)
		std::vector<Probabilidades1_st> probs;
		probs.resize(data->nBits);
		std::vector<bool> bits(data->nBits);
		for(int i=0;i<data->nBits;++i)
		{
			/*
			 * llh = log_e(p[1] / (p[0]))=log_e((1-p[0])/p[0]) - O log_e é por causa da dedução do sigma das mensagens, que considera isso.
			 *	(1-p[0])/p[0] = e^LLH
			 *	1-p[0] = p[0] * e^LLH
			 *	(1+e^LLH)*p[0] = 1
			 *	p[0] = 1/(1+e^LLH)
			 */
			probs[i].prob_0 = 1.0 / (1.0+exp(m_bits_to_dem[i]));
			probs[1].prob_1 = 1 - probs[i].prob_0;

			bits[i] = m_check_to_bits[i].bit;
		}

		//std::cerr << "Aqui 7" << std::endl;

		int simbT = mapInv[bits];

		double level_rec = data->levels[simbT] + rand.getGaussian(data->sigma);
		std::vector<double> sim_probs =
							calc_simb_probs(level_rec, data->levels, data->sigma);

		for(int qb = 0;qb < data->nBits;++qb)
		{
			m_dem_to_bits[qb] = calc_message_simb_to_bit(qb, mapInv, sim_probs, probs);
			DEBUG_ON(std::cout << "m_dem_to_bits[" << qb << "]=" << m_dem_to_bits[qb] << std::endl);
		}

		//std::cerr << "Aqui 8" << std::endl;

		for(int qb = 0; qb<data->nBits;++qb)
		{
			int nnCheks = check_to_bit_size[qb];
			m_bits_to_checks[qb].bit = m_check_to_bits[qb].bit;
			m_bits_to_checks[qb].size_messages = check_to_bit_size[qb];

			for(int qCheck=0; qCheck<nnCheks;++qCheck)
			{
				double sum = 0.0;
				for(int i=0;i<nnCheks;++i)
				{
					if(i != qCheck)
					{
						sum+=m_check_to_bits[qb].messages[i];
					}
					sum+= m_dem_to_bits[qb];
				}
				m_bits_to_checks[qb].messages[qCheck] = sum;

				response[posResponse].res = sum;
				response[posResponse].bit = m_bits_to_checks[qb].bit;
				++posResponse;

				DEBUG_ON(std::cout << "m_bits_to_checks[" << qb << "].message[" << qCheck << "]=" << sum << std::endl);
			}
		}
	}


	/////
	// Neste ponto temos o valor de m_bits_to_checks calculado dado que tinhamos na entrada o valor de m_checks_to_bits
	// Precisamos calculas as estatísticas do sinal


	response.resize(posResponse);

	double answer = calc_infMutua(response);
	return answer;
}


void calc_curv_exit_bits_to_check_and_check_to_bits(DataCalcExit_st *data, DataCalcAnswer2_st *answer)
{
	double incremento_;
	//std::vector<double> j_check_to_bits;
	//std::vector<double> j_bits_to_check;

	std::cerr << "aqui13 ...." << std::endl;

	incremento_ = data->incremento;

	int nSims = floor( (0.999 - 0) / incremento_ ) + 1;

	std::cerr << "aqui14 ...." << std::endl;
	answer->j_checks_to_bits.resize(nSims);
	answer->j_bits_to_check.resize(nSims);

	RandDataGsl rand;

	DEBUG_T(std::cout << "###################################################################################" << std::endl);
	DEBUG_T(std::cout << "            INICIANDO CALCULO CHECK_TO_BITS -  - em função de BITS_TO_CHECK" << std::endl);
	for(int i=0;i<nSims;++i)
	{
		double j = incremento_ * static_cast<double>(i);
		answer->j_checks_to_bits[i] = calc_exit_check_to_bits(j, data->pesosCheck, data->nSimuls, rand);
		DEBUG_T(std::cout << "j=" << j << " - res = " << answer->j_checks_to_bits[i] << std::endl);
	}

	DEBUG_T(std::cout << "###################################################################################" << std::endl);
	DEBUG_T(std::cout << "            INICIANDO CALCULO BITS_TO_CHECK -  - em função de CHECK_TO_BIT" << std::endl);

	for(int i=0;i<nSims;++i)
	{
		double j = incremento_ * static_cast<double>(i);
		answer->j_bits_to_check[i] = calc_exit_bits_to_check_from_check_to_bits(j, data, rand);
		DEBUG_T(std::cout << "j=" << j << " - res = " << answer->j_bits_to_check[i] << std::endl);
	}


}





