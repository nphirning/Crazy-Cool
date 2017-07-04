// File: SymbolTableTest.cc
// Description: Basic tests for the SymbolTable class.

#include <cassert>
#include <iostream>
#include "SymbolTable.h"

using namespace std;

int main() {
	SymbolTable table = SymbolTable();
	
	// Test base scope functionality.
	table.addid("a", "A");
	table.addid("b", "B");
	table.addid("c", "C");
	assert(table.lookup("b") == "B");
	assert(table.lookup("d") == "");

	// Current state:
	//	a -> [(A, 0)]
	//	b -> [(B, 0)]
	// 	c -> [(C, 0)]

	// Test adding to second scope.
	table.enter_scope();
	table.addid("d", "D");
	assert(table.lookup("a") == "A");
	assert(table.lookup("d") == "D");
	assert(table.lookup("b") == "B");

	// Current state:
	//	a -> [(A, 0)]
	//	b -> [(B, 0)]
	// 	c -> [(C, 0)]
	//	d -> [(D, 1)]

	// Test overwriting from scope.
	table.enter_scope();
	table.addid("a", "A1");
	table.addid("e", "E");
	table.addid("d", "D1");
	assert(table.lookup("a") == "A1");
	assert(table.lookup("e") == "E");
	assert(table.lookup("b") == "B");
	table.addid("e", "E1");
	assert(table.lookup("e") == "E1");

	// Current state:
	//	a -> [(A, 0), (A1, 2)]
	//	b -> [(B, 0)]
	// 	c -> [(C, 0)]
	//	d -> [(D, 1), (D1, 2)]
	// 	e -> [(E1, 2)]

	// Test for removing scope.
	table.exit_scope();
	assert(table.lookup("a") == "A");
	assert(table.lookup("b") == "B");
	assert(table.lookup("d") == "D");
	assert(table.lookup("e") == "");
	table.addid("e", "E");

	// Current state:
	//	a -> [(A, 0)]
	//	b -> [(B, 0)]
	// 	c -> [(C, 0)]
	//	d -> [(D, 1)]
	// 	e -> [(E, 1)]

	// Final removing scope test.
	table.exit_scope();

	assert(table.lookup("d") == "");
	assert(table.lookup("e") == "");
	assert(table.lookup("a") == "A");

	// Current state:
	//	a -> [(A, 0)]
	//	b -> [(B, 0)]
	// 	c -> [(C, 0)]

	cout << "Tests passed!" << endl;

	return 0;
}