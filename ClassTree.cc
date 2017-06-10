// File         : ClassTree.cc
// Description  : Implemention of ClassTree class for generating class structure.

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include "ClassTree.h"
#include "util.h"

using namespace std;

ClassTree::ClassTree(float probability_inherits, int num_classes,
                                  int class_name_length, string word_corpus) {
  this->probability_inherits = probability_inherits;
  this->num_classes = num_classes;
  this->class_name_length = class_name_length;
  this->word_corpus = word_corpus;
}

void ClassTree::generate_class_tree() {

  // Initialize data structures.
  class_names = vector<string>();
  class_ancestors = map<string, vector<string> >();
  class_descendants = map<string, set<string> >();

  // Generate class names.
  for (int i = 0; i < num_classes; i++) {
    try {
      string class_name = generate_class_name(class_name_length);
      class_names.push_back(class_name);
    } catch (string e) {
      cout << "Error: " << e << endl;
    }
  }


}
