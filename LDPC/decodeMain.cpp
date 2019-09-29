/*
 * decodeMain.cpp
 *
 *  Created on: 18 de jul de 2017
 *      Author: belli
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <string>
#include <algorithm>
#include <functional>
#include <deque>
#include "genfuncs.h"

static void printUsage()
{
	std::cerr << "Decodifica utilizando LDPC o sinal codificado com o ldpcmpamenctrans.cpp\n";

	std::cerr << "decodeMain <optionFile> " << std::endl;

	std::cerr << "Formato do arquivo <optionFIle>:\n";
	std::cerr << "arqinp=<file> indica o nome do arquivo que será decodificado>\n";
	std::cerr << "pchk-file=<file>\n"
			     "gen-file=<file>\n";
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

struct MAPOptions_st
{
	int nBits;
	int nLevels;
	// primeiro vector - qual nível
	// segundo vector  - qual mapeamento
	// terceiro vectorbool - bits do mapeamento
	std::vector<std::vector<std::vector<bool>>> map;
};

struct Options_st
{
	std::string input;
	std::string pchk;
	std::string genfile;
	std::string output;
	std::vector<double> levelsQAM;
	MAPOptions_st mapQAM;
};

struct ParseOptions_st
{
	const char *name;
	std::function<int (char *param, Options_st *opts)> func;
};

std::vector<ParseOptions_st> parseOpt =
{
	{"arqinp",
		[](char *param, Options_st *opts)->int
		{
		std::string p = param;
		p.erase(std::remove_if(p.begin(), p.end(), [](char p) -> bool
			{
				if(p=='<' || p=='>' || ::isspace(p))
				{
					return true;
				}
				return false;
			}), p.end());
			opts->input = p;
		}
	},
	{"", }
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
		removeAllSpaces(line);
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

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printUsage();
		return -1;
	}

	FILE *fid = fopen(argv[1], "r");

	if(fid==NULL)
	{
		printUsage();
		return -1;
	}

	Options_st opt;

	int ret = treatGetOptions(fid, &opt);
	if(ret < 0)
	{
		printUsage();
		return -1;
	}

	std::cout << opt << std::endl;

}

