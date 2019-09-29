/*
 * EncodeProcessOnly.cpp
 *
 *  Created on: 13 de set de 2019
 *      Author: belli
 */

#include "EncodeProcessOnly.h"
#include <iostream>

extern "C"
{
#include "mod2sparse.h"
#include "mod2dense.h"
#include "mod2convert.h"
#include "enc.h"
#include "rcode.h"
#include "alloc.h"


#include "open.h"
}

EncodeProcessOnly::EncodeProcessOnly(std::string genMatrixName, std::string pchMatrixName)
{

	read_pchk((char *)pchMatrixName.c_str());
	read_gen((char *)genMatrixName.c_str(), 0, 0);

	bitsAfter = N;
	bitsBefore = N - M;
	std::cout << "Bits before - " << bitsBefore << " - Bits After=" << bitsAfter << std::endl;

	u = NULL;
	v = NULL;
	if (type=='d')
	{
		u = mod2dense_allocate(N-M,1);
		v = mod2dense_allocate(M,1);
	}
	else if (type=='m')
	{
		u = mod2dense_allocate(M,1);
		v = mod2dense_allocate(M,1);
	}
	else if(type=='s')
	{
		std::cerr << "Fazendo o sparse encode..." << std::endl;
	}
	else
	{
		std::cerr << "Erro Type='d' ou type='n'  --->" << (int)type << " - " << type  << std::endl;
		exit(0);
	}

	std::cerr << "Iniciando processamento ()N=" << N << " M=" << M << " N-M=" << N-M << std::endl;

	sblk = (char *)chk_alloc (N-M, sizeof *sblk);
	cblk = (char *)chk_alloc (N, sizeof *cblk);
	chks = (char *)chk_alloc (M, sizeof *chks);

}

EncodeProcessOnly::~EncodeProcessOnly()
{
	// TODO Auto-generated destructor stub
}

int EncodeProcessOnly::getSizeInput()
{
	return bitsBefore;
}

int EncodeProcessOnly::getSizeOutput()
{
	return bitsAfter;
}

int EncodeProcessOnly::doEncode(std::vector<bool>& input,
		std::vector<bool>& output)
{
	//int ret = data->readData(N-M, sblk);
	if(input.size() != bitsBefore)
	{
		std::cerr << "Erro tamanho errado de dados de entrada" << std::endl;
		return -1;
	}
	for(int ii = 0; ii<bitsBefore;++ii)
	{
		sblk[ii] = input[ii]?1:0;
	}


	switch (type)
	{
		case 's':
		{
			sparse_encode(sblk, cblk);
			break;
		}
		case 'd':
		{
			dense_encode(sblk, cblk, u, v);
			break;
		}
		case 'm':
		{
			mixed_encode(sblk, cblk, u, v);
			break;
		}
	}
	mod2sparse_mulvec(H, cblk, chks);

	output.resize(N);
	for(int i=0; i<N;++i)
	{
		output[i] = cblk[i]!=0;
	}

	for (int i = 0; i < M; i++)
	{
		if (chks[i] == 1)
		{
			fprintf(stderr,	"Output is not a code word!  (Fails check %d)\n",
							 i);
			abort();
		}
	}
	return 0;
}
