// File         : ClassTree.h
// Description  : Header file for ClassTree class.

#ifndef CLASSTREE_H_
#define CLASSTREE_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include "NameGenerator.h"

// Generates information for class names, attribute names, method names.
// NOTE: None of the structures include the special values SELF_TYPE and self.
class ClassTree {
public:

  // FUNCTION: Constructor.
  // ----------------------
  // Parameters:
  //    NameGenerator name_generator
  //            Any NameGenerator object will do. This is used
  //            for name generation. Duh.
  //    Int num_classes
  //            This is the number of classes to generate. We
  //            throw if this is not positive.
  //    Int num_attributes_per_class
  //            The number of attributes to generate in each 
  //            class. Currently, there is no randomness.
  //    Int num_methods_per_class
  //            The number of methods to generate in each class.
  //    Int max_num_method_args
  //            The maximum number of arguments to each method.
  //            For each method, we'll draw from [0, max_num_method_args]
  //            to choose the number of arguments.
  //    Int probability_repeat_method_name
  //            Must be in the range [0,1]. This reflects the 
  //            probability that we repeat a method name (i.e., 
  //            method redefinition).
  ClassTree(const NameGenerator& name_generator,
            int num_classes,
            int num_attributes_per_class,
            int num_methods_per_class,
            int max_num_method_args,
            float probability_repeat_method_name);

  // FUNCTION: generate_class_information.
  // ------------------------------------
  // This populates the many data structures outlined below.
  void generate_class_information();

  // FUNCTION: print_class_information.
  // ---------------------------------
  // This prints out the information in the class for debugging. 
  // Look at the implementation for more information.
  void print_class_information();

  // FUNCTION: is_child_of
  // ---------------------
  // Checks if child <= parent.
  // Parameters:
  //    String child
  //            Name of a class inside class_names.
  //    Sting parent
  //            Name of a class inside class_names.
  // Returns:
  //    True if child <= parent.
  bool is_child_of(std::string child, std::string parent);

  // Data structures for class tree information.
  // NOTE: Most maps are intuitive, but we record their official definitions here.
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

  // Other internal methods. See implementation for details.
  void add_basic_class_methods();
  void generate_class_names();
  void generate_inheritance();
  void add_basic_classes();
  void update_ancestor_vectors(std::string child, std::string parent);
  void update_child_sets(std::string child, std::string parent);

  // Parameters for class generation.
  const NameGenerator& name_generator;
  int num_classes;                // Doesn't include basic classes.
  int num_attributes_per_class;
  int num_methods_per_class;
  int max_num_method_args;
  float probability_repeat_method_name;
};

#endif
