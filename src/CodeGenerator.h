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
                int max_recursion_depth = 10);

  // Main code generation function.
  void generate_code();

private:

  // Internal functions for generate_code();
  void generate_expression(std::string type);
  void print_class(std::string class_name);
  void print_attribute(std::string class_name, std::string attribute_name, std::string attribute_type);
  void print_method(std::string class_name, std::string method_name, std::string method_type);
  void print_tabs();

  std::string output_file;
  ClassTree tree;
  std::string expression_keys[] = ["new"];
  std::map<string, float> expression_weights;
  int max_recursion_depth;
  float probability_initialized;
  std::ofstream writer;
};

#endif
