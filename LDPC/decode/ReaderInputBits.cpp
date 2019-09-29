/*
 * ReaderInputBits.cpp
 *
 *  Created on: 5 de dez de 2017
 *      Author: belli
 */

#include "ReaderInputBits.h"
#include <iostream>

ReaderInputBits::ReaderInputBits(std::string file_name) :
	stream(file_name.c_str())
{
	
}

ReaderInputBits::~ReaderInputBits()
{
	
}

bool ReaderInputBits::isOpen()
{
	return stream.is_open();
}

int ReaderInputBits::read_data(int size, std::vector<bool>* data)
{
	std::cout << "Size=" << size << std::endl;
	int pos = 0;
	std::vector<char> vals(size);
	while(size-pos > 0)
	{

		stream.read(&vals[0], size-pos);
		int readed = stream.gcount();
		if(readed < size-pos)
		{
			std::cout << "readed=" << readed << " - needed=" << size << "-" << pos << "=" << size-pos << std::endl;
			return -1;
		}
		int sizeM = size-pos;
		for(int i=0 ; i < sizeM;++i)
		{
			if(vals[i] == '0' || vals[i] == '1')
			{
				data->at(pos) = (vals[i]=='1');
				++pos;
			}
		}
	}
	return 0;

}
