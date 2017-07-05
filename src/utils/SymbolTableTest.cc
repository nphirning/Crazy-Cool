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
	
	vector<pair<string, string> > state = vector<pair<string, string> >();
	state.push_back(pair<string, string>("a", "A"));
	state.push_back(pair<string, string>("b", "B"));
	state.push_back(pair<string, string>("c", "C"));
	assert(state == table.current_ids());

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

	state = vector<pair<string, string> >();
	state.push_back(pair<string, string>("a", "A"));
	state.push_back(pair<string, string>("b", "B"));
	state.push_back(pair<string, string>("c", "C"));
	state.push_back(pair<string, string>("d", "D"));
	assert(state == table.current_ids());

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

	state = vector<pair<string, string> >();
	state.push_back(pair<string, string>("a", "A1"));
	state.push_back(pair<string, string>("b", "B"));
	state.push_back(pair<string, string>("c", "C"));
	state.push_back(pair<string, string>("d", "D1"));
	state.push_back(pair<string, string>("e", "E1"));
	assert(state == table.current_ids());

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

	state = vector<pair<string, string> >();
	state.push_back(pair<string, string>("a", "A"));
	state.push_back(pair<string, string>("b", "B"));
	state.push_back(pair<string, string>("c", "C"));
	state.push_back(pair<string, string>("d", "D"));
	state.push_back(pair<string, string>("e", "E"));
	assert(state == table.current_ids());

	// Final removing scope test.
	table.exit_scope();

	assert(table.lookup("d") == "");
	assert(table.lookup("e") == "");
	assert(table.lookup("a") == "A");

	// Current state:
	//	a -> [(A, 0)]
	//	b -> [(B, 0)]
	// 	c -> [(C, 0)]

	state = vector<pair<string, string> >();
	state.push_back(pair<string, string>("a", "A"));
	state.push_back(pair<string, string>("b", "B"));
	state.push_back(pair<string, string>("c", "C"));
	assert(state == table.current_ids());

	cout << "Tests passed!" << endl;

	return 0;
}