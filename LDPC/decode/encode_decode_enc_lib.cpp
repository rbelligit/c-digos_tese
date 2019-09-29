/*
 * encode_decode_enc_lib.cpp
 *
 *  Created on: 4 de fev de 2018
 *      Author: belli
 */




#include "encode_decode_enc_lib.h"
#include <cstdio>
#include <cstdlib>
extern "C"
{
#include <mod2dense.h>
#include <alloc.h>
#include <enc.h>
#include <mod2sparse.h>
}

DoMapClass::DoMapClass(Options_st *opts) : opts(opts)
{

}

DoMapClass::~DoMapClass()
{

}

int DoMapClass::processData(char *data, int size, std::vector<uint8_t> &levels)
{
	//std::cout << "Processing data size=" << size << std::endl;
	int nBits = opts->mapQAM.nBits;
	int nMaps = size / nBits;
	//std::cout << "nBits=" << nBits << std::endl;
	// um pacote de dados.... um frame do LDPC
	//std::cout << "nMaps=" << nMaps << std::endl;
	auto map1 = opts->mapQAM.map;
	for(unsigned int i=0;i<map1.size();++i)
	{
		for(unsigned int k=0;k<map1[i].size();++k)
		{
			mapent.insert(std::pair<MapEntry_t, int>(map1[i][k], i));
		}
	}

	std::vector<bool> sub;
	sub.resize(nBits);

	levels.resize(nMaps);
	for(int i=0;i<nMaps;++i)
	{
		//std::cout << "MAP :";
		for(int k=0;k<nBits;++k)
		{
			sub[k]=(data[i*nBits+nBits-k-1] != 0);
			//std::cout << (char *)(sub[k]?"1":"0");
		}
		auto ptr = mapent.find(sub);
		if(ptr == mapent.end())
		{
			std::cerr << "Mapeamento não encontrado.... Erro " << sub << std::endl;
			exit(1);
		}
		levels[i]=ptr->second;
		//std::cout << " -> " << (int)levels[i] << std::endl;
	}
	return 0;
}

extern char type;
extern int N;
extern int M;
extern mod2sparse *H;

Do_encode_class::Do_encode_class(DoMapClass* map, Options_st* opts, ReadDataInput *data) :
	map(map), opts(opts), data(data)
{
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

int Do_encode_class::do_encode(std::vector<uint8_t>& encoded, std::vector<bool> &bits)
{
	int ret = data->readData(N-M, sblk);
	if(ret != N-M)
	{
		std::cerr << "Finalizado... " << std::endl;
		return -1;
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

	bits.resize(N);
	for(int i=0; i<N;++i)
	{
		bits[i] = cblk[i]!=0;
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
	map->processData(cblk, N, encoded);

	return 0;

}
