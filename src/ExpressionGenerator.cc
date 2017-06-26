// File         : ExpressionGenerator.cc
// Description  : Implements the expression generator for CodeGenerator.cc.

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "util.h"
#include "CodeGenerator.h"

using namespace std;

// EXPRESSION: new.
void CodeGenerator::generate_new(string type) {
  writer << "new " << type;
}

// EXPRESSION: Bool constant.
// Notes: Generates true/false randomly and with equal probability.
void CodeGenerator::generate_bool() {
  if (rand() % 2 == 0) {
    writer << "true";
  } else {
    writer << "false";
  }
}

// EXPRESSION: String constant.
// Notes: Generates string of 0-10 characters randomly.
void CodeGenerator::generate_string() {
  int length = rand() % 11;
  writer << '\"' << generate_random_string(length) << '\"';
}

// EXPRESSION: Int constant.
// Notes: Generates number between 0 and INT_MAX.
void CodeGenerator::generate_int() {
  writer << rand();
}

// FUNCTION: Checks if there exists an identifier with provided type.
bool CodeGenerator::identifiers_contains(string type) {

  // If we are expanding to SELF_TYPE, we need an identifier with type SELF_TYPE.
  if (type == "SELF_TYPE") {
    for (int i = 0; i < identifiers.size(); i++) {
      if (identifiers[i].second == "SELF_TYPE") {
        return true;
      }
    }
    return false;
  }

  // Otherwise, every time we see SELF_TYPE, we use the current class.
  for(int i = 0; i < identifiers.size(); i++) {
    if (identifiers[i].second == "SELF_TYPE") {
      if (tree.is_child_of(current_class, type)) {
        return true;
      }
    } else {
      if (tree.is_child_of(identifiers[i].second, type)) {
        return true;
      }
    }
  }
  return false;
}

// EXPRESSION: Identifier.
void CodeGenerator::generate_identifier(string type) {

  // Find possible identifiers.
  vector<pair<string, string> > possible_identifiers = vector<pair<string, string> >();
  if (type == "SELF_TYPE") {
    for (int i = 0; i < identifiers.size(); i++) {
      if (identifiers[i].second == "SELF_TYPE") {
        possible_identifiers.push_back(identifiers[i]);
      }
    }
  } else {
    for (int i = 0; i < identifiers.size(); i++) {
      if (identifiers[i].second == "SELF_TYPE") {
        if (tree.is_child_of(current_class, type)) {
          possible_identifiers.push_back(identifiers[i]);
        }
      } else {
        if (tree.is_child_of(identifiers[i].second, type)) {
          possible_identifiers.push_back(identifiers[i]);
        }
      }
    }
  }

  if (identifiers.size() == 0) {
    throw "Internal Error: no identifiers match expression but generate_identifier was called.";
  }

  // Choose identifier at random and print out.
  string identifier = possible_identifiers[rand() % possible_identifiers.size()].first;
  writer << identifier;
}
