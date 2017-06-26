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

  // Case 1: Expanding to SELF_TYPE.
  // NOTES: - We need an identifier of type SELF_TYPE.
  if (type == "SELF_TYPE") {
    for (int i = 0; i < identifiers.size(); i++) {
      if (identifiers[i].second == "SELF_TYPE") {
        return true;
      }
    }
    return false;
  }

  // Case 2: Expanding to anything but SELF_TYPE.
  // NOTES: - SELF_TYPE is replaced by the current class.
  for(int i = 0; i < identifiers.size(); i++) {
    string identifier_type = identifiers[i].second;
    if (identifier_type == "SELF_TYPE") {
      identifier_type = current_class;
    }

    if (tree.is_child_of(identifier_type, type)) {
        return true;
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
      string identifier_type = identifiers[i].second;
      if (identifier_type == "SELF_TYPE") {
        identifier_type = current_class;
      }

      if (tree.is_child_of(identifier_type, type)) {
        possible_identifiers.push_back(identifiers[i]);
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

// FUNCTION: Checks if an assignment can return provided type.
bool CodeGenerator::assignment_possible(string type) {

  // Case 1: Expanding to SELF_TYPE.
  // NOTES: - We need an identifier of type SELF_TYPE.
  if (type == "SELF_TYPE") {
    for(int i = 0; i < identifiers.size(); i++) {
      string identifier_type = identifiers[i].second;
      if (identifier_type == "SELF_TYPE") {
        return true;
      }
    }
    return false;
  }

  // Case 2: Not expanding to SELF_TYPE.
  // NOTES: - For each possible child of @type, we check
  //          if there exists an identifier with type <= child.
  //        - Identifiers w/type SELF_TYPE are consider instances of current class.
  set<string> possible_assign_types = tree.class_descendants[type];
  possible_assign_types.insert(type);
  for(set<string>::iterator it = possible_assign_types.begin();
                            it != possible_assign_types.end(); ++it) {
    string possible_type = *it;
    for (int i = 0; i < identifiers.size(); i++) {
      string identifier_type = identifiers[i].second;
      if (identifier_type == "SELF_TYPE") {
        identifier_type = current_class;
      } else {
        if (tree.is_child_of(possible_type, identifier_type)) {
          return true;
        }
      }
    }
  }

  return false;
}

// EXPRESSION: Assignment.
void CodeGenerator::generate_assignment(string type) {

  // Choose possible assigns.
  // NOTES: - The possible assigns are stored in a vector where elements are of the form
  //          ((identifier name, identifier type), assign expression type)
  vector<pair<pair<string, string>, string> > possible_assigns = vector<pair<pair<string, string>, string> >();

  if (type == "SELF_TYPE") {
    for (int i = 0; i < identifiers.size(); i++) {
      if (identifiers[i].second == "SELF_TYPE") {
        possible_assigns.push_back(pair<pair<string, string>, string>(identifiers[i], "SELF_TYPE"));
      }
    }
  } else {
    set<string> possible_assign_types = tree.class_descendants[type];
    possible_assign_types.insert(type);
    for (set<string>::iterator it = possible_assign_types.begin();
                               it != possible_assign_types.end(); ++it) {
      string possible_assign_type = *it;
      for (int i = 0; i < identifiers.size(); i++) {
        string identifier_type = identifiers[i].second;
        if (identifier_type == "SELF_TYPE") {
          identifier_type = current_class;
        }
        if (tree.is_child_of(possible_assign_type, identifier_type)) {
          possible_assigns.push_back(pair<pair<string, string>, string>(identifiers[i], possible_assign_type));
        }
      }
    }
  }

  if (possible_assigns.size() == 0) {
    throw "Internal Error: No possible assignments though generate_assignment was called.";
  }

  // Choose assignment randomly and output result.
  pair<pair<string, string>, string> assign = possible_assigns[rand() % possible_assigns.size()];
  writer << assign.first.first << " <- (";
  generate_expression(assign.second);
  writer << ")";
}
