/*
 * encode_decode_parse_opt.h
 *
 *  Created on: 4 de fev de 2018
 *      Author: belli
 */

#ifndef DECODE_ENCODE_DECODE_PARSE_OPT_H_
#define DECODE_ENCODE_DECODE_PARSE_OPT_H_

#include <genfuncs.h>
#include <ostream>

struct Options_st
{
	FileInputOptions_st input;
	std::string input_bit;
	std::string pchk;
	std::string genfile;
	std::vector<double> levelsQAM;
	MAPOptions_st mapQAM;
	double noise_level_ini;
	double noise_level_end;
	double noise_level_inc;
	std::string output; // resultado...
};

std::ostream &operator<<(std::ostream &str, const Options_st &st);

int treatGetOptions(FILE *fid, Options_st *opts);



#endif /* DECODE_ENCODE_DECODE_PARSE_OPT_H_ */
