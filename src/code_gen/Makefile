CC=g++
INC=-I../class_structure -I../utils
OBJ=CodeGenerator.o ExpressionGenerator.o
CFLAGS=-std=c++11 -c $(INC)

all: dependencies

dependencies: $(OBJ)

CodeGenerator.o: CodeGenerator.cc
	$(CC) $(CFLAGS) $^ -o $@

ExpressionGenerator.o: ExpressionGenerator.cc
	$(CC) $(CFLAGS) $^ -o $@

clean: 
	rm -f *.o