CC=g++
INC=../utils
SRC=ClassTree.cc
OBJ=ClassTree.o
CFLAGS=-std=c++11 -c -I$(INC)

all: dependencies

dependencies: $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $^

clean:
	rm -f *.o

