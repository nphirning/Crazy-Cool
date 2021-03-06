// File: SymbolTable.h
// Description: Header file for a basic symbol table.


#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <utility>
#include <string>
#include <map>
#include <vector>

class SymbolTable {
public:

	// Constructor. Note that the symbol table starts out in a
	// base scope to which you can add IDs.
	SymbolTable();

	// Enters a new scope.
	void enter_scope();

	// Exits the most recent scope. If we are in the ground scope,
	// then this just deletes all the entries.
	void exit_scope();

	// Adds @id of type @type to the table. If an identifier
	// with the same name exists in the current scope, then it
	// is overwritten. Note that no type is allowed to be the
	// empty string (string exception will be thrown).
	void add_id(std::string id, std::string type);

	// Looks up @id in the table and returns the type of the most 
	// closely nested identifier with that name. Returns the empty
	// string if @id is not found.
	std::string lookup(std::string id);

	// Returns a vector that contains entries (id, type) with the
	// closest scoped definitions of all keys in the symbol table.
	std::vector<std::pair<std::string, std::string> > current_ids();

	// Prints out the contents of the table. Used for debugging.
	void print_debug();

private:
	int current_scope;

	// Internal data structure: id -> [(type, scope)]
	std::map<std::string, std::vector<std::pair<std::string, int> > > table;
};


#endif