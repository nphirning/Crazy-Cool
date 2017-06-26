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
}

// EXPRESSION: Bool constant.
// Notes: Generates true/false randomly and with equal probability.
void CodeGenerator::generate_bool() {
  if (rand() % 2 == 0) {
    writer << "true";
  } else {
    writer << "false";
  }
}

// EXPRESSION: String constant.
// Notes: Generates string of 0-10 characters randomly.
void CodeGenerator::generate_string() {
  int length = rand() % 11;
  writer << '\"' << generate_random_string(length) << '\"';
}

// EXPRESSION: Int constant.
// Notes: Generates number between 0 and INT_MAX.
void CodeGenerator::generate_int() {
  writer << rand();
}
