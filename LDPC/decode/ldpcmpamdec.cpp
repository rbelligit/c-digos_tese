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


static void printUsage()
{
	std::cerr << "Decodifica utilizando LDPC depois faz o mapeamento MPAM com probabilidades passadas\n";

	std::cerr << "ldpcmpamdec <optionFile> " << std::endl;

	std::cerr << "Formato do arquivo <optionFile>:\n";
	std::cerr << "arqinp=<file> Arquivo codificado gerado pela função ldpcmpamenctrans\n";
	std::cerr << "arqinp_bit=<file> Arquivo com os bits utilizados para gerar os simbolos transmitidos\n"
			     "                   utilizado para verificar acerto na decodificacao" << std::endl;
	std::cerr << "pchk-file=<file> - Arquivo com dados da matriz de paridade\n"
			     "noise-level=<valor>\n"
			     "O <valor> é o desvio padrão do ruído a ser adicionado aos valores lidos do arqinp\n"
			     "e este valor será utilizado também nas fórmulas de decondificação.\n";
	std::cerr << "arqout=<file> " << std::endl;

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


struct Options_st
{
	FileInputOptions_st input;
	std::string input_bit;
	std::string pchk;
	std::string genfile;
	std::string output;
	std::vector<double> levelsQAM;
	MAPOptions_st mapQAM;
	double noise_level;
};

std::ostream &operator<<(std::ostream &str, const Options_st &st)
{
	str << "ENTRADA - " << st.input << std::endl;
	str << "pchkfile=" << st.pchk << "  |||  genfile=" << st.genfile << std::endl;
	str << "SAIDA FILE: " << st.output << std::endl;
	str << " nLevels=" << st.levelsQAM.size() << " ";
	for(auto &val:st.levelsQAM)
	{
		str << val << " ";
	}
	str << std::endl;
	str << st.mapQAM << std::endl;
	return str;
}



std::vector<ParseOptions_st> parseOpt =
{
	{"arqinp_bit",
		[](char *param, void *opts1) -> int
		{
			Options_st *opts = (Options_st *)opts1;
			opts->input_bit = param;
            return 0;
		}
	},
	{"arqinp",
		[](char *param, void *opts1)->int
		{
			Options_st *opts = (Options_st *)opts1;
			std::string p = param;
			p.erase(std::remove_if(p.begin(), p.end(), [](char p) -> bool
					{
						if(p=='<' || p=='>' || ::isspace(p))
						{
							return true;
						}
						return false;
					}), p.end());

			if(p[0]=='<')
			{
				std::deque<std::string> ans = splitString(p, ",");
				if(ans.size() != 2)
				{
					std::cerr << "Erro no tratamento da linha : " << param << std::endl;
					return -1;
				}
				std::cout << "ans[0]=" << ans[0] << " ans[1]=" << ans[1] << std::endl;
				opts->input.isFile = false;
				opts->input.seed = std::stoi(ans[0], 0, 10);
				opts->input.size = std::stoi(ans[1], 0, 10);
			}
			else
			{
				opts->input.name = p;
				opts->input.isFile = true;
			}
			return 0;
		}
    },
	{"pchk-file",
		[](char *param, void *opts1)->int
		{
			Options_st *opts = (Options_st *)opts1;
			opts->pchk = std::string(param);
			return 0;
		}
    },
	{"gen-file",
		[](char *param, void *opts1)->int
		{
			Options_st *opts = (Options_st *)opts1;
			opts->genfile = std::string(param);
			return 0;
		}},
	{"arqout",
		[](char *param, void *opts1)->int
		{
			Options_st *opts = (Options_st *)opts1;
			opts->output = std::string(param);
			return 0;
		}},
	{"noise-level",
			[](char *param, void *opts1)->int
			{
				Options_st *opts = (Options_st *)opts1;
				opts->noise_level = std::strtod(param, NULL);
				return 0;
			}},
};

static int getNewState(char *line)
{
	if( memcmp(line, "<QAM>", 5) == 0)
	{
		return 1;
	}
	else if( memcmp(line, "<MAP>", 5) == 0)
	{
		return 2;
	}
	else
	{
		return -1;
	}
	return -1;
}

static void removeAllSpacesDec(char *str)
{
	char *str2 = str;

	while(*str2 != '\0')
	{
		if(isspace(*str2) || !isprint(*str2) )
		{
			str2++;
		}
		else
		{
			if(str != str2)
			{
				*str = *str2;
			}
			str++;str2++;
		}
	}
	*str = '\0';
}

static int treatGetOptions(FILE *fid, Options_st *opts)
{
	char line[1024];
	int state = 0;

	// variáveis para STATE=1
	int stateTreatQAM = 0;
	int nLevelsQAM = 0;

	//variáveis para STATE=2

	int stateTreatMAP = 0;

	while(fgets(line, sizeof(line), fid) != NULL)
	{
		removeAllSpacesDec(line);
		//std::cerr << "Tratando linha : " << line << std::endl;
		int len = strlen(line);

		if(len >= 1 && line[0] != '#' && line[0] != ' ')
		{
			//std::cerr << "State=" << state << std::endl;
			switch(state)
			{
			case 0:
			{
				if(line[0]=='<')
				{
					int ret = getNewState(line);
					if(ret > 0)
					{
						state = ret;
					}
					break;
				}

				char *pointer;
				char *temp1 = strtok_r(line, "=", &pointer);
				char *temp2 = strtok_r(NULL, "=", &pointer);
				if(temp2 == NULL)
				{
					return -1;
				}

				for(auto &optp : parseOpt)
				{
					if(strcmp(optp.name, temp1) == 0)
					{
						int ret = optp.func(temp2, opts);
						if(ret < 0)
						{
							return ret;
						}
						break;
					}

				}
			}
			break;
			case 1:
			{
				//std::cerr << "stateTreatQAM=" << stateTreatQAM << std::endl;
				if(line[0]=='<')
				{
					int ret = getNewState(line);
					if(ret > 0)
					{
						state = ret;
					}
					break;
				}
				if(stateTreatQAM == 0)
				{
					int size = strtol(line, NULL, 10);
					if(size <= 0)
					{
						std::cerr << "Erro no número de níveis da modulação M-PAM" << std::endl;
						return -1;
					}
					std::cerr << "nLevels=" << size << std::endl;
					opts->levelsQAM.resize(size);
					stateTreatQAM = 1;
					nLevelsQAM = size;
					for(auto &val : opts->levelsQAM)
					{
						val = -1;
					}
				}
				else
				{
					if(stateTreatMAP-1 >= nLevelsQAM)
					{
						std::cerr << "Número de entradas de níveis maior que o atual número de níveis M-PAM = " << nLevelsQAM << std::endl;
						return -1;
					}
					opts->levelsQAM[stateTreatQAM-1] = strtod(line, NULL);
					stateTreatQAM++;
				}
			}
			break;

			case 2:
			{


				if(stateTreatMAP == 0)
				{
					opts->mapQAM.nBits = strtol(line, NULL, 10);
					stateTreatMAP++;
					opts->mapQAM.nLevels = opts->levelsQAM.size();
					opts->mapQAM.map.resize(opts->mapQAM.nLevels);
				}
				else
				{
					if(stateTreatMAP - 1 >= opts->mapQAM.nLevels)
					{
						std::cerr << "número de entradas no mapeamento maior que o número de níveis M-PAM" << std::endl;
						return -1;
					}
					std::deque<std::string> spt = splitString(line, ",");
					int i = 0;

					opts->mapQAM.map[stateTreatMAP-1].resize(spt.size());

					for(auto &r : spt)
					{
						if((int)r.size() != opts->mapQAM.nBits )
						{
							std::cerr << "número de bits no mapeamento maior que o número de bits" << std::endl;
							return -1;
						}
						opts->mapQAM.map[stateTreatMAP-1][i].resize(r.size());
						for(int k=0;k<(int)r.size();++k)
						{
							opts->mapQAM.map[stateTreatMAP-1][i][k]=(r[k]=='0')?false:true;;
						}
						++i;

					}
					stateTreatMAP++;
				}
			}
			break;
			}
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////
bool toPrint = false;

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

	//opts.pchk
	ReaderLdpcImp readerLdpc(opts.pchk);
	int rows, cols;
	readerLdpc.getMatrizHSize(&rows, &cols);

	std::cout << "Matriz H - Rows:" << rows << "  - cols:" << cols << std::endl;

	std::cout << "Noise level: " << opts.noise_level << std::endl;

	int nBitsPerSimb = opts.mapQAM.nBits;
	int nSimbPerFrame = cols / nBitsPerSimb;

	if(nSimbPerFrame * nBitsPerSimb != cols)
	{
		std::cout << "Erro.... Número de bits no frame não é multiplo do número de bits por símbolo. Aborting!!!" << std::endl;
		exit(0);
	}

	std::cout << "Numero de bits: " << cols << " - número de bits por símbolo: " << nBitsPerSimb << " - número de símbolos: " << nSimbPerFrame << std::endl;

	ReaderInputWNoise::Data_st dataOpts;
	dataOpts.levels = opts.levelsQAM;
	dataOpts.noiseLevel = opts.noise_level;
	dataOpts.seed = 0x1234123487654321ull;

	ReaderInputWNoise readerData(opts.input.name, &dataOpts);
	ReaderInputBits readerBits(opts.input_bit);

	LdpcDecodeProcessBase::Params_st paramsDec;

	if(!readerBits.isOpen())
	{
		std::cerr << "Erro abrindo arquivo com os bits da saida do LDPC... file=" << opts.input_bit << std::endl;
		exit(0);
	}

	paramsDec.levels = opts.levelsQAM;
	paramsDec.mapOpts = opts.mapQAM;
	paramsDec.nRows   = rows;
	paramsDec.nSimbs  = nSimbPerFrame;
	paramsDec.nbits   = cols;
	paramsDec.noise   = opts.noise_level;
	paramsDec.reader  = &readerLdpc;

	LdpcDecodeProcessBase ldpcDecode(&paramsDec);

	std::vector<double> dataIn(nSimbPerFrame);
	std::vector<bool> bitsReaded(cols);
	while(1)
	{
		dataIn.clear();
		// Le entrada.... símbolos que geram cols bits... Que são cols / nBitsPerSimb = nSimbPerFrame
		int ret1 = readerData.readNewData(nSimbPerFrame, &dataIn);
		if(ret1 != nSimbPerFrame)
		{
			std::cout << "Erro.... nenhum dado lido... - FIM DO PROCESSAMENTO" << std::endl;
			return 0;
		}

		ret1 = readerBits.read_data(cols, &bitsReaded);
		if(ret1 < 0)
		{
			std::cerr << "Erro lendo dados dos bits de saida do codificador LDPC " << std::endl;
			exit(0);
		}

		std::cout << "Lido entrada.... nSimb=" << ret1 << " dados lidos..." << std::endl;

		//for(auto &val : dataIn)
		//{
		//	std::cout << "D: " << val << std::endl;
		//}
		std::cout << "AFTER" << std::endl;

		/// Iniciar o processamento - - - - - -
		ldpcDecode.insertNewData(&dataIn, &bitsReaded);

		std::cout << "Inicio do processo de decodificação..." << std::endl;
		bool firsTime = true;
		for(int i=0;i<100;++i)
		{
			//toPrint = true;

			if(toPrint)
			{
				std::cout << "#############################################################################################" << std::endl;
				std::cout << "processMsgsSimbsToBits" << std::endl;
			}
			ldpcDecode.processMsgsSimbsToBits();
			if(firsTime)
			{
				int errosBits = 0;
				int oneNumber = 0;
				int errosParity = ldpcDecode.checkParity(&errosBits, &oneNumber);
				if(toPrint)
				{
					std::cout << "Erro = " << errosParity << " - errosBits=" << errosBits << " - uns=" << oneNumber << std::endl;
				}

			}
			firsTime = false;
			if(toPrint)
			{
				std::cout << "#############################################################################################" << std::endl;
				std::cout << "processMsgsBitsToLdpc" << std::endl;
			}
			ldpcDecode.processMsgsBitsToLdpc();

			if(toPrint)
			{
				std::cout << "#############################################################################################" << std::endl;
				std::cout << "processMsgsLdpcToBits" << std::endl;
			}
			ldpcDecode.processMsgsLdpcToBits();
			if(toPrint)
			{
				std::cout << "#############################################################################################" << std::endl;
				std::cout << "processMsgsBitsToSimbs" << std::endl;
			}
			ldpcDecode.processMsgsBitsToSimbs();
			if(toPrint)
			{
				std::cout << "#############################################################################################" << std::endl;
			}

			int errosBits = 0;
			int oneNumber = 0;
			int errosParity = ldpcDecode.checkParity(&errosBits, &oneNumber);

			//std::cout << "i=" << i << " - Erro = " << errosParity << " - errosBits=" << errosBits << " - uns=" << oneNumber << std::endl;
			std::cerr << "i=" << i << " - Erro = " << errosParity << " - errosBits=" << errosBits << " - uns=" << oneNumber << std::endl;
			if(errosParity == 0)
			{
				break;
			}
		}
		//break;
	}


	return 0;

	//opts.pchk

	// ReaderLdpcImp impReader(opts.pchk);


}
