
#CC :=$(CC) -O2  -I./src_orig/lib -I./   # Command to compile a module from .c to .o
CC :=$(CC)   -g -I./src_orig/lib -I./   # Command to compile a module from .c to .o

#CPP := g++ -std=c++1y -O2  -Wall -c -I./src_orig/lib -I./ -fmessage-length=0
CPP := g++ -g -std=c++1y -O2   -Wall -c -I./src_orig/lib -I./ -fmessage-length=0

LINK = cc          # Command to link a program

OBJS1 = ldpcmpamenctrans.o  genfuncs.o

OBJS2 =  genInputFile.o  genfuncs.o

OBJSDEC=./decode/LdpcDecodeProcessBase.o ./decode/ldpcmpamdec.o \
	./decode/ReaderLdpcImp.o ./decode/ReaderInputWNoise.o \
	genfuncs.o ./decode/CalcSimbToBitsMsg.o ./decode/ReaderInputBits.o 
	
OBJSENCDEC=./decode/LdpcDecodeProcessBase.o ./decode/encode_decode_parse_opt.o \
     ./decode/ReaderInputWNoise.o ./decode/encode_decode_enc_lib.o genfuncs.o \
     ./decode/encode_decode.o  ./decode/ReaderLdpcImp.o ./decode/CalcSimbToBitsMsg.o

OBJSDECTEST = ./decode/LdpcDecodeProcessBase.o ./decode/encode_decode_parse_opt.o \
     ./decode/ReaderInputWNoise.o ./decode/encode_decode_enc_lib.o genfuncs.o \
     ./decode/decode_teste.o  ./decode/ReaderLdpcImp.o ./decode/CalcSimbToBitsMsg.o

OBJSCOUNTONES = ./decode/EncodeProcessOnly.o ./decode/encode_bit_count.o

OBJ_makeldpc = ./src_orig/make-ldpc.o

OBJ_makegen = ./src_orig/make-gen.o

OBK_analise_matriz_h = ./analise_matriz_h.o ./decode/ReaderLdpcImp.o

OBJTESTE_READ_MATRIX = ./teste_read_matrix.o


.PHONY : ALL

ALL : geninputfile ldpcmpamenctrans ldpcmpamdec make-ldpc make-gen analise_matriz_h encode_decode

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CFLAGS)

LIBS=-L./src_orig/lib/ -lLdpc1 -lm -Wl,-rpath,./src_orig/lib/

LIBSExit = -lgsl -lgslcblas -lm

ldpcmpamenctrans : $(OBJS1)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

testeexit : $(OBJtesteexit1)
	g++ -o $@ $^ $(CFLAGS) $(LIBSExit)
	
geninputfile : $(OBJS2)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

ldpcmpamdec : $(OBJSDEC)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

analise_matriz_h : $(OBK_analise_matriz_h)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)
	
make-ldpc : $(OBJ_makeldpc)
	$(LINK) -o $@ $^ $(CFLAGS) $(LIBS)
	
make-gen : $(OBJ_makegen)
	$(LINK) -o $@ $^ $(CFLAGS) $(LIBS)
	
encode_decode : $(OBJSENCDEC) 
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


teste_read_matrix : $(OBJTESTE_READ_MATRIX)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

decode_teste : $(OBJSDECTEST) 
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

encode_bit_count : $(OBJSCOUNTONES)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)
	
.PHONY: lib clean libclean

lib : 
	 cd ./src_orig/lib/; make

libclean : 
	cd ./src_orig/lib/; make clean

clean :
	rm -f *.o ldpcmpamenctrans *.d
	rm -f ./decode/*.o ./decode/*.d 



