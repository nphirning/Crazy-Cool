// File       : CodeGenerator.cc
// Description: Implementation of COOL code generation.

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "ClassTree.h"
#include "CodeGenerator.h"

using namespace std;

int indentation_tabs = 0;
int recursive_depth = 0;

// FUNCTION: Constructor.
CodeGenerator::CodeGenerator( ClassTree tree,
                              vector<float> expression_weights,
                              string output_file,
                              float probability_initialized,
                              int max_recursion_depth):
                              writer(output_file) {
  this->tree = tree;
  this->output_file = output_file;
  this->max_recursion_depth = max_recursion_depth;
  this->probability_initialized = probability_initialized;

  // Extract expression weights to map.
  this->expression_weights = map<string, float>();
  string keys[] = ["new"];
  for (int i = 0; i < 1; i++) {
    this->expression_weights[keys[i]] = expression_weights[i];
  }
}

void CodeGenerator::generate_expression(string expression_type) {
  
  // Compute possible expansions and keep track of weights.
  vector<string> possible_expansions = vector<string>();
  float normalization_factor = 0.0;
  vector<float> probability_cutoffs;

  // New.
  normalization_factor += expression_weights["new"];
  possible_expansions.push_back("new");

  // Choose expansion.
  transform(probability_cutoffs.begin(), probability_cutoffs.end(), probability_cutoffs.begin(),
            [normalization_factor](float i){return i / normalization_factor;});

}

// FUNCTION: Prints the number of tabs indicated by global indentation_tabs.
void CodeGenerator::print_tabs() {
  writer << string(indentation_tabs, '\t');
}

// FUNCTION: Prints one attribute.
void CodeGenerator::print_attribute(string class_name, string attribute_name, string attribute_type) {
  print_tabs();
  writer << attribute_name << ": " << attribute_type << " <- ";

  // FOR TESTING
  writer << "new " << attribute_type << ";" << endl;
}

// FUNCTION: Prints one method.
void CodeGenerator::print_method(string class_name, string method_name, string method_type) {

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

  print_tabs();
  writer << "new " << method_type << endl;

  // End method declaration.
  indentation_tabs--;
  print_tabs();
  writer << "};" << endl;
}

// FUNCTION: Prints one class.
void CodeGenerator::print_class(string class_name) {

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

}


// FUNCTION: Main function that generates the output code file.
void CodeGenerator::generate_code() {
  for (int i = 0; i < tree.class_names.size(); i++) {
    string class_name = tree.class_names[i];
    if (class_name == "Object" || class_name == "Bool" ||
        class_name == "String" || class_name == "Int" ||
        class_name == "IO") continue;
    print_class(tree.class_names[i]);
  }
}
