/*
 * ReaderInputBits.h
 *
 *  Created on: 5 de dez de 2017
 *      Author: belli
 */

#ifndef DECODE_READERINPUTBITS_H_
#define DECODE_READERINPUTBITS_H_

#include <string>
#include <fstream>
#include <vector>

class ReaderInputBits
{
public:
	ReaderInputBits(std::string fileName);
	virtual ~ReaderInputBits();
	bool isOpen();

	int read_data(int size, std::vector<bool> *data);
private:
	std::ifstream stream;
};

#endif /* DECODE_READERINPUTBITS_H_ */
