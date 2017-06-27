// File       : CodeGenerator.h
// Description: File for generating the

#ifndef CODEGENERATOR_H_
#define CODEGENERATOR_H_

#include "ClassTree.h"
#include <fstream>
#include <vector>


class CodeGenerator {
public:

  // Constructor.
  // NOTE: Expression weights are in the following order:
  //  [new, ]
  CodeGenerator(ClassTree tree,
                std::vector<float> expression_weights = std::vector<float>(1, 1.0),
                std::string output_file = "output.cl",
                float probability_initialized = 0.5,
                int max_recursion_depth = 5,
                bool should_break_lines = true,
                int max_line_length = 80,
                int max_expression_count = 10000);

  // Main code generation function.
  void generate_code();

private:

  // Internal functions for generate_code();
  void generate_expression(std::string type);
  void print_class(std::string class_name);
  void print_attribute(std::string class_name, std::string attribute_name, std::string attribute_type);
  void print_method(std::string class_name, std::string method_name, std::string method_type);
  void print_tabs();

  // Expression generation.
  void generate_new(std::string type);
  void generate_bool();
  void generate_string();
  void generate_int();
  bool generate_identifier(std::string type, bool abort_early);
  bool generate_assignment(std::string type, bool abort_early);
  bool generate_dispatch(std::string type, bool abort_early);

  std::string output_file;
  ClassTree tree;
  std::vector<std::string> expression_keys = {"new", "bool", "string", "int",
                                                    "identifier", "assignment"
                                                    "dispatch", "static_dispatch"};
  std::map<std::string, float> expression_map;
  int max_recursion_depth;
  bool should_break_lines;
  int max_line_length;
  int max_expression_count;
  float probability_initialized;
  std::ofstream writer;

  // State-dependent variables.
  std::vector<std::pair<std::string, std::string> > identifiers;
  std::string current_class;
  int current_line_length; // Currently only updated for expression generation.
  int recursive_depth;
  int expression_count;
  int indentation_tabs;
};

#endif
