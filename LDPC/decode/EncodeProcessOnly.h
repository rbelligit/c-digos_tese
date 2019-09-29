/*
 * EncodeProcessOnly.h
 *
 *  Created on: 13 de set de 2019
 *      Author: belli
 */

#ifndef DECODE_ENCODEPROCESSONLY_H_
#define DECODE_ENCODEPROCESSONLY_H_

#include <string>
#include <vector>

extern "C"
{
#include "mod2dense.h"
}
class EncodeProcessOnly {
public:
	EncodeProcessOnly(std::string genMatrixName, std::string pchMatrixName);
	virtual ~EncodeProcessOnly();

	int getSizeInput();
	int getSizeOutput();
	int doEncode(std::vector<bool> &input, std::vector<bool> &output);

private:
	int bitsBefore;
	int bitsAfter;

	char *sblk, *cblk, *chks;
	mod2dense *u = NULL, *v = NULL;
};

#endif /* DECODE_ENCODEPROCESSONLY_H_ */
