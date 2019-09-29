/*
 * genfuncs.cpp
 *
 *  Created on: 7 de jan de 2017
 *      Author: belli
 */


#include "genfuncs.h"


std::deque<std::string> splitString(std::string arg, std::string splits)
{
	std::string::size_type pos;
	std::deque<std::string> dd;
	while((pos=arg.find_first_of(splits))!= std::string::npos)
	{
		std::string temp = arg.substr(0, pos);
		arg = arg.substr(pos+1);
		dd.push_back(temp);
	}
	if(arg.size() > 0)
	{
		dd.push_back(arg);
	}
	return dd;
}

void removeAllSpaces(char *str)
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

Printvectorbool::Printvectorbool(const std::vector<bool>& val) : vec(val){}

Printvectorbool::~Printvectorbool(){}

std::ostream& operator <<(std::ostream& str, const Printvectorbool& ptr)
{
	for(auto v : ptr.vec)
	{
		if(v)
		{
			str << "1";
		}
		else
		{
			str << "0";
		}
	}
	return str;
}

std::ostream &operator<<(std::ostream &str, const FileInputOptions_st &st)
{
	if(st.isFile)
	{
		str << "FILE: " << st.name;
	}
	else
	{
		str << "SEED: " << st.seed << " - len: " << st.size;
	}
	return str;

}

ReadDataInput::ReadDataInput(FileInputOptions_st* opt) :
		opt(opt), buffer(1000), fid(NULL),
		dist(0,255)
{
	currentSeed = opt->seed;
	sizeRemaining = opt->size;



	if(opt->isFile)
	{
		std::cerr << "Abrindo arquivo de entrada " << opt->name << std::endl;
		fid = fopen(opt->name.c_str(), "rb");
	}

	if(!opt->isFile)
	{
		std::cerr << "Inicializando geração randomica " << std::endl;
		randG.seed(opt->seed);
	}
}

ReadDataInput::~ReadDataInput()
{
}

int ReadDataInput::readData(int len, char* data)
{
	//std::cout << "isFile=" << opt->isFile << std::endl;
	if(!opt->isFile)
	{
		sizeRemaining -=len;
		if(sizeRemaining < 0)
		{
			std::cerr << "Erro - Size reamaining=" << sizeRemaining << std::endl;
			return -1;
		}
	}


	if(buffer.size() == 0)
	{
		bool ret = fillBuffer();
		if(!ret)
		{
			std::cerr << "Fill Buffer error=" << ret << std::endl;
			return -1;
		}
	}

	uint8_t val = buffer.front();
	val >>= posLastBit;
	int pos = 0;
	while(len > 0)
	{
		data[pos] = val & 1;
		++pos;
		val >>= 1;
		posLastBit++;
		if(posLastBit == 8)
		{
			posLastBit = 0;
			buffer.pop_front();
			if(buffer.size() == 0)
			{
				bool ret = fillBuffer();
				if(!ret)
				{
					return pos;
				}
			}
			val = buffer.front();
		}
		--len;
	}
	return pos;
}

bool ReadDataInput::fillBuffer()
{
	int readSize = buffer.capacity() - buffer.size();
	std::vector<uint8_t> data;
	data.resize(readSize);
	if(opt->isFile)
	{
		if(fid != NULL)
		{
			int ret = fread(&data[0], 1, readSize, fid);
			if(ret <= 0)
			{
				return false;
			}
			for(int i=0;i<ret;++i)
			{
				buffer.push_back(data[i]);
			}
		}
		else
		{
			std::cerr << "Erro here!!!!" << std::endl;
			return false;
		}
	}
	else
	{


		for(int i=0;i<readSize;++i)
		{
			uint8_t v = dist(randG);
			buffer.push_back(v);
		}
	}

	return true;

}



std::ostream &operator<<(std::ostream &str, const MAPOptions_st &opt)
{
	str << "NBITS: " << opt.nBits << std::endl;
	for(int i=0; i<(int)opt.map.size() ; ++i)
	{
		str << "MAP LEVEL " << i << " ";
		for(const auto &v : opt.map[i])
		{
			str << Printvectorbool(v) << " ";
		}
		str << std::endl;
	}
	return str;
}

