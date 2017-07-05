// File       : CodeGenerator.cc
// Description: Implementation of COOL code generation.

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "ClassTree.h"
#include "CodeGenerator.h"
#include "SymbolTable.h"

using namespace std;

int spaces_per_tab = 4; // Used to keep track of line length.

// FUNCTION: Constructor.
CodeGenerator::CodeGenerator( ClassTree tree,
                              vector<float> expression_weights,
                              string output_file,
                              float probability_initialized,
                              int max_recursion_depth,
                              bool should_break_lines,
                              int max_line_length,
                              int max_expression_count):
                              writer(output_file) {
  this->tree = tree;
  this->output_file = output_file;
  this->max_recursion_depth = max_recursion_depth;
  this->probability_initialized = probability_initialized;
  this->should_break_lines = should_break_lines;
  this->max_line_length = max_line_length;
  this->current_line_length = 0;
  this->max_expression_count = max_expression_count;

  this->indentation_tabs = 0;
  this->recursive_depth = 0;
  this->expression_count = 0;

  this->identifiers = SymbolTable();

  // Create map from name -> weights.
  this->expression_map = map<string, float>();
  for (int i = 0; i < expression_keys.size(); i++) {
    this->expression_map[expression_keys[i]] = expression_weights[i];
  }
}

// FUNCTION: Generates an expression of the given type.
void CodeGenerator::generate_expression(string expression_type) {

  // Increase recursive depth.
  recursive_depth++;

  // Increase expression count.
  expression_count++;

  // Compute possible expansions and keep track of weights.
  vector<string> possible_expansions = vector<string>();
  float normalization_factor = 0.0;
  vector<float> probability_cutoffs;

  // POSSIBLE EXPRESSION TYPES.
  // 1. New.
  // 2. Bool constants.
  // 3. String constants.
  // 4. Int constants.
  // 5. Identifiers.
  // 6. Assignment.
  // 7. Dispatch.
  // 8. Static dispatch.
  // 9. Self dispatch.
  // 10. Conditional.
  // 11. Loop.

  // New.
  normalization_factor += expression_map["new"];
  possible_expansions.push_back("new");
  probability_cutoffs.push_back(expression_map["new"]);

  // Bool constants (only valid if we're generating a Bool).
  if (expression_type == "Bool") {
    normalization_factor += expression_map["bool"];
    possible_expansions.push_back("bool");
    probability_cutoffs.push_back(expression_map["bool"]);
  }

  // String constants (only valid if we're generating a String).
  if (expression_type == "String") {
    normalization_factor += expression_map["string"];
    possible_expansions.push_back("string");
    probability_cutoffs.push_back(expression_map["string"]);
  }

  // Int constants (only valid if we're generating an Int).
  if (expression_type == "Int") {
    normalization_factor += expression_map["int"];
    possible_expansions.push_back("int");
    probability_cutoffs.push_back(expression_map["int"]);
  }

  // Identifiers.
  if (generate_identifier(expression_type, true)) {
    normalization_factor += expression_map["identifier"];
    possible_expansions.push_back("identifier");
    probability_cutoffs.push_back(expression_map["identifier"]);
  }

  // Assignment.
  if (generate_assignment(expression_type, true) && recursive_depth < max_recursion_depth
                                                  && expression_count < max_expression_count) {
    normalization_factor += expression_map["assignment"];
    possible_expansions.push_back("assignment");
    probability_cutoffs.push_back(expression_map["assignment"]);
  }

  // Dispatch.
  generate_dispatch_structures(expression_type);
  if (recursive_depth < max_recursion_depth && expression_count < max_expression_count) {
    if (self_dispatches.size() != 0) {
      normalization_factor += expression_map["self_dispatch"];
      possible_expansions.push_back("self_dispatch");
      probability_cutoffs.push_back(expression_map["self_dispatch"]);
    }
    if (static_dispatches.size() != 0) {
      normalization_factor += expression_map["static_dispatch"];
      possible_expansions.push_back("static_dispatch");
      probability_cutoffs.push_back(expression_map["static_dispatch"]);
    }
    if (dispatches.size() != 0) {
      normalization_factor += expression_map["dispatch"];
      possible_expansions.push_back("dispatch");
      probability_cutoffs.push_back(expression_map["dispatch"]);
    }
  }

  // Conditional.
  if (recursive_depth < max_recursion_depth && expression_count < max_expression_count) {
    normalization_factor += expression_map["conditional"];
    possible_expansions.push_back("conditional");
    probability_cutoffs.push_back(expression_map["conditional"]);
  }

  // Loop.
  if (expression_type == "Object" && recursive_depth < max_recursion_depth 
                                    && expression_count < max_expression_count) {
    normalization_factor += expression_map["loop"];
    possible_expansions.push_back("loop");
    probability_cutoffs.push_back(expression_map["loop"]);
  }

  // EXPANSION CHOICE AND GENERATION.

  // Choose expansion.
  float probability_sum = 0.0;
  for (int i = 0; i < probability_cutoffs.size(); i++) {
    probability_sum += probability_cutoffs[i];
    probability_cutoffs[i] = probability_sum;
  }
  transform(probability_cutoffs.begin(), probability_cutoffs.end(), probability_cutoffs.begin(),
            [normalization_factor](float i){return i / normalization_factor;});
  double probability_cutoff = ((double) rand() / (RAND_MAX));
  int expansion_index = 0;
  for (; expansion_index < probability_cutoffs.size(); expansion_index++) {
    if (probability_cutoff < probability_cutoffs[expansion_index]) break;
  }

  string expansion = possible_expansions[expansion_index];

  // Generate code corresponding to chosen expansion.
  if (expansion == "new") {
    generate_new(expression_type);
  } else if (expansion == "bool") {
    generate_bool();
  } else if (expansion == "string") {
    generate_string();
  } else if (expansion == "int") {
    generate_int();
  } else if (expansion == "identifier") {
    generate_identifier(expression_type, false);
  } else if (expansion == "assignment") {
    generate_assignment(expression_type, false);
  } else if (expansion == "self_dispatch") {
    write_dispatch("self");
  } else if (expansion == "static_dispatch") {
    write_dispatch("static");
  } else if (expansion == "dispatch") {
    write_dispatch("regular");
  } else if (expansion == "conditional") {
    generate_conditional(expression_type);
  } else if (expansion == "loop") {
    generate_loop();
  } else {
    throw "Internal error: chosen expression type not a possible expansion.";
  }

  // Reduce recursive depth.
  recursive_depth--;

}

// FUNCTION: Prints the number of tabs indicated by global indentation_tabs.
// NOTES: - This should only be used on a new line. Otherwise, current_line_length
//          will be incorrect.
void CodeGenerator::print_tabs() {
  current_line_length = indentation_tabs * spaces_per_tab;
  writer << string(indentation_tabs, '\t');
}

// FUNCTION: Prints one attribute.
void CodeGenerator::print_attribute(string class_name, string attribute_name, string attribute_type) {
  print_tabs();
  indentation_tabs++;

  writer << attribute_name << ": " << attribute_type;
  current_line_length += attribute_name.length() + attribute_type.length() + 2;

  // Generate initialization based on initialization probability.
  double cutoff = ((double) rand() / (RAND_MAX));
  if (cutoff >= probability_initialized) {
    writer << ";" << endl;
  } else {
    writer << " <- (";
    current_line_length += 5;
    generate_expression(attribute_type);
    writer << ");" << endl;
  }

  indentation_tabs--;
}

// FUNCTION: Prints one method.
// NOTES: Handles updating the identifiers vector with
//        all the arguments in one method.
void CodeGenerator::print_method(string class_name, string method_name, string method_type) {

  // Update identifiers.
  identifiers.enter_scope();
  map<string, vector<pair<string, string> > > class_method_args = tree.class_method_args[class_name];
  vector<pair<string, string> > method_args = class_method_args[method_name];
  for (int i = 0; i < method_args.size(); i++) {
    identifiers.add_id(method_args[i].first, method_args[i].second);
  }

  // Tabs + method name.
  print_tabs();
  writer << method_name << "(";

  // Print arguments.
  vector<pair<string, string> > args = tree.class_method_args[class_name][method_name];
  for (int i = 0; i < args.size(); i++) {
    if (i == args.size() - 1) {
      writer << args[i].first << ": " << args[i].second;
    } else {
      writer << args[i].first << ": " << args[i].second << ", ";
    }
  }

  // Print return type.
  writer << ") : " << method_type << " {" << endl;
  indentation_tabs++;

  // Generate body.
  print_tabs();
  generate_expression(method_type);
  writer << endl;

  // End method declaration.
  indentation_tabs--;
  print_tabs();
  writer << "};" << endl;

  // Remove arguments from identifiers.
  identifiers.exit_scope();
}

// FUNCTION: Prints one class.
// NOTES: - updates identifiers vectors with attributes + self.
//        - updates current_class as well.
void CodeGenerator::print_class(string class_name) {

  current_class = class_name;

  // Update identifiers vector with local variables.
  vector<string> attribute_holders = tree.class_ancestors[class_name];
  attribute_holders.push_back(class_name);
  for (int i = 0; i < attribute_holders.size(); i++) {
    vector<pair<string, string> > current_attribute_pairs = tree.class_attributes[attribute_holders[i]];
    for (int j = 0; j < current_attribute_pairs.size(); j++) {
      identifiers.add_id(current_attribute_pairs[j].first, current_attribute_pairs[j].second);
    }
  }
  identifiers.add_id("self", class_name);

  // Print class declaration line.
  string parent = tree.class_ancestors[class_name][0];
  print_tabs();
  if (parent == "Object") {
    writer << "class " << class_name << " {" << endl;
  } else {
    writer << "class " << class_name << " inherits " << parent << " {" << endl;
  }
  indentation_tabs++;

  // Print attributes.
  for (int i = 0; i < tree.class_attributes[class_name].size(); i++) {
    string attribute_name = tree.class_attributes[class_name][i].first;
    string attribute_type = tree.class_attributes[class_name][i].second;
    print_attribute(class_name, attribute_name, attribute_type);
  }

  // One line between methods and attributes.
  writer << endl;

  // Print methods.
  for (int i = 0; i < tree.class_method_names[class_name].size(); i++) {
    string method_name = tree.class_method_names[class_name][i];
    string method_type = tree.class_method_types[class_name][method_name];
    print_method(class_name, method_name, method_type);
  }

  // Print class end.
  indentation_tabs--;
  print_tabs();
  writer << "};" << endl << endl;

  // Reset identifiers vector.
  identifiers.exit_scope();
}


// FUNCTION: Main function that generates the output code file.
void CodeGenerator::generate_code() {

  for (int i = 0; i < tree.class_names.size(); i++) {
    string class_name = tree.class_names[i];
    if (class_name == "Object" || class_name == "Bool" ||
        class_name == "String" || class_name == "Int" ||
        class_name == "IO") continue;
    print_class(tree.class_names[i]);

    if (i % 10 == 0 && i > 0) cout << i << " classes generated." << endl;
  }
}
