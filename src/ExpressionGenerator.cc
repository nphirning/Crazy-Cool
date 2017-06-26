// File         : ExpressionGenerator.cc
// Description  : Implements the expression generator for CodeGenerator.cc.

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "CodeGenerator.h"

using namespace std;

// EXPRESSION: new.
void CodeGenerator::generate_new(string type) {
  writer << "new " << type;
}

// EXPRESSION: bool constant.
// Notes: Generates true/false randomly and with equal probability.
void CodeGenerator::generate_bool() {
  if (rand() % 2 == 0) {
    writer << "true";
  } else {
    writer << "false";
  }
}
