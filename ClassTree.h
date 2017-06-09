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
  ClassTree(int num_classes = 10, std::string word_corpus = "");

  // Accessors and mutators.
  void set_num_classes(int a) { num_classes = a; }
  void set_word_corpus(std::string a) { word_corpus = a; }

  std::vector<std::string> class_names;
  std::map<std::string, std::vector<std::string> > class_ancestors;
  std::map<std::string, std::set<std::string> > class_descendants;

private:
  int num_classes; // Not including basic classes.
  std::string word_corpus;

};

#endif
