// File         : ClassTree.cc
// Description  : Implemention of ClassTree class for generating class structure.

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <stdlib.h>
#include "ClassTree.h"
#include "util.h"

#include <thread>
#include <chrono>

using namespace std;

// FUNCTION: Constructor.
ClassTree::ClassTree(int num_classes, string word_corpus, int class_name_length) {
  this->num_classes = num_classes;
  this->class_name_length = class_name_length;
  this->word_corpus = word_corpus;
}

// FUNCTION: Print class tree for debugging.
void ClassTree::print_class_tree() {

  // Print class names.
  cout << "Class Names: [ ";
  for (int i = 0; i < class_names.size(); i++) {
    cout << class_names[i] << " ";
  }
  cout << "]" << endl;

  // Print inheritance data.
  cout << "Inheritance Data" << endl;
  for (int i = 0; i < class_names.size(); i++) {
      string current_class = class_names[i];
      cout << "Class: " << current_class << endl;

      cout << "\t[ ";
      for (int j = 0; j < class_ancestors[current_class].size(); j++) {
        string current_ancestor = class_ancestors[current_class][j];
        cout << current_ancestor << " ";
      }
      cout << "]" << endl;

      cout << "\t{ ";
      for (set<string>::iterator it = class_descendants[current_class].begin();
                      it != class_descendants[current_class].end(); ++it) {
        string current_descendant = *it;
        cout << current_descendant << " ";
      }
      cout << "}" << endl;
  }
}

// FUNCTION: Populates class_names with random strings + Main (no basic classes).
void ClassTree::generate_class_names() {
  class_names = vector<string>();

  // Generate class names.
  for (int i = 0; i < num_classes; i++) {
    try {
      string class_name = generate_class_name(class_name_length);
      class_names.push_back(class_name);
    } catch (string e) {
      cout << "Error: " << e << endl;
    }
  }

  // Add Main class.
  class_names.push_back("Main");

}

// FUNCTION: Updates ancestor vectors for child-parent connection.
void ClassTree::update_ancestor_vectors(string child, string parent) {

  // Append parent lineage to child ancestor vector.
  vector<string> parent_ancestors = class_ancestors[parent];
  vector<string>& child_ancestors = class_ancestors[child];
  child_ancestors.push_back(parent);
  child_ancestors.insert(child_ancestors.end(), parent_ancestors.begin(), parent_ancestors.end());

  // Update the ancestors of each children of child.
  set<string> child_descendants = class_descendants[child];
  for (set<string>::iterator it = child_descendants.begin(); it != child_descendants.end(); ++it) {
    string current_class = *it;
    vector<string>& current_ancestors = class_ancestors[current_class];
    current_ancestors.insert(current_ancestors.end(), child_ancestors.begin(), child_ancestors.end());
  }
}

// FUNCTION: Updates child sets for child-parent connection.
// NOTE: Assumes ancestor vectors have been updated.
void ClassTree::update_child_sets(string child, string parent) {

  set<string> child_descendants = class_descendants[child];
  child_descendants.insert(child);
  vector<string> child_ancestors = class_ancestors[child];
  for (int i = 0; i < child_ancestors.size(); i++) {
    string current_ancestor = child_ancestors[i];
    set<string>& current_descendants = class_descendants[current_ancestor];
    current_descendants.insert(child_descendants.begin(), child_descendants.end());
  }
}

// FUNCTION: Generates inheritance for all classes in class_names.
// NOTE:  This assumes that class_names does NOT contain basic classes.
// NOTE:  This will add Object, IO to class_descendants and class_ancestors,
//        but not other basic classes.
void ClassTree::generate_inheritance() {

  // Initialize map data structures.
  class_ancestors = map<string, vector<string> >();
  class_descendants = map<string, set<string> >();

  for (int i = 0; i < class_names.size(); i++) {
    class_ancestors[class_names[i]] = vector<string>();
    class_descendants[class_names[i]] = set<string>();
  }

  // Add Object and IO manually to ancestor and descendant maps.
  class_descendants["Object"] = set<string>();
  class_descendants["Object"].insert("IO");
  class_descendants["IO"] = set<string>();
  class_ancestors["Object"] = vector<string>();
  class_ancestors["IO"] = vector<string>();
  class_ancestors["IO"].push_back("Object");

  // Create a vector of possible parents.
  vector<string> possible_parents = vector<string>(class_names);
  possible_parents.push_back("IO");
  possible_parents.push_back("Object");

  for(int i = 0; i < class_names.size(); i++) {
    string current_class = class_names[i];
    string parent_class = "";

    // Compute possible parents for current class.
    set<string> current_possible_parents = set<string>(possible_parents.begin(), possible_parents.end());
    set<string> current_descendants = class_descendants[current_class];
    set_difference( current_possible_parents.begin(),
                      current_possible_parents.end(),
                      current_descendants.begin(),
                      current_descendants.end(),
                      inserter(current_possible_parents,
                        current_possible_parents.begin()));
    current_possible_parents.erase(current_class);

    // Choose parent randomly.
    set<string>::iterator it = current_possible_parents.begin();
    advance(it, rand() % current_possible_parents.size());
    parent_class = *it;

    // Update data structures.
    update_ancestor_vectors(current_class, parent_class);
    update_child_sets(current_class, parent_class);
  }
}

// FUNCTION: Adds basic classes to data structures.
// NOTE:  Assumes that Object, IO have been added to class_descendants and
//        class_ancestors but no other basic classes have been added to any
//        data structures.
void ClassTree::add_basic_classes() {
  class_names.push_back("Object");
  class_names.push_back("IO");
  class_names.push_back("Int");
  class_names.push_back("String");
  class_names.push_back("Bool");
  class_descendants["Int"] = set<string>();
  class_descendants["String"] = set<string>();
  class_descendants["Bool"] = set<string>();
  class_ancestors["Int"] = vector<string>();
  class_ancestors["Int"].push_back("Object");
  class_ancestors["String"] = vector<string>();
  class_ancestors["String"].push_back("Object");
  class_ancestors["Bool"] = vector<string>();
  class_ancestors["Bool"].push_back("Object");
}

// FUNCTION: Generates class tree.
void ClassTree::generate_class_tree() {

  // Generate class names.
  generate_class_names();

  // Generate inheritance.
  generate_inheritance();

  // Add basic classes.
  add_basic_classes();

}
