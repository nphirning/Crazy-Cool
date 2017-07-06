// File       : CodeGenerator.h
// Description: File for generating the

#ifndef CODEGENERATOR_H_
#define CODEGENERATOR_H_

#include "ClassTree.h"
#include <fstream>
#include <vector>
#include "SymbolTable.h"

#define NUM_EXPRESSION_TYPES 14

class CodeGenerator {
public:

  // Constructor.
  // NOTE: Expression weights are in the following order:
  //  [new, ]
  CodeGenerator(ClassTree tree,
                std::vector<float> expression_weights = std::vector<float>(NUM_EXPRESSION_TYPES, 1.0),
                std::string output_file = "output.cl",
                float probability_initialized = 0.75,
                int max_recursion_depth = 5,
                bool should_break_lines = true,
                int max_block_length = 5,
                int max_line_length = 80,
                int max_expression_count = 10000);

  // Main code generation function.
  void generate_code();


private:

  // Constant values used for expression generation.
  std::vector<std::string> expression_keys = {"new", "bool", "string", "int",
                                                    "identifier", "assignment",
                                                    "dispatch", "static_dispatch",
                                                    "self_dispatch", "conditional",
                                                    "loop", "block", "isvoid",
                                                    "arithmetic"};

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
  void generate_dispatch_structures(std::string type);
  void write_dispatch(std::string dispatch_type);
  void generate_conditional(std::string type);
  void generate_loop();
  void generate_block(std::string type);
  void generate_isvoid();
  void generate_arithmetic();

  // Configurable inputs.
  std::string output_file;
  int max_recursion_depth;
  int max_block_length;
  bool should_break_lines;
  int max_line_length;
  int max_expression_count;
  float probability_initialized;
  std::ofstream writer;
  ClassTree tree;

  // Internal variables.
  std::map<std::string, float> expression_map;
  SymbolTable identifiers;
  std::string current_class;
  int current_line_length; // Currently only updated for expression generation.
  int recursive_depth;
  int expression_count;
  int indentation_tabs;

  // Internal dispatch structures.
  std::vector<std::pair<std::string, std::string> > self_dispatches;
  std::vector<std::pair<std::string, std::pair<std::string, std::string> > > dispatches;
  std::vector<std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string> > > static_dispatches;
};

#endif
