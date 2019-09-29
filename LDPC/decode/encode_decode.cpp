/*
 * encode_decode.cpp
 *
 *  Created on: 4 de fev de 2018
 *      Author: belli
 */

/*
 * ldpcmpamdec.cpp
 *
 *  Created on: Aug 16, 2017
 *      Author: belli
 */


#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <genfuncs.h>

#include "ReaderLdpcImp.h"
#include "ReaderInputWNoise.h"
#include "LdpcDecodeProcessBase.h"
#include "ReaderInputBits.h"
#include "encode_decode_parse_opt.h"
#include "encode_decode_enc_lib.h"

extern "C"
{
#include "mod2sparse.h"
#include "mod2dense.h"
#include "mod2convert.h"
#include "enc.h"
#include "rcode.h"
#include "alloc.h"

#include "open.h"
}


static void printUsage()
{
	std::cerr << "Codifica e Decodifica utilizando LDPC depois faz o mapeamento MPAM com probabilidades passadas\n";

	std::cerr << "encode_decode <optionFile> " << std::endl;

	std::cerr << "Formato do arquivo <optionFile>:\n";
	std::cerr << "arqinp=<file> - \"<seed,size>\" indica gerar aleatoriamente com semente seed e tamanho size>\n";
	std::cerr << "arqinp_bit=<file> Arquivo com os bits utilizados para gerar os simbolos transmitidos\n"
			     "                   utilizado para verificar acerto na decodificacao" << std::endl;
	std::cerr << "pchk-file=<file> - Arquivo com dados da matriz de paridade\n"
			     "noise-level=<valor_ini>:<valor_fim>:<n_entradas> - snr relativo a sinal de potência 1\n"
			     "O <valor> é o desvio padrão do ruído a ser adicionado aos valores lidos do arqinp\n"
			     "e este valor será utilizado também nas fórmulas de decondificação.\n";


	std::cerr << "Após o <QAM> teremos entrada definindo como será os níveis da modulação M-PAM." << std::endl;
	std::cerr << "O formato após o QAM será: " << std::endl;
	std::cerr << " <número de níveis M-PAM> " << std::endl;
	std::cerr << " <nível 1> <nível 2> ... <nível M>" << std::endl;

	std::cerr << "Após o <MAP> teremos entrada definindo como serão os mapeamentos para cada nível M-PAM." << std::endl;
	std::cerr << "O formato após o MAP será: " << std::endl;
	std::cerr << " <número de bits por mapeamento> " << std::endl;
	std::cerr << " XXXXXX,YYYYYY,ZZZZZZ,....   - uma linha por nível M-PAM com os mapeamentos para este nível" << std::endl;
	std::cerr << " os XXXXXX e YYYY, etx tem <número de bits por mapeamento> bits" << std::endl;

}

bool toPrint=false;
bool toPrint2 = false;

struct Result_opts_st
{
	int maxErr;
	int minIter;

	int totBits;
	int totError;
	int totErrorSemFec;
};

static int do_process(Options_st &opts, Result_opts_st *res, double snr);

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printUsage();
		exit(0);
	}

	FILE *fid = fopen(argv[1], "r");
	if(fid == NULL)
	{
		std::cout << "Erro abrindo arquivo : " << argv[1] << std::endl;
		std::cout << std::endl;
		printUsage();
		exit(0);
	}
	Options_st opts;
	int ret = treatGetOptions(fid, &opts);
	if(ret < 0)
	{
		std::cout << "Erro trantando arquivo de opções : " << argv[1] << "\n\n" << std::endl;
		printUsage();
		exit(0);
	}

	fclose(fid);
	read_pchk((char *)opts.pchk.c_str());
	read_gen((char *)opts.genfile.c_str(), 0, 0);

	Result_opts_st res;
	res.maxErr = 15000;
	res.minIter = 10;
	double snr=opts.noise_level_ini;
	double incNoise = opts.noise_level_end - opts.noise_level_ini;
	incNoise = opts.noise_level_inc;
	std::cout << "##### ->   N=" << N << " - M=" << M << " - incNoise=" << incNoise << " ini=" << snr << " - end=" << opts.noise_level_end << std::endl;
	int countFimBef = 0;

	while(snr < opts.noise_level_end && countFimBef < 5)
	{
		//std::cout << "###############################################################################" << std::endl;
		//std::cout << "Noise=" << snr << std::endl;
		do_process(opts, &res, snr);
		std::cout << "Result: Noise=" << snr << " Bits: " << res.totBits << " - errors=" << res.totError << " Taxa=" << (double) res.totError/ (double)res.totBits <<
				" - taxaSemFec=" << (double) res.totErrorSemFec/ (double)res.totBits << std::endl;

		double taxa = (double) res.totError/ (double)res.totBits;
		if(taxa < 1e-6)
		{
			++countFimBef;
		}

		snr+=incNoise;
	}

	return 0;
}

#ifndef pow10
#define pow10(x) pow(10,x)
#endif


static int do_process(Options_st &opts, Result_opts_st *res, double snr)
{
	// snr = 10*log10(sinal/ruido) -> 10^(snr/10) = sinal / ruido -> ruido = sinal * 10^(-snr/10)
	double noise = pow10(-snr/10); // utilizado 10 para seguir a formula do artigo
	ReaderLdpcImp readerLdpc(opts.pchk);

	//int nbits_input = N-M; // numero de bits de entrada por processamento

	ReadDataInput readData(&opts.input);
	DoMapClass mapC(&opts);

	int nBitsPerSimb = opts.mapQAM.nBits;
	int nSimbPerFrame = N / nBitsPerSimb;

	LdpcDecodeProcessBase::Params_st paramsDec;
	paramsDec.levels = opts.levelsQAM;
	paramsDec.mapOpts = opts.mapQAM;
	paramsDec.nRows   = M;
	paramsDec.nSimbs  = nSimbPerFrame;
	paramsDec.nbits   = N;
	paramsDec.noise   = noise;
	paramsDec.reader  = &readerLdpc;
	LdpcDecodeProcessBase ldpcDecode(&paramsDec);

	ReaderInputWNoise::Data_st dataReaderInput;
	dataReaderInput.levels = opts.levelsQAM;
	dataReaderInput.noiseLevel = noise;
	dataReaderInput.seed = 0x1234123487654321ull + floor(dataReaderInput.noiseLevel*10000);
	ReaderInputWNoise addNoiseCl("", &dataReaderInput);

	Do_encode_class encode(&mapC, &opts, &readData);
	std::vector<uint8_t> encodeData;
	std::vector<bool> bitsGen;
	std::vector<double> recData;
	encodeData.resize(nSimbPerFrame);
	bitsGen.resize(N);
	recData.resize(nSimbPerFrame);

	res->totBits = 0;
	res->totError = 0;
	res->totErrorSemFec = 0;



	while(1)
	{
		std::cerr << "Antes do inicio do encode" << std::endl;
		int ret = encode.do_encode(encodeData, bitsGen);
		if(ret < 0)
		{
			std::cout << "Encode=" << ret << std::endl;
			break;
		}

		std::cerr << "Depois do encode" << std::endl;

		res->totBits+=N;
		//// Adicionar ruído nos dados...
		addNoiseCl.readNewData(encodeData, &recData);
		std::cerr << "antes do ldpcDecode.insertNewData" << std::endl;
		ldpcDecode.insertNewData(&recData, &bitsGen);

		std::cerr << "Antes do for" << std::endl;

		bool firsTime = true;
		int errosBits = 0;
		int oneNumber = 0;
		for(int i=0;i<200;++i)
		{
			ldpcDecode.processMsgsSimbsToBits();
			if(firsTime)
			{
				int errosBits = 0;
				int oneNumber = 0;
				int errosParity = ldpcDecode.checkParity(&errosBits, &oneNumber);
				res->totErrorSemFec += errosBits;
				if(toPrint)
				{
					std::cout << "Erro = " << errosParity << " - errosBits=" << errosBits << " - uns=" << oneNumber << std::endl;
				}

			}
			firsTime = false;
			if(toPrint)
			{
				std::cerr << "#############################################################################################" << std::endl;
				std::cerr << "processMsgsBitsToLdpc" << std::endl;
			}
			ldpcDecode.processMsgsBitsToLdpc();

			if(toPrint)
			{
				std::cerr << "#############################################################################################" << std::endl;
				std::cerr << "processMsgsLdpcToBits" << std::endl;
			}
			ldpcDecode.processMsgsLdpcToBits();

			if(toPrint)
			{
				std::cerr << "#############################################################################################" << std::endl;
				std::cerr << "processMsgsBitsToSimbs" << std::endl;
			}
			ldpcDecode.processMsgsBitsToSimbs();

			if(toPrint)
			{
				std::cerr << "#############################################################################################" << std::endl;
			}

			errosBits = 0;
			oneNumber = 0;
			int errosParity = ldpcDecode.checkParity(&errosBits, &oneNumber);

			//std::cout << "i=" << i << " - Erro = " << errosParity << " - errosBits=" << errosBits << " - uns=" << oneNumber << std::endl;
			if(toPrint2)
			{
				std::cerr << "i=" << i << " - Erro = " << errosParity << " - errosBits=" << errosBits << " - uns=" << oneNumber << std::endl;
			}
			if(errosParity == 0)
			{
				break;
			}
		}
		std::cerr << "sim " << res->totBits / N << " - erros=" << errosBits << " - totalBit=" << res->totBits << " - totalErr=" << res->totError  << std::endl;
		res->totError+=errosBits;

		if(res->totError > res->maxErr && res->totBits > (res->minIter*N))
		{
			break;
		}
	}
	return 0;
}


