CC=g++
SRC=main.cc ClassTree.cc util.cc CodeGenerator.cc ExpressionGenerator.cc
CFLAGS=-std=c++11

all: crazycool

crazycool: $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f crazycool
