// File : main.cc

#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <stdlib.h>
#include "ClassTree.h"
#include "CodeGenerator.h"

using namespace std;

bool DEBUG = true;

// FUNCTION: main execution
int main(int argc, char* argv[]) {

  // Initialization
  srand(time(NULL));

  // Flag parsing.

  int num_classes = 5;
  string corpus_name = "";

  int c;
  while ((c = getopt (argc, argv, "cw:")) != -1) {
    switch(c) {
      case 'c':
        try {
          int num_classes = stoi(optarg);
        }
        catch (const invalid_argument& ia) {
          cout << "Invalid argument: " << ia.what() << endl;
        }
        catch (const std::out_of_range& oor) {
          cout << "Out of Range error: " << oor.what() << endl;
        }
        break;
      case 'w':
        corpus_name = optarg;
        break;
    }
  }

  // Generate class information.
  ClassTree tree = ClassTree(num_classes);
  tree.generate_class_information();
  if (DEBUG) tree.print_class_information();

  // Generate code.
  CodeGenerator generator(tree);
  generator.generate_code();

}
