// File : CrazyCool.cc

#include <unistd.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include "ClassTree.h"

using namespace std;

int main(int argc, char* argv[]) {

  // Initialization
  srand(time(NULL));

  // Flag parsing.

  int num_classes = 10;
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

    // Construct class tree.
  }





}
