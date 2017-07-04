CC=g++
SRC_TEST=SymbolTable.o SymbolTableTest.cc
SRC=SymbolTable.o util.o
CFLAGS=-std=c++11 
CFLAGS_COMPILE=-std=c++11 -c

all: symboltest dependencies

symboltest: $(SRC_TEST)
	$(CC) $(CFLAGS) $^ -o $@

dependencies: $(SRC)

SymbolTable.o: SymbolTable.cc
	$(CC) $(CFLAGS_COMPILE) $^

util.o: util.cc
	$(CC) $(CFLAGS_COMPILE) $^

clean: 
	rm -f *.o symboltest