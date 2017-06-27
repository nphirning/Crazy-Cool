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

// EXPRESSION: Identifier.
// NOTES: - If @abort_early is true, then this will return whether
//          an identifier exists that can be used for @type.
//        - If @abort_early is false, the return will be true on successful
//          output (an exception will be thrown if no possible identifiers exist).
bool CodeGenerator::generate_identifier(string type, bool abort_early) {

  // Find possible identifiers.
  vector<pair<string, string> > possible_identifiers = vector<pair<string, string> >();
  if (type == "SELF_TYPE") {
    for (int i = 0; i < identifiers.size(); i++) {
      if (identifiers[i].second == "SELF_TYPE") {
        if (abort_early) return true;
        possible_identifiers.push_back(identifiers[i]);
      }
    }
  } else {
    for (int i = 0; i < identifiers.size(); i++) {
      string identifier_type = identifiers[i].second;
      if (identifier_type == "SELF_TYPE") {
        identifier_type = current_class;
      }

      if (tree.is_child_of(identifier_type, type)) {
        if (abort_early) return true;
        possible_identifiers.push_back(identifiers[i]);
      }
    }
  }

  // No possible identifiers found.
  if (abort_early) return false;

  if (identifiers.size() == 0) {
    throw "Internal Error: no identifiers match expression but generate_identifier was called.";
  }

  // Choose identifier at random and print out.
  string identifier = possible_identifiers[rand() % possible_identifiers.size()].first;
  writer << identifier;

  // Default return.
  return true;
}

// EXPRESSION: Assignment.
// NOTES: - If @abort_early is true, then this will return whether
//          an assignment exists that can be used for @type.
//        - If @abort_early is false, the return will be true on successful
//          output (an exception will be thrown if no possible assignments exist).
bool CodeGenerator::generate_assignment(string type, bool abort_early) {

  // Choose possible assigns.
  // NOTES: - The possible assigns are stored in a vector where elements are of the form
  //          ((identifier name, identifier type), assign expression type)
  vector<pair<pair<string, string>, string> > possible_assigns = vector<pair<pair<string, string>, string> >();

  if (type == "SELF_TYPE") {
    for(int i = 0; i < identifiers.size(); i++) {

      string identifier_type = identifiers[i].second;

      // Can't assign to self.
      if (identifiers[i].first == "self") continue;

      if (identifier_type == "SELF_TYPE" || tree.is_child_of(current_class, identifier_type)) {
        if (abort_early) return true;
        possible_assigns.push_back(pair<pair<string, string>, string>(identifiers[i], "SELF_TYPE"));
      }
    }
  } else {
    set<string> possible_assign_types = tree.class_descendants[type];
    possible_assign_types.insert(type);
    if (tree.is_child_of(current_class, type)) {
      possible_assign_types.insert("SELF_TYPE");
    }

    for(set<string>::iterator it = possible_assign_types.begin();
                              it != possible_assign_types.end(); ++it) {
      string possible_type = *it;
      if (possible_type == "SELF_TYPE") {
        possible_type = current_class;
      }

      for (int i = 0; i < identifiers.size(); i++) {

        string identifier_type = identifiers[i].second;

        // Can't assign to self.
        if (identifiers[i].first == "self") continue;

        // If identifier is SELF_TYPE, we need assignment to be SELF_TYPE.
        // Otherwise, if assignment is SELF_TYPE, we treat it as current class.
        if (identifier_type == "SELF_TYPE") {
          if (*it == "SELF_TYPE") {
            if (abort_early) return true;
            possible_assigns.push_back(pair<pair<string, string>, string>(identifiers[i], *it));
          }
        } else {
          if (tree.is_child_of(possible_type, identifier_type)) {
            if (abort_early) return true;
            possible_assigns.push_back(pair<pair<string, string>, string>(identifiers[i], *it));
          }
        }
      }
    }
  }

  // No possible assignments found.
  if (abort_early) return false;

  if (possible_assigns.size() == 0) {
    throw "Internal Error: No possible assignments though generate_assignment was called.";
  }

  // Choose assignment randomly and output result.
  pair<pair<string, string>, string> assign = possible_assigns[rand() % possible_assigns.size()];
  writer << assign.first.first << " <- (";
  generate_expression(assign.second);
  writer << ")";

  // Default return.
  return true;
}
