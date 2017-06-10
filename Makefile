CC=g++
SRC=main.cc ClassTree.cc util.cc

all: crazycool

crazycool: $(SRC)
	$(CC) $^ -o $@

