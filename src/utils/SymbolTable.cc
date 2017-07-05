// File: SymbolTable.cc
// Description: Implementation of the symbol table. 

#include "SymbolTable.h"
#include <map>
#include <string>
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

SymbolTable::SymbolTable() {
	this->current_scope = 0;
	this->table = map<string, vector<pair<string, int> > >();
}

void SymbolTable::enter_scope() {
	current_scope++;
}

void SymbolTable::exit_scope() {
	map<string, vector<pair<string, int> > >::iterator it = table.begin();
	while (it != table.end()) {
		vector<pair<string, int> >& entry = it->second;

		// Remove back if it is in this scope.
		if (entry.size() != 0 && entry.back().second == current_scope) {
			entry.pop_back();
		}

		// Remove the table entry if it's empty.
		if (entry.size() == 0) {
			it = table.erase(it);
		} else {
			++it;
		}
	}

	if (current_scope != 0) {
		current_scope--;
	}
}

void SymbolTable::add_id(string id, string type) {

	if (type == "") {
		throw "Type cannot be the empty string";
	}

	if (table.find(id) == table.end()) {
		table[id] = vector<pair<string, int> >();
	} else {
		vector<pair<string, int> >& entry = table[id];
		
		// Remove existing scope definition.
		if (entry.size() != 0 && entry.back().second == current_scope) {
			entry.pop_back();
		}
	}
	table[id].push_back(pair<string, int>(type, current_scope));
}

string SymbolTable::lookup(string id) {
	if (table.find(id) == table.end()) {
		return "";
	} else {
		return table[id].back().first;
	}
}

vector<pair<string, string> > SymbolTable::current_ids() {
	vector<pair<string, string> > ids = vector<pair<string, string> >();
	for (map<string, vector<pair<string, int> > >::iterator it = table.begin(); it != table.end(); ++it) {
		vector<pair<string, int> > entry = it->second;
		ids.push_back(pair<string, string>(it->first, entry.back().first));
	}
	return ids;
}

void SymbolTable::print_debug() {
	cout << "PRINT ------" << endl;
	for (map<string, vector<pair<string, int> > >::iterator it = table.begin(); it != table.end(); ++it) {
		vector<pair<string, int> >entry = it->second;
		cout << it->first << " -> [ ";
		for (int i = 0; i < entry.size(); i++) {
			cout << '(' << entry[i].first << ',' << entry[i].second << ") ";
		}
		cout << "]" << endl;
	}
	cout << "------------" << endl;
}






