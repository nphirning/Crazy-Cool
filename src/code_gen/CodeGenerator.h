// File       : CodeGenerator.h
// Description: File for generating COOL code.

#ifndef CODEGENERATOR_H_
#define CODEGENERATOR_H_

#include "ClassTree.h"
#include <fstream>
#include <vector>
#include "SymbolTable.h"
#include "NameGenerator.h"

// Total number of expression types in COOL.
#define NUM_EXPRESSION_TYPES 19

// CLASS CodeGenerator
// -------------------
// This is the standalone class that generates
// COOL code.
//
// Usage: Very simple.
//          1. Construct the class.
//          2. Call generate_code.
//
// That's it! Add configuration when you
// construct the class. Or -- if you want
// some real configuration *power* -- go edit
// the constants in the constructor. Much of
// the configuration is currently hardcoded there.
class CodeGenerator {
public:

  // FUNCTION: Constructor.
  // ----------------------
  // Parameters:
  //    Int num_classes
  //        The number of classes to generate.
  //    String corpus_name
  //        The path (relative or absolute) to the corpus from
  //        which to draw names.
  CodeGenerator (int num_classes = 10, std::string corpus_name = "");

  // FUNCTION generate_code
  // ----------------------
  // Generates code and deposits in the file output.cl.
  void generate_code();


private:

  // Constant values used for expression generation.
  std::vector<std::string> expression_keys = {"new", "bool", "string", "int",
    "identifier", "assignment", "dispatch", "static_dispatch", "self_dispatch",
    "conditional", "loop", "block", "isvoid", "arithmetic", "comparison",
    "int_complement", "bool_complement", "let", "case"};

  // Internal functions for generate_code();
  void generate_expression(std::string type);
  void print_class(std::string class_name);
  void print_attribute(std::string class_name, std::string attribute_name, std::string attribute_type);
  void print_method(std::string class_name, std::string method_name, std::string method_type);
  void print_tabs();

  // Expression generation.
  void generate_expansion(std::string expansion, std::string expression_type);
  float populate_possible_expansions(std::vector<std::string>& possible_expansions,
    std::vector<float>& probability_cutoffs, std::string expression_type);
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
  void generate_comparison();
  void generate_bool_complement();
  void generate_int_complement();
  void generate_let(std::string type);
  void generate_case(std::string type);

  // These values can be configured but
  // are currently constants that are
  // hardcoded inside the constructor.
  std::string output_file;
  int max_recursion_depth;
  int max_block_length;
  int max_let_defines;
  int max_case_branches;
  int max_line_length;
  int max_expression_count;
  float probability_initialized; // This applies to let statements as well.

  // The following is declared in the initialization list ---------

  // NameGenerator configuration.
  int class_name_length;
  int class_attribute_length;
  int class_method_length;
  int class_method_arg_length;
  int class_variable_length;
  NameGenerator name_generator;

  // ClassTree configuration.
  int num_classes;
  int num_attributes_per_class;
  int num_methods_per_class;
  int max_num_method_args;
  float probability_repeat_method_name;
  ClassTree tree;

  // --------------------------------------------------------------

  // Variables used internally.
  std::ofstream writer;
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
