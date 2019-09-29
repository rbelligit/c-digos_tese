/*
 * teste_read_matrix.cpp
 *
 *  Created on: 10 de jan de 2019
 *      Author: belli
 */

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
#include <vector>
#include <string>

static void printUsage()
{
	std::cerr << "teste_read_matrix 'matr_pchk' 'matr_gen' 'n realizacoes de codif'" << std::endl;
	exit(0);
}

void generate_bits(std::vector<char> *data)
{
	int ll = data->size();
	std::vector<char> temp;
	temp.resize((ll+7)/8);
	FILE *fid = fopen("/dev/urandom", "rb");
	fread(temp.data(), 1, temp.size(), fid);
	fclose(fid);

	for(int i=0;i<ll;++i)
	{
		int pos = i/8;
		int spos = i%8;
		(*data)[i] = (temp[pos] & (1<<spos))?1:0;
	}
}

int main(int argc, const char *argv[])
{
	if(argc != 4)
	{
		printUsage();
	}

	const char *file_pchk = argv[1];
	const char *file_gen = argv[2];
	read_pchk((char *)file_pchk);
	read_gen((char *)file_gen, 0, 0);

	int n_ents = strtol(argv[3], nullptr, 10);
	if(n_ents <= 0)
	{
		printUsage();
	}

	std::cout << M << "," << N << "," << n_ents << std::endl;
	// Gera uma sequencia de n realizacoes de codificacao
	int quant_entrada = N-M;
	int quant_saida = N;
	//std::cout << "quantidade entrada=" << quant_entrada << " - quant saida=" << quant_saida << std::endl;

	std::vector<char> entrada, saida;
	entrada.resize(quant_entrada);
	saida.resize(quant_saida);
	for(int kk=0;kk<n_ents;++kk)
	{
		generate_bits(&entrada);

		switch (type)
		{
			case 's':
			{
				sparse_encode (entrada.data(), saida.data());
				break;
			}
			case 'd':
			{
				mod2dense *u, *v;
				u = mod2dense_allocate(N-M,1);
				v = mod2dense_allocate(M,1);
				dense_encode (entrada.data(), saida.data(), u, v);
				break;
			}
			case 'm':
			{
				mod2dense *u, *v;
				u = mod2dense_allocate(M,1);
				v = mod2dense_allocate(M,1);
				mixed_encode (entrada.data(), saida.data(), u, v);
				break;
			}
		}

		for(char ss : saida)
		{
			if(ss)
			{
				std::cout << "1";
			}
			else
			{
				std::cout << "0";
			}
		}
		std::cout << std::endl;

	}

	return 0;

}


