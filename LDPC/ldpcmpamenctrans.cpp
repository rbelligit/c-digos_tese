/*
 * ldpcmpamenctrans.cpp
 *
 *  Created on: 6 de jan de 2017
 *      Author: belli
 */

#include "ldpcmpamenctrans.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "genfuncs.h"

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
	std::cerr << "Codifica utilizando LDPC depois faz o mapeamento MPAM com probabilidades passadas\n";

	std::cerr << "ldpcmpamenctrans <optionFile> " << std::endl;

	std::cerr << "Formato do arquivo <optionFIle>:\n";
	std::cerr << "arqinp=<file> - \"<seed,size>\" indica gerar aleatoriamente com semente seed e tamanho size>\n";
	std::cerr << "pchk-file=<file>\n"
			     "gen-file=<file>\n";
	std::cerr << "arqout=<file> " << std::endl;
	std::cerr << "arqout_bit=<file> - arquivo com os dados antes do mapeamento, para conferencia de erros na recepção\n" << std::endl;

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
	std::string pchk;
	std::string genfile;
	std::string output;
	std::string arqout_bit;
	std::vector<double> levelsQAM;
	MAPOptions_st mapQAM;
};

std::ostream &operator<<(std::ostream &str, const Options_st &st)
{
	str << "ENTRADA - " << st.input << std::endl;
	str << "pchkfile=" << st.pchk << "  |||  genfile=" << st.genfile << std::endl;
	str << "SAIDA FILE: " << st.output << std::endl;
	str << "saida FILE bit = " << st.arqout_bit << std::endl;
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
	ParseOptions_st{"arqinp",
		[](char *param, void *opts1)->int
		{
			Options_st *opts = (Options_st *)opts1;
			std::string p = param;
			p.erase(std::remove_if(p.begin(), p.end(), [](char p) -> bool
					{
						if(::isspace(p))
						{
							return true;
						}
						return false;
					}), p.end());

			bool isNotFile = false;
			if(p[0]=='<')
			{
				isNotFile = true;
			}

			p.erase(std::remove_if(p.begin(), p.end(), [](char p) -> bool
								{
									if(p=='<' || p=='>' || ::isspace(p))
									{
										return true;
									}
									return false;
								}), p.end());

			if(isNotFile)
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
		}},
	ParseOptions_st{"pchk-file",
		[](char *param, void *opts1)->int
		{
			Options_st *opts = (Options_st *)opts1;
			opts->pchk = std::string(param);
			return 0;
		}},
	ParseOptions_st{"gen-file",
		[](char *param, void *opts1)->int
		{
			Options_st *opts = (Options_st *)opts1;
			opts->genfile = std::string(param);
			return 0;
		}},
	ParseOptions_st{"arqout",
		[](char *param, void *opts1)->int
		{
			Options_st *opts = (Options_st *)opts1;
			opts->output = std::string(param);
			return 0;
		}},
	ParseOptions_st{"arqout_bit",
		[](char *param, void *opts1)->int
		{
			Options_st *opts = (Options_st *)opts1;
			opts->arqout_bit = std::string(param);
			return 0;
		}
		},

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

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

using MapEntry_t = std::vector<bool>;

//static bool operator<(MapEntry_t &e1, MapEntry_t &e2)
//{
//	int s1 = e1.size();
//	int s2 = e2.size();
//	if(s1 != s2)
//	{
//		std::cerr << "Erro na comparação das entradas do MAP entry. Wrong size" << std::endl;
//		exit(1);
//	}
//	for(int i=0;i<s1;++i)
//	{
//		if(e1[i]==false && e2[i]==true)
//		{
//			return true;
//		}
//	}
//	return false;
//}

using MapEntryToLevel_t = std::unordered_map<MapEntry_t, int>;

class DoMapClass
{
public:
	DoMapClass(Options_st *opts);
	~DoMapClass();
	int processData(char *data, int size);
private:
	Options_st *opts;

	MapEntryToLevel_t mapent;

	FILE *fid;
};

DoMapClass::DoMapClass(Options_st *opts) : opts(opts)
{
	fid = fopen(opts->output.c_str(), "wb");
	if(fid==NULL)
	{
		std::cerr << "Erro ao abrir o arquivo : " << opts->output << std::endl;
		exit(1);
	}
}

DoMapClass::~DoMapClass()
{
	fclose(fid);
}

int DoMapClass::processData(char *data, int size)
{
	std::cout << "Processing data size=" << size << std::endl;
	int nBits = opts->mapQAM.nBits;
	int nMaps = size / nBits;

	FILE *fidBit = fopen(opts->arqout_bit.c_str(), "a+");
	if(fidBit == NULL)
	{
		std::cerr << "Erro abrindo arquivo para gravar bits . " << std::endl;
		exit(0);
	}

	//std::cout << "nBits=" << nBits << std::endl;

	for(int i=0;i<size;++i)
	{
		fprintf(fidBit, "%c", data[i]==0?'0':'1');
	}

	fclose(fidBit);

	// um pacote de dados.... um frame do LDPC

	//std::cout << "nMaps=" << nMaps << std::endl;

	auto map1 = opts->mapQAM.map;

	for(unsigned int i=0;i<map1.size();++i)
	{
		for(unsigned int k=0;k<map1[i].size();++k)
		{
			mapent.insert(std::pair<MapEntry_t, int>(map1[i][k], i));
		}
	}

	std::vector<bool> sub;
	sub.resize(nBits);
	std::vector<uint8_t> levels;
	levels.resize(nMaps);
	for(int i=0;i<nMaps;++i)
	{
		//std::cout << "MAP :";
		for(int k=0;k<nBits;++k)
		{
			sub[k]=(data[i*nBits+nBits-k-1] != 0);
			//std::cout << (char *)(sub[k]?"1":"0");
		}
		auto ptr = mapent.find(sub);
		if(ptr == mapent.end())
		{
			std::cerr << "Mapeamento não encontrado.... Erro " << sub << std::endl;
			exit(1);
		}

		levels[i]=ptr->second;
		//std::cout << " -> " << (int)levels[i] << std::endl;

	}
	fwrite(&levels[0], levels.size(), 1, fid);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

static int doEncodeLdpc(Options_st *opts, ReadDataInput *data, DoMapClass *map)
{
	char *sblk, *cblk, *chks;
	mod2dense *u = NULL, *v = NULL;
	if (type=='d')
	{
		u = mod2dense_allocate(N-M,1);
		v = mod2dense_allocate(M,1);
	}
	else if (type=='m')
	{
		u = mod2dense_allocate(M,1);
		v = mod2dense_allocate(M,1);
	}
	else if(type=='s')
	{
		std::cout << "Fazendo o sparse encode..." << std::endl;
	}
	else
	{
		std::cerr << "Erro Type='d' ou type='n'  --->" << (int)type << " - " << type  << std::endl;
		return -1;
	}

	std::cout << "Iniciando processamento ()N=" << N << " M=" << M << " N-M=" << N-M << std::endl;

	sblk = (char *)chk_alloc (N-M, sizeof *sblk);
	cblk = (char *)chk_alloc (N, sizeof *cblk);
	chks = (char *)chk_alloc (M, sizeof *chks);

	for(int n=0;;++n)
	{
            int ret = data->readData(N-M, sblk);
            std::cout << "Lendo " << N-M << "bytes ret=" << ret << std::endl;
            if(ret != N-M)
            {
                    std::cerr << "Finalizado... " << std::endl;
                    break;
            }
            

            switch (type)
            {
                    case 's':
                    {
                            sparse_encode(sblk, cblk);
                            break;
                    }
                    case 'd':
                    {
                            dense_encode(sblk, cblk, u, v);
                            break;
                    }
                    case 'm':
                    {
                            mixed_encode(sblk, cblk, u, v);
                            break;
                    }
            }

            /* Check that encoded block is a code word. */

            mod2sparse_mulvec(H, cblk, chks);

            for (int i = 0; i < M; i++)
            {
                    if (chks[i] == 1)
                    {
                            fprintf(stderr,	"Output block %d is not a code word!  (Fails check %d)\n",
                                            n, i);
                            abort();
                    }
            }

            map->processData(cblk, N);


	}

	std::cout << "Aqui..... terminou doEncodeLdpc" << std::endl;


	return 0;
}




int main(int argc, char *argv[])
{
	Options_st options;
	if(argc != 2)
	{
		printUsage();
		return 0;
	}

	FILE *foption = fopen(argv[1], "r");
	if(foption == NULL)
	{
		std::cerr << "Erro ao abrir o arquivo de opções " << argv[1] << "\n" << std::endl;
		printUsage();
		return 0;
	}

	int ret = treatGetOptions(foption, &options);
	fclose(foption);
	if(ret < 0)
	{
		std::cerr << "Erro fazendo parse no arquivo de opções" << std::endl << std::endl;
		printUsage();
		return 0;
	}

	unlink(options.arqout_bit.c_str());

	std::cout << "OPTIONS:\n" << options << std::endl;

	/**
	 * READ PARITY CHECK MATRIX.  Sets the H, M, and N global variables.  If an
     * error is encountered, a message is displayed on standard error, and the
     * program is terminated
     *
     * Estes parâmetros são gerados pela função: make-ldpc e salvos
     * em um arquivo. A função abaixo lê os arquivo.
     *
	 */
	read_pchk((char *)options.pchk.c_str());

	if (N<=M)
	{
		std::cerr << "N <= M -> ERRO, não da para codificar se o número de bits " << N << " não for maior que o número de checks " << M << std::endl;
		return 0;
	}

	/**
	 *  READ GENERATOR MATRIX.  The parity check matrix must have already been
   read, unless the last argument is set to 1.  The generator matrix must be
   compatible with the parity check matrix, if it has been read.  If the
   second argument is 1, only the column ordering (the last N-M of which are
   the indexes of the message bits) is read, into the 'cols' global variable.
   Otherwise, everything is read, into the global variables appropriate
   to the representation.  The 'type' global variable is set to a letter
   indicating which represention is used.
     *
     * A matriz geradora é gerada pela função: make-gen
	 */
	read_gen((char *)options.genfile.c_str(), 0, 0);

	/**
	 * Classe que gerência a leitura do arquivo de dados ou a gerção pseudo aleatoria dos dados
	 *
	 * Se quiser o arquivo de entrada pode ser gerado pela função: genInputFile
	 */
	ReadDataInput readData(&options.input);

	DoMapClass mapC(&options);

	std::cout << "Do encode... " << std::endl;

	doEncodeLdpc(&options, &readData, &mapC);



	return 1;

}



