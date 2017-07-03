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
// NOTES: - This updates the internal class structures storing information
//          about dispatches.
void CodeGenerator::generate_dispatch_structures(string type) {

  // Reset data structures.
  static_dispatches = vector<pair<pair<string, string>, pair<string, string> > >();
  dispatches = vector<pair<string, pair<string, string> > >();
  self_dispatches = vector<pair<string, string> >();

  // Iterate through all methods in all classes.
  for (int i = 0; i < tree.class_names.size(); i++) {
    string class_name = tree.class_names[i];
    vector<string> class_methods = tree.class_method_names[class_name];
    for (int j = 0; j < class_methods.size(); j++) {
      string method_name = class_methods[j];
      pair<string, string> method_signature = pair<string, string>(class_name, method_name);
      string return_type = tree.class_method_types[class_name][method_name];

      // Case 1: We need the dispatch to conform to SELF_TYPE.
      //          - This can only occur if the return type of the method is
      //            SELF_TYPE and the object it is called on is of type SELF_TYPE.
      //          - No such thing as a static dispatch to SELF_TYPE.

      if (type == "SELF_TYPE") {
        if (return_type == "SELF_TYPE") {

          // Self.
          if (tree.is_child_of(current_class, class_name)) {
            self_dispatches.push_back(method_signature);
          }

          // Regular.
          if (tree.is_child_of(current_class, class_name)) {
            dispatches.push_back(pair<string, pair<string, string> >("SELF_TYPE", method_signature));
          }
        }
      }

      // Case 2: The type we need to expand to is not SELF_TYPE.
      //          - (2a) If the method returns SELF_TYPE then we have cases:
      //            * self: we need @current_class <= @type and
      //                            @current_class <= @class_name
      //            * static: consider A@B.m(). Then we need
      //                      B <= @class_name
      //                      A <= B
      //                      B <= @type.
      //            * regular: consider A.m(). Then we need
      //                      A <= @class_name
      //                      A <= @type.
      //          - (2b) If the method returns type C:
      //            * self: we need C <= @type
      //                            @current_class <= @class_name.
      //            * static: consider A@B.m(). Then we need
      //                      C <= @type
      //                      B <= @class_name
      //                      A <= B.
      //            * regular: consider A.m(). Then we need
      //                      C <= @type
      //                      A <= @class_name.

      if (type != "SELF_TYPE") {

        // Case 2a.
        if (return_type == "SELF_TYPE") {

          // Self.
          if (tree.is_child_of(current_class, class_name) && tree.is_child_of(current_class, type)) {
            self_dispatches.push_back(method_signature);
          }

          // Static.
          set<string> type_children = tree.class_descendants[type];
          type_children.insert(type);
          for(set<string>::iterator it = type_children.begin(); it != type_children.end(); ++it) {
            string possible_static_type = *it;
            if (!tree.is_child_of(possible_static_type, class_name)) continue;
            set<string> static_type_children = tree.class_descendants[possible_static_type];
            static_type_children.insert(possible_static_type);
            for(set<string>::iterator it2 = static_type_children.begin(); it2 != static_type_children.end(); ++it2) {
              string expression_type = *it2;
              pair<string, string> static_signature = pair<string, string>(expression_type, possible_static_type);
              static_dispatches.push_back(pair<pair<string, string>, pair<string, string> >(static_signature, method_signature));
            }
          }

          // Regular.
          for(set<string>::iterator it = type_children.begin(); it != type_children.end(); ++it) {
            string possible_type = *it;
            if (!tree.is_child_of(possible_type, class_name)) continue;
            dispatches.push_back(pair<string, pair<string, string> >(possible_type, method_signature));
          }
        }

        // Case 2b.
        if (return_type != "SELF_TYPE") {
          if (tree.is_child_of(return_type, type)) {

            // Self.
            if (tree.is_child_of(current_class, class_name)) {
              self_dispatches.push_back(method_signature);
            }

            // Static.
            set<string> type_children = tree.class_descendants[class_name];
            type_children.insert(class_name);
            for(set<string>::iterator it = type_children.begin(); it != type_children.end(); ++it) {
              string possible_static_type = *it;
              if (!tree.is_child_of(possible_static_type, class_name)) continue;
              set<string> static_type_children = tree.class_descendants[possible_static_type];
              static_type_children.insert(possible_static_type);
              for(set<string>::iterator it2 = static_type_children.begin(); it2 != static_type_children.end(); ++it2) {
                string expression_type = *it2;
                pair<string, string> static_signature = pair<string, string>(expression_type, possible_static_type);
                static_dispatches.push_back(pair<pair<string, string>, pair<string, string> >(static_signature, method_signature));
              }
            }

            // Regular.
            for(set<string>::iterator it = type_children.begin(); it != type_children.end(); ++it) {
              string possible_type = *it;
              if (!tree.is_child_of(possible_type, class_name)) continue;
              dispatches.push_back(pair<string, pair<string, string> >(possible_type, method_signature));
            }
          }
        }
      }
    }
  }
}

// EXPRESSION: Dispatch.
// NOTES: - This writes out a dispatch with the assumption that the
//          internal structures are updated (with a call to generate_dispatch_structures).
//        - @dispatch_type has three possible values:
//          * 'self' for a self-dispatch (e.g., method_name(args)).
//          * 'static' for a static dispatch (e.g., <expr>@<type>.method(args)).
//          * 'regular' for a normal dispatch (e.g., <expr>.method(args)).
void CodeGenerator::write_dispatch(string dispatch_type) {
  string class_name, method_name;

  if (dispatch_type == "self") {
    if (self_dispatches.size() == 0) {
      throw "Internal Error: self_dispatches is empty during write_dispatch(\"self\") call.";
    }
    pair<string, string> dispatch = self_dispatches[rand() % self_dispatches.size()];
    class_name = dispatch.first;
    method_name = dispatch.second;
  } else if (dispatch_type == "static") {
    if (static_dispatches.size() == 0) {
      throw "Internal Error: static_dispatches is empty during write_dispatch(\"static\") call.";
    }
    pair<pair<string, string>, pair<string, string> > dispatch = static_dispatches[rand() % static_dispatches.size()];
    class_name = dispatch.second.first;
    method_name = dispatch.second.second;

    // Write output.
    writer << '(';
    current_line_length += 1;
    if (current_line_length >= max_line_length) {
      writer << endl;
      print_tabs();
      generate_expression(dispatch.first.first);
      writer << endl;
      print_tabs();
    } else {
      generate_expression(dispatch.first.first);
    }
    writer << ")@" << dispatch.first.second << '.';
    current_line_length += 3 + dispatch.first.second.length();

  } else if (dispatch_type == "regular") {
    if (dispatches.size() == 0) {
      throw "Internal Error: dispatches is empty during write_dispatch(\"regular\") call.";
    }
    pair<string, pair<string, string> > dispatch = dispatches[rand() % dispatches.size()];
    class_name = dispatch.second.first;
    method_name = dispatch.second.second;

    // Write output.
    writer << '(';
    current_line_length += 1;
    if (current_line_length >= max_line_length) {
      writer << endl;
      print_tabs();
      generate_expression(dispatch.first);
      writer << endl;
      print_tabs();
    } else {
      generate_expression(dispatch.first);
    }
    writer << ").";
    current_line_length += 2;

  } else {
    throw "Internal Error: dispatch_type must be one of \"self\", \"static\", \"regular\".";
  }

  vector<pair<string, string> > arguments = tree.class_method_args[class_name][method_name];
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
}

// EXPRESSION: Conditional
// NOTES: - This writes out a conditional.
void CodeGenerator::generate_conditional(string type) {
  // Generate all possible conditional expressions.
  // We keep track of this with a vector where the entry
  //  (a,b) represents if (bool) then (type a) else (type b).
  vector<pair<string, string> > possible_conditionals = vector<pair<string, string> >();

  // Case 1: type is SELF_TYPE and both branches must then be SELF_TYPE.
  if (type == "SELF_TYPE") {
    possible_conditionals.push_back(pair<string, string>("SELF_TYPE", "SELF_TYPE"));
  } 

  // Case 2: type is not SELF_TYPE.
  if (type != "SELF_TYPE") {

    // The branches have the same type possibilities.
    set<string> possible_branch_types = tree.class_descendants[type];
    possible_branch_types.insert(type);

    // We can add SELF_TYPE if the current class <= type.
    if (tree.is_child_of(current_class, type)) {
      possible_branch_types.insert("SELF_TYPE");
    }

    for (set<string>::iterator it = possible_branch_types.begin(); it != possible_branch_types.end(); ++it) {
      string possible_then_type = *it;
      for (set<string>::iterator it2 = possible_branch_types.begin(); it2 != possible_branch_types.end(); ++it2) {
        string possible_else_type = *it2;
        possible_conditionals.push_back(pair<string, string>(possible_then_type, possible_else_type));
      }
    }
  }

  // Choose conditional.
  pair<string, string> branch_types = possible_conditionals[rand() % possible_conditionals.size()];
  string then_type = branch_types.first;
  string else_type = branch_types.second;

  // Write output.

  //    if (bool) {
  writer << "if (";
  current_line_length += 4;
  generate_expression("Bool");
  writer << ") then (" << endl;

  //       then_type
  //    } else {
  indentation_tabs++;
  print_tabs();
  generate_expression(then_type);
  writer << endl;
  indentation_tabs--;
  print_tabs();
  writer << ") else (" << endl;

  //      else_type
  //    }
  indentation_tabs++;
  print_tabs();
  generate_expression(else_type);
  writer << endl;
  indentation_tabs--;
  print_tabs();
  writer << ") fi";
  current_line_length += 4;
}











