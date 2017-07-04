CC=g++
INC=-I../class_structure -I../utils
SRC=CodeGenerator.cc ExpressionGenerator.cc
CFLAGS=-std=c++11 -c $(INC)

all: dependencies

dependencies: $(SRC)
	$(CC) $(CFLAGS) $^

clean: 
	rm -f *.o