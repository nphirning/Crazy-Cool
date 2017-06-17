// File       : CodeGenerator.h
// Description: File for generating the

#ifndef CODEGENERATOR_H_
#define CODEGENERATOR_H_

#include "ClassTree.h"
#include <fstream>


class CodeGenerator {
public:

  // Constructor.
  CodeGenerator(ClassTree tree,
                std::string output_file = "output.cl",
                float probability_initialized = 0.5,
                int max_recursion_depth = 10);

  void generate_code();


private:

  // Internal functions for generate_code();
  void print_class(std::string class_name);
  void print_attribute(std::string class_name, std::string attribute_name, std::string attribute_type);
  void print_method(std::string class_name, std::string method_name, std::string method_type);
  void print_tabs();

  std::string output_file;
  ClassTree tree;
  int max_recursion_depth;
  float probability_initialized;
  std::ofstream writer;
};

#endif
