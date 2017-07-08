CC=g++
SYMBOLTEST_SRC=SymbolTable.o SymbolTableTest.cc
OBJ=SymbolTable.o NameGenerator.o util.o
CFLAGS=-std=c++11 
CFLAGS_COMPILE=-std=c++11 -c
DEPS=SymbolTable.h util.h NameGenerator.h


all: symboltest dependencies

symboltest: $(SYMBOLTEST_SRC)
	$(CC) $(CFLAGS) $^ -o $@

dependencies: $(OBJ)

%.o: %.cc $(DEPS)
	$(CC) $(CFLAGS_COMPILE) $< -o $@

clean: 
	rm -f *.o symboltest