// File         : main.cc
// Description  : Main executable of Crazy Cool.

#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <stdlib.h>
#include "CodeGenerator.h"

using namespace std;

bool DEBUG = false;

// FUNCTION: main execution
int main(int argc, char* argv[]) {

  // Initialization
  srand(time(NULL));

  // Flag parsing.
  int num_classes = 10;
  string corpus_name = "";

  int c;
  while ((c = getopt (argc, argv, "c:w:")) != -1) {

    switch(c) {
      case 'c':
        try {
          num_classes = stoi(optarg);
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

  try {

    // Main code generation call.
    CodeGenerator cg(num_classes, corpus_name);
    cg.generate_code();

  } catch (string e) {
    cout << "Error: " << e << endl;
  }

}
