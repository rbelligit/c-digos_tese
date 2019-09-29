/*
 * analise_matriz_h.cpp
 *
 *  Created on: 1 de jan de 2018
 *      Author: belli
 */

#include <iostream>
#include <iomanip>
#include <string>

#include "decode/ReaderLdpcImp.h"


void printUsage()
{
	std::cout << "Usage: analize_matriz_h <arquivo>" << std::endl;
	exit(0);
}

int main(int argc, char *argv[])
{

	if(argc != 2)
	{
		printUsage();
	}

	std::string filename(argv[1]);

	ReaderLdpcImp imp(filename);

	int rows, cols;
	imp.getMatrizHSize(&rows, &cols);

	std::cout << "Tamanho da matriz H - rows=" << rows << " - cols=" << cols << std::endl;

	for(int i=0;i<rows;++i)
	{
		// pega o número de ones por linha
		std::list<int> onesInRow;
		imp.getOnesInRow(i, &onesInRow);
		std::cout << "Row (" << std::setw(4) << i << ") - ones=" << onesInRow.size() << std::endl;
	}

	for(int i=0;i<cols;++i)
	{
		// pega o número de ones por linha
		std::list<int> onesInCols;
		imp.getOnesInCol(i, &onesInCols);
		std::cout << "Col (" << std::setw(4) << i << ") - ones=" << onesInCols.size() << std::endl;
	}
	return 0;
}



