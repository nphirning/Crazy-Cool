CC=g++
INC=../utils
SRC=ClassTree.cc
CFLAGS=-std=c++11 -c -I$(INC)

all: dependencies

dependencies: $(SRC)
	$(CC) $(CFLAGS) $^

clean:
	rm -f *.o

