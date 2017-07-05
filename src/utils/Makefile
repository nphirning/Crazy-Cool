CC=g++
TEST=SymbolTable.o SymbolTableTest.cc
OBJ=SymbolTable.o util.o
CFLAGS=-std=c++11 
CFLAGS_COMPILE=-std=c++11 -c

all: symboltest dependencies

symboltest: $(TEST)
	$(CC) $(CFLAGS) $^ -o $@

dependencies: $(OBJ)

SymbolTable.o: SymbolTable.cc
	$(CC) $(CFLAGS_COMPILE) $^

util.o: util.cc
	$(CC) $(CFLAGS_COMPILE) $^

clean: 
	rm -f *.o symboltest