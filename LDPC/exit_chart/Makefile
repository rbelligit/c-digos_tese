CC :=$(CC)   -g -I./src_orig/lib -I./   # Command to compile a module from .c to .o

#CPP := g++ -std=c++1y -O2  -Wall -c -I./src_orig/lib -I./ -fmessage-length=0
CPP := g++ -g -std=c++1y -O2   -Wall -c -I./src_orig/lib -I./ -fmessage-length=0

LINK = cc          # Command to link a program

LIBS = -lgsl -lgslcblas -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CFLAGS)

OBJ_teste1 = calcexit.o genFuncs.o teste1.o

OBJ_teste2 = calcexit.o genFuncs.o teste2.o

OBJ_teste3 = calcexit.o genFuncs.o calc_exit_bits_checks.o teste3.o

teste1 : $(OBJ_teste1)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)
	
teste2 : $(OBJ_teste2)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

teste3 : $(OBJ_teste3)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

clean : 
	rm -f *.o teste1 teste2
