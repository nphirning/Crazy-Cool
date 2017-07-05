CC=g++
INC=../utils
OBJ=ClassTree.o
CFLAGS=-std=c++11 -c -I$(INC)
DEPS=ClassTree.h

all: dependencies

dependencies: $(OBJ)

%.o: %.cc $(DEPS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o

