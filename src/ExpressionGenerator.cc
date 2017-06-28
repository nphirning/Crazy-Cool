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
  current_line_length += 4 + type.length();
}

// EXPRESSION: Bool constant.
// Notes: Generates true/false randomly and with equal probability.
void CodeGenerator::generate_bool() {
  if (rand() % 2 == 0) {
    writer << "true";
    current_line_length += 4;
  } else {
    writer << "false";
    current_line_length += 5;
  }
}

// EXPRESSION: String constant.
// Notes: Generates string of 0-10 characters randomly.
void CodeGenerator::generate_string() {
  int length = rand() % 11;
  writer << '\"' << generate_random_string(length) << '\"';
  current_line_length += length + 2;
}

// EXPRESSION: Int constant.
// Notes: Generates number between 0 and INT_MAX.
void CodeGenerator::generate_int() {
  int a = rand();
  writer << a;
  current_line_length += to_string(a).length();
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
  current_line_length += identifier.length();

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
  current_line_length += assign.first.first.length() + 5;

  if (current_line_length >= max_line_length) {
    writer << endl;
    indentation_tabs++;
    print_tabs();
    generate_expression(assign.second);
    indentation_tabs--;
    writer << endl;
    print_tabs();
    writer << ")";
  } else {
    generate_expression(assign.second);
    writer << ")";
  }

  // Default return.
  return true;
}

// EXPRESSION: Dispatch.
// NOTES: See identifier/assignment for definition of @abort_early.
bool CodeGenerator::generate_dispatch(string type, bool abort_early) {

  // Iterate through identifiers and compute dispatches.
  // Stored in a vector with entries ((identifier name, identifier type), method_name).
  vector<pair<pair<string, string>, string> > possible_dispatches = vector<pair<pair<string, string>, string> >();

  for (int i = 0; i < identifiers.size(); i++) {
    string identifier_name = identifiers[i].first;
    string identifier_type = identifiers[i].second;
    if (identifier_type == "SELF_TYPE") identifier_type = current_class;

    // STEP 1: Extract all possible methods.
    // NOTES: - We keep a set of the method signatures (name, return, [arg types])
    //          for keeping track of inherited methods.
    //        - In the end, we want a set of (class_name, method_name) pairs
    //          representing all the call-able methods on a given identifier.
    set<pair<pair<string, string>, vector<string> > > method_signatures =
                            set<pair<pair<string, string>, vector<string> > >();
    set<pair<string, string> > method_names = set<pair<string, string> >();
    vector<string> available_method_classes = tree.class_ancestors[identifier_type];
    available_method_classes.insert(available_method_classes.begin(), identifier_type);

    for (int j = 0; j < available_method_classes.size(); j++) {
      string class_to_check = available_method_classes[j];
      vector<string> class_methods = tree.class_method_names[class_to_check];
      for (int k = 0; k < class_methods.size(); k++) {
        string method_name = class_methods[k];

        // Compute method signature.
        string method_return = tree.class_method_types[class_to_check][method_name];
        pair<string, string> name_return_pair = pair<string, string>(method_name, method_return);
        vector<string> argument_types = vector<string>();

        vector<pair<string, string> > method_args = tree.class_method_args[class_to_check][method_name];
        for (int l = 0; l < method_args.size(); l++) {
          argument_types.push_back(method_args[l].second);
        }

        pair<pair<string, string>, vector<string> > method_signature =
              pair<pair<string, string>, vector<string> >(name_return_pair, argument_types);

        // Only add if this is not inherited and hidden by another method.
        if (method_signatures.find(method_signature) == method_signatures.end()) {
          method_signatures.insert(method_signature);
          method_names.insert(pair<string, string>(class_to_check, method_name));
        }
      }
    }

    // STEP 2: For each method, check if the return type will pass type checking.

    for (set<pair<string, string> >::iterator it = method_names.begin();
            it != method_names.end(); ++it) {

      string class_name = it->first;
      string method_name = it->second;
      string return_type = tree.class_method_types[class_name][method_name];

      // If a method returns SELF_TYPE and is called on an identifier of type
      // SELF_TYPE, then the return type is still SELF_TYPE. Otherwise, it
      // should be the actual type of the identifier.
      if (return_type == "SELF_TYPE") {
        return_type = identifiers[i].second;
      }

      // Lots of little SELF_TYPE edge cases.
      if (type == "SELF_TYPE") {
        if (return_type == "SELF_TYPE") {
          if (abort_early) return true;
          possible_dispatches.push_back(pair<pair<string, string>, string>(identifiers[i], method_name));
        }
      } else {
        if (return_type == "SELF_TYPE") return_type = current_class;
        if (tree.is_child_of(return_type, type)) {
          if (abort_early) return true;
          possible_dispatches.push_back(pair<pair<string, string>, string>(identifiers[i], method_name));
        }
      }
    }
  }

  // Return if none found.
  if (abort_early) return false;

  if (possible_dispatches.size() == 0) {
    throw "Internal Error: generate_dispatches called but no dispatches possible.";
  }

  // Choose dispatch and extract information.
  pair<pair<string, string>, string> dispatch = possible_dispatches[rand() % possible_dispatches.size()];
  string identifier_name = dispatch.first.first;
  string identifier_type = dispatch.first.second;
  string method_name = dispatch.second;
  vector<pair<string, string> > arguments = tree.class_method_args[identifier_type][method_name];

  // Write dispatch.
  if (identifier_name != "self") {
    writer << identifier_name << ".";
    current_line_length += identifier_name.length() + 1;
  }
  writer << method_name << '(';
  current_line_length += method_name.length() + 1;

  // Print on other lines if enough arguments / line too long.
  if (arguments.size() >= 3 || current_line_length >= max_line_length) {
    writer << endl;
    indentation_tabs++;
    for (int i = 0; i < arguments.size(); i++) {
      print_tabs();
      generate_expression(arguments[i].second);
      if (i != arguments.size() - 1) writer << ',';
      writer << endl;
    }
    indentation_tabs--;
    print_tabs();
    writer << ')';
  } else {
    for (int i = 0; i < arguments.size(); i++) {
      generate_expression(arguments[i].second);
      if (i != arguments.size() - 1) {
        writer << ", ";
        current_line_length += 2;
      }
    }
    writer << ')';
    current_line_length++;
  }

  // Defaut return.
  return true;
}
