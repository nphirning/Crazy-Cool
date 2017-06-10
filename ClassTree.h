// File         : ClassTree.h
// Description  : Header file for ClassTree class.

#ifndef CLASSTREE_H_
#define CLASSTREE_H_

#include <string>
#include <vector>
#include <map>
#include <set>

class ClassTree {
public:
  void generate_class_tree();

  // Constructor.
  ClassTree(float probability_inherits = 0.5,
            int num_classes = 10,
            int class_name_length = 10,
            std::string word_corpus = "");

  // Accessors and mutators.
  void set_num_classes(int a) { num_classes = a; }
  void set_word_corpus(std::string a) { word_corpus = a; }

  // Data structures for class tree information.
  std::vector<std::string> class_names;
  std::map<std::string, std::vector<std::string> > class_ancestors;
  std::map<std::string, std::set<std::string> > class_descendants;

private:
  float probability_inherits; // float in interval [0,1];
  int num_classes; // Not including basic classes.

  // Two options for class name generation
  //  - random generation
  //  - picking from a word corpus
  int class_name_length;
  std::string word_corpus; // path from source directory

};

#endif
