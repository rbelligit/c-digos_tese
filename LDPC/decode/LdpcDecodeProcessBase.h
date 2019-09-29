/*
 * LdpcDecodeProcessBase.h
 *
 *  Created on: 8 de nov de 2017
 *      Author: belli
 */

#ifndef DECODE_LDPCDECODEPROCESSBASE_H_
#define DECODE_LDPCDECODEPROCESSBASE_H_

#include "ReaderLdpcBase.h"
#include <vector>
#include <list>
#include <genfuncs.h>

#include "CalcSimbToBitsMsg.h"


class LdpcDecodeProcessBase {
public:
	struct Params_st
	{
		int nbits;
		int nSimbs;
		int nRows;
		double noise;
		std::vector<double> levels;
		MAPOptions_st mapOpts;

		// Matriz:
		ReaderLdpcBase *reader;
	};


	LdpcDecodeProcessBase(Params_st *params);
	virtual ~LdpcDecodeProcessBase();

	/**
	 * precisa ser nSimbs dados... Caso contrário um erro será retornado.
	 * Pre-processamento inicial apenas.
	 */
	int insertNewData(std::vector<double> *data, std::vector<bool> *bits);

	/**
	 * Processa mensagens dos símbolos para os bits
	 */
	int processMsgsSimbsToBits();

	/**
	 * Processa mensagens dos bits para os símbolos
	 */
	int processMsgsBitsToSimbs();

	/**
	 * Processa mensagens dos bits para os chkparityldpc
	 */
	int processMsgsBitsToLdpc();

	/**
	 * Processa mensagens dos chkparityldpc para os bits
	 */
	int processMsgsLdpcToBits();

	/**
	 * Checa se paridade esta certa...
	 * Retorna o número de erros da paridade.
	 * Na variável erros_w_transm guarda os erros comparando com o que realmente foi transmitido.
	 */
	int checkParity(int *erros_w_transm, int *oneNumber);


protected:
	Params_st *params;
	int nBitsPerSimb;

	void parseMatrix();

	std::vector<std::vector<int>> posOneInCols;
	std::vector<std::vector<int>> posOneInRows;



	std::vector<std::vector<double>> probsSimbs; //probsSimbs[nSimbs][nLevels]

	std::vector<bool> bitsCoded;

	////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 * Mensagens VC_s -> b
	 * índice do vetor é o bit destino - 0 até nbits-1
	 *
	 * Tamanho nbits
	 * -> não necessita ser inicializado
	 */
	std::vector<double> msg_VCs_b;

	void init_msg_VCs_b();

	////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 * Mensagens b-> VC_s
	 * índice é o b
	 * tamanho nbits
	 */
	std::vector<double> msg_b_VCs;

	void init_msg_b_VCs();


	////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 * Mensagens b -> ldpc
	 *
	 * dois índices ->
	 *   -> um o bit de origem 0 -> até nbits-1
	 *   -> outro é o o índice do uns nas colunas (posOneInCols[bit][->>pos<<-])
	 */
	std::vector<std::vector<double>> msg_b_ldpc;

	struct Msgs_arrive_ldpc_st
	{
		int bitOrig;
		double *ptrMsg;
	};
	/**
	 * Vetor com dois índices, para busca contrária de mensagens
	 * que chegam na função ldpc
	 * 2 índices -> 1 -> ldpc
	 *              2 -> bits ligados a este nó (uns na tabela posOneInRows)
	 */
	std::vector<std::vector<Msgs_arrive_ldpc_st>> msgs_arrive_ldpc;

	void init_msg_b_ldpc();

	////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 * Mensagens ldpc -> b   em log likelihood
	 *
	 * dois indices ->
	 *   -> um a linha ldpc de origem 0 até nRows-1
	 *   -> Qual um (posição do um) na tabela posOneInRows[ldpc][->>pos<<--]
	 *
	 */
	std::vector<std::vector<double>> msg_ldpc_b; // mensagem em log likelihood

	struct Msgs_arrive_bits_from_ldpc_st
	{
		int LdpcOrig;
		double *ptrMsg;
	};

	/**
	 * Vetor com dois índices, para busca contrária de mensagens
	 * que chegam nos bits da função ldpc
	 * 2 índices -> 1 -> bit
	 *              2 -> funcoes ldpc ligados a este nó (uns na tabela posOneInCols)
	 */
	std::vector<std::vector<Msgs_arrive_bits_from_ldpc_st>> msgs_arrive_bit_from_ldpc;

	void init_msg_ldpc_b();

	/*******************************************************************************************************
	 * FIM DA DEFINIÇÃO DE MENSAGENS, Ínicio das funções que calculam (update) as mensagens e inicializações
     */



	/**
	 *
	 */
	double calcNormProb(double rec, double level);

	/**
	 * Inicializa todas as mensagens
	 * Variáveis:
	 *  -msg_VCs_b
	 *  -msg_b_VCs
	 */
	void initializeAllMsgs(void);

	////////////////////////////////////////////////
	// Todas as funções abaixo retornam o log likelihood pois as variáveis podem
	// ser apenas 1 ou 0


	/**
	 * orig_s de 0 até nSimbs-1
	 * dst_b de 0 até nbitsperSimb-1
	 * Resposta em log likelihood....
	 *
	 * mensagem calculada da origem -> função símbolo orig_s
	 * para o bit dst_b
	 *
	 * dst_b deve estar no intervalo orig_s*nBitsPerSimb até (orig_s+1)*nBitsPerSimb - 1
	 */
public:
	double calcMsg_VC_b(int orig_s, int dst_b);

protected:

	/**
	 * dst_s de 0 até nSimbs-1
	 * orig_b de 0 até nbits-1
	 *
	 * calculo da mensagem entre o bit orig_b até a função simb dst_s
     *
	 * dst_b deve estar no intervalo orig_s*nBitsPerSimb até (orig_s+1)*nBitsPerSimb - 1
	 */
	double calcMsg_b_VC(int orig_b, int dst_s);

	/**
	 * dst_l de 0 até nRows-1
	 * orig_b de 0 até nbits-1
	 * mensagem calculada do bit orig_b até a função ldpc dst_l
	 */
	double calcMsg_b_fldpc(int orig_b, int dst_l);

	/**
	 * orig_l de 0 até nRows-1
	 * dst_b de 0 até nbits-1
	 *
	 * mensagem calculada da função ldpc dst_l até o bit dts_b
	 */
	double calcMsg_fldpc_b(int orig_l, int dst_b);

	//////////////////////////////////////////////////////////////
	CalcSimbToBitsMsg calcSimbToBitsMsg;
};

#endif /* DECODE_LDPCDECODEPROCESSBASE_H_ */
