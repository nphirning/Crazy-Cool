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
            int num_method_args = 5,
            float probability_repeat_method_name = 0.2,
            std::string word_corpus = "",
            int class_name_length = 10,
            int attribute_name_length = 5,
            int method_name_length = 5,
            int method_arg_name_length = 5);

  // Populates data structures with information.
  void generate_class_information();

  // Print method for debugging.
  void print_class_information();

  // Data structures for class tree information.
  // NOTE: Most maps are intuitive, but just for the record:
  //    class_names       : vector of class names as strings
  //    class_ancestors   : map from class name to ordered vector of ancestors
  //    class_descendants : map from class name to set of all children
  //    class_attributes  : map from class name to vector of (name, type) pairs
  //    class_method_names: map from class name to vector of names
  //    class_method_types: map from class name to map from method name to method type
  //    class_method_args : map from class name to map of method name -> vector of (arg name, arg type)
  std::vector<std::string> class_names;
  std::map<std::string, std::vector<std::string> > class_ancestors;
  std::map<std::string, std::set<std::string> > class_descendants;
  std::map<std::string, std::vector<std::pair<std::string, std::string> > > class_attributes;
  std::map<std::string, std::vector<std::string > > class_method_names;
  std::map<std::string, std::map<std::string, std::string> > class_method_types;
  std::map<std::string, std::map<std::string, std::vector<std::pair<std::string, std::string> > > > class_method_args;

private:

  // Internal methods for generate_class_information().
  // These should be called in the following order.
  void generate_class_tree();
  void generate_class_attributes();
  void generate_class_methods();

  // Internal methods used by generate_class_methods().
  void add_basic_class_methods();

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
  int method_arg_name_length;
  int num_method_args;
  float probability_repeat_method_name;
  std::string word_corpus;  // Path to corpus for class name selection.

};

#endif
