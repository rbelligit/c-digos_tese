/*
 * ReaderLdpcBase.h
 *
 *  Created on: 19 de out de 2017
 *      Author: rbelli
 */

#ifndef DECODE_READERLDPCBASE_H_
#define DECODE_READERLDPCBASE_H_

#include <list>
#include <vector>

class ReaderLdpcBase {
public:
	ReaderLdpcBase(){};
	virtual ~ReaderLdpcBase(){};
	virtual int getMatrizHSize(int *rows, int *cols) = 0;
	virtual int getOnesInRow(int row, std::list<int> *ones) = 0;
	virtual int getOnesInCol(int col, std::list<int> *ones) = 0;
	virtual int getOnesInRow(int row, std::vector<int> *ones) = 0;
	virtual int getOnesInCol(int col, std::vector<int> *ones) = 0;

};

#endif /* DECODE_READERLDPCBASE_H_ */
