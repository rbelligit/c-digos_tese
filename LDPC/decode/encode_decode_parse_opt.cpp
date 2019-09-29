#include "encode_decode_parse_opt.h"


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
	{"arqinp",
		[](char *param, void *opts1)->int
		{
			Options_st *opts = (Options_st *)opts1;
			std::string p = param;
			p.erase(std::remove_if(p.begin(), p.end(), [](char p) -> bool
					{
						if( ::isspace(p))
						{
							return true;
						}
						return false;
					}), p.end());

			if(p[0]=='<')
			{
				p.erase(std::remove_if(p.begin(), p.end(), [](char p) -> bool
				{
					if( p=='<' || p=='>')
					{
						return true;
					}
					return false;
				}), p.end());

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
				std::deque<std::string> ans = splitString(param, ":");
				if(ans.size() != 3)
				{
					std::cerr << "Erro no tratamento do noise-level" << std::endl;
					return -1;
				}
				Options_st *opts = (Options_st *)opts1;
				//opts->noise_level = std::strtod(param, NULL);
				opts->noise_level_ini = std::stod(ans[0]);
				opts->noise_level_end = std::stod(ans[1]);
				opts->noise_level_inc = std::stod(ans[2]);
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

int treatGetOptions(FILE *fid, Options_st *opts)
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


