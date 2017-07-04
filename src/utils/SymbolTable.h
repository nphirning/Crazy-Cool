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
	// base scope to which you can add IDs. You should not exit this scope.
	SymbolTable();

	// Enters a new scope.
	void enter_scope();

	// Exits the most recent scope. Calling exit scope when 
	// in the ground scope results in a string exception thrown and
	// the behavior is undefined.
	void exit_scope();

	// Adds @id of type @type to the table. If an identifier
	// with the same name exists in the current scope, then it
	// is overwritten. Note that no type is allowed to be the
	// empty string (string exception will be thrown).
	void addid(std::string id, std::string type);

	// Looks up @id in the table and returns the type of the most 
	// closely nested identifier with that name. Returns the empty
	// string if @id is not found.
	std::string lookup(std::string id);

	// Prints out the contents of the table. Used for debugging.
	void print_debug();

private:
	int current_scope;

	// Internal data structure: id -> [(type, scope)]
	std::map<std::string, std::vector<std::pair<std::string, int> > > table;
};


#endif