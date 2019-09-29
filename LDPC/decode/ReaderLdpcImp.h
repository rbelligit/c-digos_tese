/*
 * ReaderLdpcImp.h
 *
 *  Created on: 19 de out de 2017
 *      Author: rbelli
 */

#ifndef DECODE_READERLDPCIMP_H_
#define DECODE_READERLDPCIMP_H_

#include "ReaderLdpcBase.h"

extern "C" {
#include "mod2sparse.h"
}

#include <string>
#include <list>


class ReaderLdpcImp: public ReaderLdpcBase
{
public:
	ReaderLdpcImp(std::string &H_filename);
	virtual ~ReaderLdpcImp();

	virtual int getMatrizHSize(int *rows, int *cols);
	virtual int getOnesInRow(int row, std::list<int> *ones);
	virtual int getOnesInCol(int col, std::list<int> *ones);
	virtual int getOnesInRow(int row, std::vector<int> *ones);
	virtual int getOnesInCol(int col, std::vector<int> *ones);
private:
	int readHMatrix();

	std::string filename;

	mod2sparse *matrizH;

};

#endif /* DECODE_READERLDPCIMP_H_ */
