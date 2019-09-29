/*
 * ReaderLdpcImp.cpp
 *
 *  Created on: 19 de out de 2017
 *      Author: rbelli
 */

#include "ReaderLdpcImp.h"


#include <cstdio>
#include <algorithm>

extern "C"
{
#include "alloc.h"
#include "intio.h"
#include "open.h"
#include "mod2sparse.h"
#include "mod2dense.h"
#include "mod2convert.h"
}

ReaderLdpcImp::ReaderLdpcImp(std::string &H_filename):
	filename(H_filename)
{
	// TODO Auto-generated constructor stub
	readHMatrix();
}

ReaderLdpcImp::~ReaderLdpcImp()
{
	// TODO Auto-generated destructor stub
}

int ReaderLdpcImp::getMatrizHSize(int* rows, int* cols)
{
	if(matrizH == NULL)
	{
		return -1;
	}
	*rows = mod2sparse_rows(matrizH);
	*cols = mod2sparse_cols(matrizH);
	return 0;
}

int ReaderLdpcImp::getOnesInRow(int row, std::list<int>* ones)
{
	if(row < matrizH->n_rows)
	{
		mod2entry *entryIni = mod2sparse_first_in_row(matrizH, row);

		while(!mod2sparse_at_end(entryIni))
		{
			ones->push_back(entryIni->col);
			entryIni = mod2sparse_next_in_row(entryIni);
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

int ReaderLdpcImp::getOnesInCol(int col, std::list<int>* ones)
{
	if(col < matrizH->n_cols)
	{
		mod2entry *entryIni = mod2sparse_first_in_col(matrizH, col);

		while(!mod2sparse_at_end(entryIni))
		{
			ones->push_back(entryIni->row);
			entryIni = mod2sparse_next_in_col(entryIni);
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

int ReaderLdpcImp::getOnesInRow(int row, std::vector<int>* ones)
{
	std::list<int> vals;
	getOnesInRow(row, &vals);
	ones->resize(vals.size());

	std::copy(vals.begin(), vals.end(), ones->begin());

	return 0;
}

int ReaderLdpcImp::getOnesInCol(int col, std::vector<int>* ones)
{
	std::list<int> vals;
	getOnesInCol(col, &vals);
	ones->resize(vals.size());

	std::copy(vals.begin(), vals.end(), ones->begin());

	return 0;

}

int ReaderLdpcImp::readHMatrix()
{
	FILE *f;

  f = open_file_std(filename.c_str(),"rb");
  if (f==NULL)
  { fprintf(stderr,"Can't open parity check file: %s\n",filename.c_str());
	exit(1);
  }

  if (intio_read(f)!=('P'<<8)+0x80)
  { fprintf(stderr,"File %s doesn't contain a parity check matrix\n",filename.c_str());
	exit(1);
  }

  matrizH = mod2sparse_read(f);

  if (matrizH==0)
  { fprintf(stderr,"Error reading parity check matrix from %s\n",filename.c_str());
	exit(1);
  }

  return 0;
}
