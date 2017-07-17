CC=g++
INC=-I../class_structure -I../utils
OBJ=CodeGenerator.o ExpressionGenerator.o
CFLAGS=-std=c++11 -c $(INC)
DEPS=CodeGenerator.h

all: dependencies

dependencies: $(OBJ)

%.o: %.cc $(DEPS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o
