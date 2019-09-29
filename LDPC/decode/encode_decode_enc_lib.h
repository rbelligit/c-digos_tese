/*
 * encode_decode_enc_lib.h
 *
 *  Created on: 4 de fev de 2018
 *      Author: belli
 */

#ifndef DECODE_ENCODE_DECODE_ENC_LIB_H_
#define DECODE_ENCODE_DECODE_ENC_LIB_H_

#include "encode_decode_parse_opt.h"

#include <vector>
#include <unordered_map>

extern "C"
{
#include "mod2dense.h"
}

using MapEntry_t = std::vector<bool>;
using MapEntryToLevel_t = std::unordered_map<MapEntry_t, int>;

class DoMapClass
{
public:
	DoMapClass(Options_st *opts);
	~DoMapClass();
	int processData(char *data, int size, std::vector<uint8_t> &levels);
private:
	Options_st *opts;

	MapEntryToLevel_t mapent;

};

class Do_encode_class
{
public:
	Do_encode_class(DoMapClass *map, Options_st *opts, ReadDataInput *data);

	int do_encode(std::vector<uint8_t> &encoded, std::vector<bool> &bits);
private:
	DoMapClass *map;
	Options_st *opts;
	ReadDataInput *data;

	char *sblk, *cblk, *chks;
	mod2dense *u = NULL, *v = NULL;
};




#endif /* DECODE_ENCODE_DECODE_ENC_LIB_H_ */
