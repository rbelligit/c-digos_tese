/*
 * LdpcDecodeProcessBase.cpp
 *
 *  Created on: 8 de nov de 2017
 *      Author: belli
 */

#include "LdpcDecodeProcessBase.h"
#include <cmath>
#include <iomanip>
#include <cstdint>

static uint32_t bitToUint32(std::vector<bool> &vals)
{
	uint32_t ans = 0;
	for(auto val : vals)
	{
		ans <<= 1;
		ans |= val?1:0;
	}
	return ans;
}

LdpcDecodeProcessBase::LdpcDecodeProcessBase(Params_st *params) :
	params(params)
{

	parseMatrix();

	std::vector<uint32_t> map;
	uint32_t nComb = 1<<params->mapOpts.nBits;
	map.resize(nComb);



	//std::vector<std::vector<std::vector<bool>>> map;
	// vetor externo... índice é o nível
	// segundo vetor... Valor é os mapeamentos para este nível
	int nlevels = params->mapOpts.map.size();
	for(int level = 0; level < nlevels;++level)
	{
		for(auto &map1 : params->mapOpts.map[level])
		{
			// mapeamentos que levam a este nível
			uint32_t bits = bitToUint32(map1);
			if(bits >= nComb)
			{
				std::cout << "Erro calculando bit To nível bits=" << bits << " - nComb=" << nComb << " params->nbits=" << params->nbits << std::endl;
				exit(0);
			}
			map[bits] = level;
		}
	}

	nBitsPerSimb = params->nbits / params->nSimbs;

	std::cerr << "Numero de bits por simbolo=" << nBitsPerSimb << std::endl;


	calcSimbToBitsMsg.setMapping(&map);
}

LdpcDecodeProcessBase::~LdpcDecodeProcessBase()
{

}

int LdpcDecodeProcessBase::insertNewData(std::vector<double>* data, std::vector<bool> *bits)
{
	//std::cout << "insertNewData - data->size=" << data->size() << " - bits->size=" << bits->size() << std::endl;
	//std::cerr << "Pass1 data=" << data << " - params=" << params << std::endl;
	if(data->size() != (size_t)params->nSimbs)
	{
		std::cerr << "Wrong size: data->size=" << data->size() << " - bits->size=" << bits->size() << std::endl;
		return -1; // wrong size in data.
	}
	//std::cerr << "Pass2" << std::endl;
	probsSimbs.resize(params->nSimbs);

	//std::cout << "SimbQuant = " << params->levels.size() << " - nSimbs=" << params->nSimbs << std::endl;

	//std::cerr << "insertNewData - pass 1" << std::endl;

	for(int i=0;i<params->nSimbs;++i)
	{
		double vAt = data->at(i);
		int simbQuant = params->levels.size();
		probsSimbs[i].resize(simbQuant);
		for(int s=0;s<simbQuant; ++s)
		{
			probsSimbs[i][s] = calcNormProb(vAt, params->levels[s]);
			//std::cout << "probsSimbs[" << i << "][" << s << "]=" << probsSimbs[i][s] << std::endl;
		}

	}

	//std::cerr << "insertNewData - pass 2" << std::endl;

	initializeAllMsgs();

	//std::cerr << "insertNewData - pass 3" << std::endl;

  bitsCoded = *bits; // copia os bits codificados de modo a poder comparar os erros da decodificação.

	return 0;
}

////////////////////////////////////////////////////////////////
extern bool toPrint;

int LdpcDecodeProcessBase::processMsgsSimbsToBits()
{
	int nSimbs = params->nSimbs;
	int kk = 0;
	for(int i=0;i<nSimbs;++i)
	{
		//agora todos os bits ligados a este símbolo
		for(int k=0;k<nBitsPerSimb;++k)
		{
			// Atualiza a mensagem
			msg_VCs_b[kk] = calcMsg_VC_b(i, k);
			if(msg_VCs_b[kk] < -1000)
			{
				msg_VCs_b[kk] = -1000;
			}
			else if(msg_VCs_b[kk] > 1000)
			{
				msg_VCs_b[kk] = 1000;
			}
			if(toPrint)
			{
				std::cout << "msg_Vcs[" << kk << "]=" << msg_VCs_b[kk] << std::endl;
			}
			++kk;
			// preciso atualizar a mensagem agora

		}
	}
	return 0;
}

int LdpcDecodeProcessBase::processMsgsBitsToSimbs()
{
	int nBits = params->nbits;
	for(int i=0;i<nBits;++i)
	{
		msg_b_VCs[i] = calcMsg_b_VC(i, 0); // destino não é utilizado
		if(toPrint)
		{
			std::cout << "msg_b_VCs[" << i << "]=" << msg_b_VCs[i] << std::endl;
		}
	}
	return 0;
}


int LdpcDecodeProcessBase::processMsgsBitsToLdpc()
{
	/**
	 * Mensagens b -> ldpc
	 *
	 * dois índices ->
	 *   -> um o bit de origem 0 -> até nbits-1
	 *   -> outro é o o índice do uns nas colunas (posOneInCols[bit][->>pos<<-])
	 */
	//std::vector<std::vector<double>> msg_b_ldpc;
	/**
	 * dst_l de 0 até nRows-1
	 * orig_b de 0 até nbits-1
	 * mensagem calculada do bit orig_b até a função ldpc dst_l
	 *double calcMsg_b_fldpc(int orig_b, int dst_l);
	 */
	int nBits = params->nbits;
	for(int i=0;i<nBits;++i) // varre as colunas.... a mensagem vai para apenas onde tem 1 nesta coluna
	{
		if(toPrint)
		{
			std::cout << "msg_b_ldpc[" << i << "]";
		}
		int kk = 0;
		for(auto &val : posOneInCols[i]) // varre as linhas que tem 1 nesta coluna
		{
			msg_b_ldpc[i][kk] = calcMsg_b_fldpc(i, val);
			if(toPrint)
			{
				std::cout << " (" << kk << ")=" << msg_b_ldpc[i][kk] ;
			}
			++kk;
		}
		if(toPrint)
		{
			std::cout << std::endl;
		}
	}
	return 0;

}

int LdpcDecodeProcessBase::processMsgsLdpcToBits()
{
	/**
	 * Mensagens ldpc -> b   em log likelihood
	 *
	 * dois indices ->
	 *   -> um a linha ldpc de origem 0 até nRows-1
	 *   -> Qual um (posição do um) na tabela posOneInRows[ldpc][->>pos<<--]
	 *
	 */
	//std::vector<std::vector<double>> msg_ldpc_b; // mensagem em log likelihood
	/**
	 * orig_l de 0 até nRows-1
	 * dst_b de 0 até nbits-1
	 *
	 * mensagem calculada da função ldpc dst_l até o bit dts_b
	 */
	//double calcMsg_fldpc_b(int orig_l, int dst_b);
	int nRows = params->nRows;
	for(int i=0;i<nRows;++i) // varre as linhas
	{
		int kk = 0;
		for(auto val:posOneInRows[i]) // varrer as colunas com '1' na linha i
		{
			msg_ldpc_b[i][kk] = calcMsg_fldpc_b(i,val);
			if(toPrint)
			{
				std::cout << "msg_ldpc_b[" << i << "][" << kk << "]=" << msg_ldpc_b[i][kk] << std::endl;
			}
			++kk;
		}
	}
	return 0;
}

int LdpcDecodeProcessBase::checkParity(int *erros_w_transm, int *oneNumber)
{
	int nbits = params->nbits;
	std::vector<double> valsBits(nbits);
	*oneNumber = 0;
	for(int i=0;i<nbits;++i)
	{
		valsBits[i] = msg_VCs_b[i] + msg_b_VCs[i];

		if(valsBits[i] > 0 )
		{
			(*oneNumber)++;
		}
	}

	int nrows = params->nRows;
	int erros = 0, erros2 = 0;

	for(int i=0;i<nrows;++i) // varre cada linha
	{
		int xorini = 0;
		for(auto val : posOneInRows[i])
		{
			xorini ^= (valsBits[val]<0)?1:0;
		}
		if(xorini != 0)
		{
			++erros;
		}
		xorini = 0;
		for(auto val : posOneInRows[i])
		{
			xorini ^= bitsCoded[val];
		}
		if(xorini != 0)
		{
			++erros2;
		}
	}


	if(erros_w_transm != NULL)
	{
		int counte = 0;
		for(int i=0;i<nbits;++i)
		{
			if(((valsBits[i]<0)?1:0) != bitsCoded[i])
			{
				++counte;
			}
		}
		*erros_w_transm = counte;
	}

	return erros;
}


////////////////////////////////////////////////////////////////

void LdpcDecodeProcessBase::parseMatrix()
{
	int nRows = params->nRows;
	int nCols = params->nbits;

	// preencher ones per column
	posOneInCols.resize(nCols);
	for(int i=0;i<nCols;++i)
	{
		params->reader->getOnesInCol(i, &posOneInCols[i]);
	}
	posOneInRows.resize(nRows);
	for(int i=0;i<nRows;++i)
	{
		params->reader->getOnesInRow(i, &posOneInRows[i]);
	}

}

constexpr double PI =  3.141592653589793238L;
constexpr double sqrt2 = sqrt(2);
constexpr double sqrt2pi = sqrt(2*PI);

double LdpcDecodeProcessBase::calcNormProb(double rec, double level)
{
	double diff = rec - level;
	diff = diff / (sqrt2 * params->noise);
	diff = exp(- (diff * diff));
	diff = diff / (sqrt2pi * params->noise);
	return diff;

}

/////////////////////////////////////////////////////////////////////////////////
//// INICIALIZAÇÃO DAS MENSAGENS
void LdpcDecodeProcessBase::init_msg_VCs_b()
{
	int nb = params->nbits;
	msg_VCs_b.resize(nb);
	for(int i=0;i<nb;++i)
	{
		msg_VCs_b[i] = 0;
	}
}

void LdpcDecodeProcessBase::init_msg_b_VCs()
{
	int nb = params->nbits;
	msg_b_VCs.resize(nb);
	for(int i=0;i<nb;++i)
	{
		msg_b_VCs[i] = 0;
	}
}

void LdpcDecodeProcessBase::init_msg_b_ldpc()
{
	// inicializar msg_b_ldpc,
	int nb = params->nbits;
	msg_b_ldpc.resize(nb);
	//std::cerr << "PosOneInCols size=" << posOneInCols.size() << " - posOneInRows size=" << posOneInRows.size() << " - nb=" << nb << " - nrows=" << params->nRows << " - " << std::endl;
	//std::cerr << "init_msg_b_ldpc pass1" << std::endl;
	for(int i=0;i<nb;++i)
	{
		int nsc = posOneInCols[i].size();
		msg_b_ldpc[i].resize(nsc);
		for(int k=0;k<nsc;++k)
		{
			msg_b_ldpc[i][k] = 0;
		}
	}

	// inicializar msgs_arrive_ldpc
	int nLdpcs = params->nRows;

	//std::cerr << "init_msg_b_ldpc pass2-" << std::endl;
	if(posOneInRows.size() < nLdpcs)
	{
		std::cerr << "Erro tamamno posOneInRows - size=" << posOneInRows.size() << " - esperado = " << nLdpcs << std::endl;

	}

	//std::cerr << "init_msg_b_ldpc pass3-" << std::endl;

	msgs_arrive_ldpc.resize(nLdpcs);
	for(int i=0;i<nLdpcs;++i) // i é a linha
	{
		msgs_arrive_ldpc[i].resize(posOneInRows[i].size());
		int count = 0;
		if(msgs_arrive_ldpc[i].size() < posOneInRows[i].size())
		{
			std::cerr << "Erro msgs_arrive_ldpc[i].size()=" << msgs_arrive_ldpc[i].size() << " < posOneInRows[i].size()= " << posOneInRows[i].size() << std::endl;
		}
		//std::cerr << ".";
		for(auto &v : posOneInRows[i]) // v é a coluna
		{

			if(v >= nb)
			{
				std::cerr << "v maior que nb " << v << "," << nb << " na linha " << i << std::endl;
				exit(0);
			}
			int nsc = posOneInCols[v].size();
			if(msg_b_ldpc[v].size() < nsc)
			{
				std::cerr << "msg_b_ldpc[v].size()=" << msg_b_ldpc[v].size() << "< nsc=" << nsc << std::endl;
			}
			//std::cerr << ",";
			for(int k=0;k<nsc;++k) // k é segundo indice em posOneInCols -> posOneInCols[v][k] == uma posicao com 1 na coluna v
			{
				if(posOneInCols[v][k] == i)
				{
					msgs_arrive_ldpc[i][count] =
					    Msgs_arrive_ldpc_st{
							v,
							&msg_b_ldpc[v][k]
						};
					break;
				}
			}
			++count;
		}
	}

	//std::cerr << "\ninit_msg_b_ldpc pass4" << std::endl;

}

/**
 * Vetor com dois índices, para busca contrária de mensagens
 * que chegam nos bits da função ldpc
 * 2 índices -> 1 -> bit
 *              2 -> funcoes ldpc ligados a este nó (uns na tabela posOneInCols)
 */
//std::vector<std::vector<Msgs_arrive_bits_from_ldpc_st>> msgs_arrive_bit_from_ldpc;

void LdpcDecodeProcessBase::init_msg_ldpc_b()
{
	int nLdpcs = params->nRows;
	msg_ldpc_b.resize(nLdpcs);

	for(int i=0;i<nLdpcs;++i)
	{
		int nsr = posOneInRows[i].size();
		msg_ldpc_b[i].resize(nsr);
		for(int j=0;j<nsr;++j)
		{
			msg_ldpc_b[i][j] = 0;
		}
	}

	int nBits = params->nbits;
	msgs_arrive_bit_from_ldpc.resize(nBits);


	for(int i=0;i<nBits;++i)
	{

		msgs_arrive_bit_from_ldpc[i].resize(posOneInCols[i].size());

		int count = 0;
		for(int lin : posOneInCols[i])
		{
			// a matriz de paridade tem um 1 em i,lin
			// preciso achar a posicao em posOneInRows[lin] que tem um '1', esta posicao será o segundo indice de msg_ldpc_b
			int nsr = posOneInRows[lin].size();
			for(int k=0;k<nsr;++k)
			{
				if(posOneInRows[lin][k] == i)
				{
					msgs_arrive_bit_from_ldpc[i][count] =
							Msgs_arrive_bits_from_ldpc_st
							{
								lin,
								&msg_ldpc_b[lin][k]
							};
				}
			}
			++count;
		}

	}

}




void LdpcDecodeProcessBase::initializeAllMsgs(void)
{
	std::cerr << "initializeAllMsgs - pass 1" << std::endl;
	init_msg_VCs_b();
	std::cerr << "initializeAllMsgs - pass 2" << std::endl;
	init_msg_b_VCs();
	std::cerr << "initializeAllMsgs - pass 3" << std::endl;
	init_msg_b_ldpc();
	std::cerr << "initializeAllMsgs - pass 4" << std::endl;
	init_msg_ldpc_b();
	std::cerr << "initializeAllMsgs - pass 5" << std::endl;
}

/////////////////////////////////////////////////////////////////////////////////


/**
 * orig_s de 0 até nSimbs-1
 * dst_b de 0 até nbitsPerSimb-1
 * Resposta em log likelihood....
 *
 * mensagem calculada da origem -> função símbolo orig_s
 * para o bit dst_b
 *
 * dst_b deve estar no intervalo orig_s*nBitsPerSimb até (orig_s+1)*nBitsPerSimb - 1
 */
double LdpcDecodeProcessBase::calcMsg_VC_b(int orig_s, int dst_b)
{
	// calcula nova mensagem do símbolo s para o bit b
	// envia o log likelihood
	int nBitOrig = nBitsPerSimb * orig_s;
	//int nBitFim = nBitOrig + nBitsPerSimb - 1;



	std::vector<double> msgsFromBitsT;
	msgsFromBitsT.resize(nBitsPerSimb);
	// conta os bits...

	//std::vector<double> &probsSimbT = probsSimbs[orig_s];
	for(int i=0;i<nBitsPerSimb;++i)
	{
		//msg_b_VCs
		msgsFromBitsT[i] = msg_b_VCs[nBitOrig +  i ];

	}
	if(toPrint)
	{
		//probsSimbs[i][k] /
		for(int i=0;i<probsSimbs[orig_s].size();++i)
		{
			std::cout << ": " << probsSimbs[orig_s][i];
		}
		std::cout << std::endl;
	}


	return calcSimbToBitsMsg.calcFuncsimbToBit(probsSimbs[orig_s], msgsFromBitsT, dst_b);
}

/**
 * dst_s de 0 até nSimbs-1
 * orig_b de 0 até nbits-1
 *
 * calculo da mensagem entre o bit orig_b até a função simb dst_s
 *
 * dst_b deve estar no intervalo orig_s*nBitsPerSimb até (orig_s+1)*nBitsPerSimb - 1
 */
double LdpcDecodeProcessBase::calcMsg_b_VC(int orig_b, int dst_s)
{
	// produto de todas as mensagens que chegam no bit dst_s
	// lista dos chk nodes que chegam neste bit...

	//std::vector<std::vector<double>> msg_ldpc_b; // mensagem em log likelihood
	//std::vector<std::vector<Msgs_arrive_bits_from_ldpc_st>> msgs_arrive_bit_from_ldpc;

	double ans = 0;
	for(auto &val : msgs_arrive_bit_from_ldpc[orig_b])
	{
		ans += *(val.ptrMsg);
	}
	return ans;
}

double LdpcDecodeProcessBase::calcMsg_b_fldpc(int orig_b, int dst_l)
{
	// quase igual ao anterior, mas somando a mensagem do VC

	double ans = 0;
	for(auto &val : msgs_arrive_bit_from_ldpc[orig_b])
	{
		if(val.LdpcOrig != dst_l)
		{
			ans += *(val.ptrMsg);
		}
	}

	ans += msg_VCs_b[orig_b];

	return ans;
}



double LdpcDecodeProcessBase::calcMsg_fldpc_b(int orig_l, int dst_b)
{
	double prod = 1;
	if(toPrint)
	{
		std::cout << "Messages arrived: " << orig_l ;
	}
	for(auto &val : msgs_arrive_ldpc[orig_l])
	{
		if(toPrint)
		{
			std::cout << " - " << std::setprecision(3) << *val.ptrMsg;
		}
		if(val.bitOrig != dst_b)
		{
			if(toPrint)
			{
				std::cout << " s";
			}
			prod *= tanh((*val.ptrMsg) / 2.0);
		}
		else
		{
			if(toPrint)
			{
				std::cout << " n";
			}
		}
	}

	if(toPrint)
	{
		std::cout << std::endl;
	}
	if(prod > 0.9999999999)
	{
		return 20;
	}
	if(prod < -0.99999999999)
	{
		return -20;
	}
	return 2.0*atanh(prod);
}

