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

  // Constructor.
  ClassTree(int num_classes = 10,
            std::string word_corpus = "",
            int class_name_length = 10);

  // Populates data structures with information.
  void generate_class_tree();

  // Print method for debugging.
  void print_class_tree();

  // Data structures for class tree information.
  std::vector<std::string> class_names;
  std::map<std::string, std::vector<std::string> > class_ancestors;
  std::map<std::string, std::set<std::string> > class_descendants;

private:

  // Internal methods used by generate_class_tree();
  void generate_class_names();
  void generate_inheritance();
  void add_basic_classes();
  void update_ancestor_vectors(std::string child, std::string parent);
  void update_child_sets(std::string child, std::string parent);

  // Parameters for class generation.
  int num_classes;          // Not including basic classes.
  int class_name_length;    // Length of randomly generated strings.
  std::string word_corpus;  // Path to corpus for class name selection.

};

#endif
