// File         : ClassTree.h
// Description  : Header file for ClassTree class.

#ifndef CLASSTREE_H_
#define CLASSTREE_H_

#include <string>
#include <vector>
#include <map>
#include <set>

// Generates information for class names, attribute names, method names.
// NOTE: None of the structures include the special values SELF_TYPE and self.
class ClassTree {
public:

  // Constructor.
  ClassTree(int num_classes = 10,
            int num_attributes_per_class = 3,
            int num_methods_per_class = 3,
            float probability_repeat_method_name = 0.2,
            std::string word_corpus = "",
            int class_name_length = 10,
            int attribute_name_length = 5,
            int method_name_length = 5);

  // Populates data structures with information.
  void generate_class_information();

  // Print method for debugging.
  void print_class_information();

  // Data structures for class tree information.
  std::vector<std::string> class_names;
  std::map<std::string, std::vector<std::string> > class_ancestors;
  std::map<std::string, std::set<std::string> > class_descendants;
  std::map<std::string, std::vector<std::pair<std::string, std::string> > > class_attributes;

private:

  // Internal methods for generate_class_information().
  // These should be called in the following order.
  void generate_class_tree();
  void generate_class_attributes();
  void generate_class_methods();

  // Internal methods used by generate_class_tree().
  void generate_class_names();
  void generate_inheritance();
  void add_basic_classes();
  void update_ancestor_vectors(std::string child, std::string parent);
  void update_child_sets(std::string child, std::string parent);

  // Parameters for class generation.
  int num_classes;          // Not including basic classes.
  int num_attributes_per_class;
  int num_methods_per_class;
  int class_name_length;    // Length of randomly generated strings.
  int attribute_name_length;
  int method_name_length;
  float probability_repeat_method_name;
  std::string word_corpus;  // Path to corpus for class name selection.

};

#endif
