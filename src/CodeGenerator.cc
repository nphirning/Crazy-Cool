// File       : CodeGenerator.cc
// Description: Implementation of COOL code generation.

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "ClassTree.h"
#include "CodeGenerator.h"

using namespace std;

int spaces_per_tab = 4; // Used to keep track of line length.

// FUNCTION: Constructor.
CodeGenerator::CodeGenerator( ClassTree tree,
                              vector<float> expression_weights,
                              string output_file,
                              float probability_initialized,
                              int max_recursion_depth,
                              bool should_break_lines,
                              int max_line_length):
                              writer(output_file) {
  this->tree = tree;
  this->output_file = output_file;
  this->max_recursion_depth = max_recursion_depth;
  this->probability_initialized = probability_initialized;
  this->should_break_lines = should_break_lines;
  this->max_line_length = max_line_length;
  this->current_line_length = 0;

  indentation_tabs = 0;
  recursive_depth = 0;

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
  if (generate_assignment(expression_type, true) && recursive_depth < max_recursion_depth) {
    normalization_factor += expression_map["assignment"];
    possible_expansions.push_back("assignment");
    probability_cutoffs.push_back(expression_map["assignment"]);
  }

  // EXPANSION CHOICE AND GENERATION.

  // Choose expansion.
  transform(probability_cutoffs.begin(), probability_cutoffs.end(), probability_cutoffs.begin(),
            [normalization_factor](float i){return i / normalization_factor;});
  double probability_cutoff = ((double) rand() / (RAND_MAX));
  int expansion_index = 0;
  while (expansion_index < probability_cutoffs.size() - 1 &&
          probability_cutoff >= probability_cutoffs[expansion_index + 1]) {
    expansion_index++;
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
}

// FUNCTION: Prints one method.
// NOTES: Handles updating the identifiers vector with
//        all the arguments in one method.
void CodeGenerator::print_method(string class_name, string method_name, string method_type) {

  // Update identifiers vector.
  map<string, vector<pair<string, string> > > class_method_args = tree.class_method_args[class_name];
  identifiers.insert(identifiers.end(), class_method_args[method_name].begin(), class_method_args[method_name].end());

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

  // Remove from identifiers vector.
  identifiers.resize(identifiers.size() - tree.class_method_args[class_name][method_name].size());
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
    identifiers.insert(identifiers.end(), current_attribute_pairs.begin(), current_attribute_pairs.end());
  }
  identifiers.push_back(pair<string, string>("self", class_name));

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
  identifiers = vector<pair<string, string> >();
}


// FUNCTION: Main function that generates the output code file.
void CodeGenerator::generate_code() {
  identifiers = vector<pair<string, string> >();

  for (int i = 0; i < tree.class_names.size(); i++) {
    string class_name = tree.class_names[i];
    if (class_name == "Object" || class_name == "Bool" ||
        class_name == "String" || class_name == "Int" ||
        class_name == "IO") continue;
    print_class(tree.class_names[i]);
  }
}
