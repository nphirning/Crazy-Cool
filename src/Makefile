CC=g++
INC=-Iclass_structure -Icode_gen -Iutils
OBJ=utils/SymbolTable.o code_gen/CodeGenerator.o code_gen/ExpressionGenerator.o utils/util.o utils/NameGenerator.o \
		class_structure/ClassTree.o
SRC=main.cc
CFLAGS=-std=c++11 $(INC)

all: crazycool

crazycool: $(SRC) makefiles
	$(CC) $(CFLAGS) $< $(OBJ) -o $@

makefiles:
	$(MAKE) -C utils
	$(MAKE) -C class_structure
	$(MAKE) -C code_gen

clean:
	rm -f crazycool
	find . -type f -name '*.o' -delete

.PHONY: makefiles all clean
