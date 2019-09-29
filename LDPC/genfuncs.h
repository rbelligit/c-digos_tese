/*
 * genFuncs.h
 *
 *  Created on: 7 de jan de 2017
 *      Author: belli
 */

#ifndef GENFUNCS_H_
#define GENFUNCS_H_

#include <string>
#include <deque>
#include <vector>
#include <ostream>
#include <boost/circular_buffer.hpp>
#include <functional>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/discrete_distribution.hpp>

std::deque<std::string> splitString(std::string arg, std::string splits);

class Printvectorbool
{
public:
	Printvectorbool(const std::vector<bool> &val);
	~Printvectorbool();
private:
	const std::vector<bool> &vec;
	friend std::ostream &operator<<(std::ostream &str, const Printvectorbool &ptr);
};

void removeAllSpaces(char *str);

std::ostream &operator<<(std::ostream &str, const Printvectorbool &ptr);



struct FileInputOptions_st
{
	std::string name;
	bool isFile;
	int seed;
	int64_t size;
};

std::ostream &operator<<(std::ostream &str, const FileInputOptions_st &st);

class ReadDataInput
{
public:
	ReadDataInput(FileInputOptions_st *opt);
	~ReadDataInput();
	int readData(int len, char *data);
private:

	bool fillBuffer();
	int64_t sizeRemaining;

	FileInputOptions_st *opt;
	int currentSeed;
	boost::circular_buffer<uint8_t> buffer;
	int posLastBit = 0;

	FILE *fid;

	boost::random::mt19937 randG;
	boost::random::uniform_int_distribution<> dist;
};

struct MAPOptions_st
{
	int nBits;
	int nLevels;
	// primeiro vector - qual nível
	// segundo vector  - qual mapeamento
	// terceiro vectorbool - bits do mapeamento
	std::vector<std::vector<std::vector<bool>>> map;
};

std::ostream &operator<<(std::ostream &str, const MAPOptions_st &opt);



struct ParseOptions_st
{
	const char *name;
	std::function<int (char *param, void *opts)> func;
};





#endif /* GENFUNCS_H_ */
