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
#include "NameGenerator.h"

using namespace std;

// FUNCTION: Constructor.
ClassTree::ClassTree(   const NameGenerator& name_generator,
                        int num_classes,
                        int num_attributes_per_class,
                        int num_methods_per_class,
                        int max_num_method_args,
                        float probability_repeat_method_name):
                        name_generator(name_generator) {

  // Set internal variables.
  this->probability_repeat_method_name = probability_repeat_method_name;
  this->num_classes = num_classes;
  this->max_num_method_args = max_num_method_args;
  this->num_attributes_per_class = num_attributes_per_class;
  this->num_methods_per_class = num_methods_per_class;

  // Sanitize inputs.
  if (probability_repeat_method_name > 1 || probability_repeat_method_name < 0) {
    throw "In ClassTree constructor, probability of repeating a method name must be in [0,1]";
  }
  if (num_classes < 0) {
    throw "In ClassTree constructor, number of classes must be nonnegative.";
  }
  if (num_attributes_per_class < 0) {
    throw "In ClassTree constructor, number of attributes per class must be nonnegative.";
  }
  if (num_methods_per_class < 0) {
    throw "In ClassTree constructor, number of methods per class must be nonnegative.";
  }
  if (max_num_method_args < 0) {
    throw "In ClassTree constructor, maximum number of method arguments must be nonnegative.";
  }
}

// FUNCTION: Print class tree for debugging.
void ClassTree::print_class_information() {

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
      cout << "\tClass: " << current_class << endl;

      cout << "\t\t[ ";
      for (int j = 0; j < class_ancestors[current_class].size(); j++) {
        string current_ancestor = class_ancestors[current_class][j];
        cout << current_ancestor << " ";
      }
      cout << "]" << endl;

      cout << "\t\t{ ";
      for (set<string>::iterator it = class_descendants[current_class].begin();
                      it != class_descendants[current_class].end(); ++it) {
        string current_descendant = *it;
        cout << current_descendant << " ";
      }
      cout << "}" << endl;
  }

  // Print attribute information.
  cout << "Attribute Information" << endl;
  for (int i = 0; i < class_names.size(); i++) {
		string current_class = class_names[i];
		cout << "\tClass: " << current_class << endl;

		for (int j = 0; j < class_attributes[current_class].size(); j++) {
			cout << "\t\t" << class_attributes[current_class][j].first << " ";
			cout << class_attributes[current_class][j].second << endl;
		}
  }

  // Print method information.
  cout << "Method Information" << endl;
  for (int i = 0; i < class_names.size(); i++) {
		string current_class = class_names[i];
		cout << "\tClass: " << current_class << endl;

		for (int j = 0; j < class_method_names[current_class].size(); j++) {
      string current_method_name = class_method_names[current_class][j];
      string current_method_type = class_method_types[current_class][current_method_name];
			cout << "\t\t" << current_method_name << "(";
      for (int k = 0; k < class_method_args[current_class][current_method_name].size(); k++) {
        string arg_name = class_method_args[current_class][current_method_name][k].first;
        string arg_type = class_method_args[current_class][current_method_name][k].second;
        cout << arg_name << ": " << arg_type << ", ";
      }
      cout << "): " << current_method_type << endl;
		}
  }
}

// FUNCTION: Populates class_names with random strings + Main (no basic classes).
void ClassTree::generate_class_names() {
  class_names = vector<string>();

  // Generate class names.
  for (int i = 0; i < num_classes; i++) {
    try {
      string class_name = name_generator.generate(NameType::className, class_names);
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
    set<string> current_descendants = class_descendants[current_class];
    set<string> current_possible_parents = set<string>(possible_parents.begin(), possible_parents.end());
    for (set<string>::iterator it = current_descendants.begin(); it != current_descendants.end(); ++it) {
      current_possible_parents.erase(*it);
    }
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

  generate_class_names();
  generate_inheritance();
  add_basic_classes();
}

// FUNCTION: Generates class attributes.
// NOTE: String, Int, and Bool classes are given no attributes.
void ClassTree::generate_class_attributes() {

  // Initialize data structures.
  class_attributes = map<string, vector<pair<string, string> > >();

  // Generate possible attribute types.
  vector<string> possible_attribute_types = class_names;
  possible_attribute_types.push_back("SELF_TYPE");

  // Generate attributes for each class.
  for (int i = 0; i < class_names.size(); i++) {

    // Extract class and initialize data structures.
    string current_class = class_names[i];

    // Attributes cannot be duplicated within a class and
    // we can't use the names of inherited attributes.
    // Because we aren't traversing the tree in a particular order
    // this means that we need to check the descendants as well.
    vector<string> disallowed_attribute_names = vector<string>();

    // Add ancestor attribute names.
    vector<string> ancestors = class_ancestors[current_class];
    for (int j = 0; j < ancestors.size(); j++) {
      string ancestor = ancestors[j];
      vector<pair<string, string> > ancestor_attributes = class_attributes[ancestor];
      for (int k = 0; k < ancestor_attributes.size(); k++) {
        string attribute_name = ancestor_attributes[k].first;
        disallowed_attribute_names.push_back(attribute_name);
      }
    }

    // Add descendant attribute names.
    set<string> descendants = class_descendants[current_class];
    for (set<string>::iterator it = descendants.begin(); it != descendants.end(); ++it) {
      string descendant = *it;
      vector<pair<string, string> > descendant_attributes = class_attributes[descendant];
      for (int k = 0; k < descendant_attributes.size(); k++) {
        string attribute_name = descendant_attributes[k].first;
        disallowed_attribute_names.push_back(attribute_name);
      }
    }

    class_attributes[current_class] = vector<pair<string, string> >();

    // Basic classes should have no attributes.
    if (current_class == "Object" || current_class == "String" ||
        current_class == "Int" || current_class == "Bool" || current_class == "IO") {
      continue;
    } else {
      for (int j = 0; j < this->num_attributes_per_class; j++) {
        // Choose attribute name/type.
        string attribute_name = name_generator.generate(NameType::attribute, disallowed_attribute_names);
        disallowed_attribute_names.push_back(attribute_name);
        string attribute_type = possible_attribute_types[rand() % possible_attribute_types.size()];

        // Update data structures.
        class_attributes[current_class].push_back(pair<string, string>(attribute_name, attribute_type));
      }
    }
  }
}

// FUNCTION: Adds basic classes to method data structures.
// NOTE: Assumes top-level data structures have been initialized.
void ClassTree::add_basic_class_methods() {

  // Object: abort, type_name, copy.
  class_method_names["Object"] = vector<string>();
  class_method_types["Object"] = map<string, string>();
  class_method_names["Object"].push_back("abort");
  class_method_types["Object"]["abort"] = "Object";
  class_method_names["Object"].push_back("type_name");
  class_method_types["Object"]["type_name"] = "String";
  class_method_names["Object"].push_back("copy");
  class_method_types["Object"]["copy"] = "SELF_TYPE";

  class_method_args["Object"] = map<string, vector<pair<string, string> > >();
  class_method_args["Object"]["abort"] = vector<pair<string, string> >();
  class_method_args["Object"]["type_name"] = vector<pair<string, string> >();
  class_method_args["Object"]["copy"] = vector<pair<string, string> >();

  // String: length, concat, substr.
  class_method_names["String"] = vector<string>();
  class_method_types["String"] = map<string, string>();
  class_method_names["String"].push_back("length");
  class_method_types["String"]["length"] = "Int";
  class_method_names["String"].push_back("concat");
  class_method_types["String"]["concat"] = "String";
  class_method_names["String"].push_back("substr");
  class_method_types["String"]["substr"] = "String";

  class_method_args["String"] = map<string, vector<pair<string, string> > >();
  class_method_args["String"]["length"] = vector<pair<string, string> >();
  class_method_args["String"]["concat"] = vector<pair<string, string> >();
  class_method_args["String"]["concat"].push_back(pair<string, string>("s", "String"));
  class_method_args["String"]["substr"] = vector<pair<string, string> >();
  class_method_args["String"]["substr"].push_back(pair<string, string>("i", "Int"));
  class_method_args["String"]["substr"].push_back(pair<string, string>("l", "Int"));

  // Int.
  class_method_names["Int"] = vector<string>();
  class_method_types["Int"] = map<string, string>();
  class_method_args["Int"] = map<string, vector<pair<string, string> > >();

  // Bool.
  class_method_names["Bool"] = vector<string>();
  class_method_types["Bool"] = map<string, string>();
  class_method_args["Bool"] = map<string, vector<pair<string, string> > >();

  // IO: out_string, out_int, in_string, in_int.
  class_method_names["IO"] = vector<string>();
  class_method_types["IO"] = map<string, string>();
  class_method_names["IO"].push_back("out_string");
  class_method_types["IO"]["out_string"] = "SELF_TYPE";
  class_method_names["IO"].push_back("out_int");
  class_method_types["IO"]["out_int"] = "SELF_TYPE";
  class_method_names["IO"].push_back("in_string");
  class_method_types["IO"]["in_string"] = "String";
  class_method_names["IO"].push_back("in_int");
  class_method_types["IO"]["in_int"] = "Int";

  class_method_args["IO"] = map<string, vector<pair<string, string> > >();
  class_method_args["IO"]["out_string"] = vector<pair<string, string> >();
  class_method_args["IO"]["out_string"].push_back(pair<string, string>("x", "String"));
  class_method_args["IO"]["out_int"] = vector<pair<string, string> >();
  class_method_args["IO"]["out_int"].push_back(pair<string, string>("x", "Int"));
  class_method_args["IO"]["in_string"] = vector<pair<string, string> >();
  class_method_args["IO"]["in_int"] = vector<pair<string, string> >();
}

// FUNCTION: Generates class methods.
void ClassTree::generate_class_methods() {

	// Initialize data structures.
  class_method_names = map<string, vector<string > >();
  class_method_types = map<string, map<string, string> >();
	class_method_args = map<string, map<string, vector<pair<string, string> > > >();

	// Generate possible method types.
  vector<string> possible_types = class_names;
  possible_types.push_back("SELF_TYPE");

  // Handle basic classes.
  add_basic_class_methods();

  // Generate methods for each class.
  for (int i = 0; i < class_names.size(); i++) {

    // Extract class, initialize data structures, skip basic classes.
    string current_class = class_names[i];
    if (current_class == "Object" || current_class == "IO" ||
          current_class == "String" || current_class == "Int" ||
          current_class == "Bool") continue;
    class_method_names[current_class] = vector<string>();
    class_method_types[current_class] = map<string, string>();
    class_method_args[current_class] = map<string, vector<pair<string, string> > >();
    
    // We want to compute the names that we should not use if we 
    // are not redefining a method. Thus, we must look at all parents
    // and all children of the current class. For simplicity, we don't allow
    // redefinition of the method main until it has been created in Main.
    vector<string> unavailable_names = vector<string>();
    unavailable_names.push_back("main");

    // We will also update the method signatures that we can redefine.
    vector<pair<string, string> > redefinable_methods = vector<pair<string, string> >();

    // Iterate through ancestors.
    vector<string> ancestors = class_ancestors[current_class];
    for (int j = 0; j < ancestors.size(); j++) {
      string ancestor = ancestors[j];
      vector<string > ancestor_methods = class_method_names[ancestor];
      for (int k = 0; k < ancestor_methods.size(); k++) {
        string method_name = ancestor_methods[k];
        unavailable_names.push_back(method_name);
        pair<string, string> redefinable_method = pair<string, string>(method_name, current_class);
        redefinable_methods.push_back(redefinable_method);
      }
    }

    // Iterate through descendants.
    set<string> descendants = class_descendants[current_class];
    for (set<string>::iterator it = descendants.begin(); it != descendants.end(); ++it) {
      string descendant = *it;
      vector<string> descendant_methods = class_method_names[descendant];
      for (int k = 0; k < descendant_methods.size(); k++) {
        string method_name = descendant_methods[k];
        unavailable_names.push_back(method_name);
        pair<string, string> redefinable_method = pair<string, string>(method_name, current_class);
        redefinable_methods.push_back(redefinable_method);
      }
    }

    for (int j = 0; j < this->num_methods_per_class; j++) {

      // Method generation.
      // Case 1: main method inside Main class.
      // Case 2: redefinition of method from some class.
      // Case 3: creation of new method.
      if (j == 0 && current_class == "Main") {
        string method_name = "main";

        // Choose return type.
        string method_type = possible_types[rand() % possible_types.size()];

        // Update data structures.
        unavailable_names.push_back(method_name);
        class_method_names[current_class].push_back(method_name);
        class_method_types[current_class][method_name] = method_type;
        class_method_args[current_class][method_name] = vector<pair<string, string> >();

      } else {
        if ((double) rand() / (RAND_MAX) <= this->probability_repeat_method_name) {

          // Choose method to redefine and extract information.
          pair<string, string> method_to_redefine = redefinable_methods[rand() % redefinable_methods.size()];
          string method_name = method_to_redefine.first;
          string method_type = class_method_types[method_to_redefine.second][method_name];
          vector<pair<string, string> > method_redef_args = class_method_args[method_to_redefine.second][method_name];

          // Update data structures.
          class_method_types[current_class][method_name] = method_type;

          // Generate formals (keep return type the same as method_redef_args).
          class_method_args[current_class][method_name] = vector<pair<string, string> >();
          vector<string> method_args = vector<string>();
          for (int k = 0; k < method_redef_args.size(); k++) {
            string argument_type = method_redef_args[k].second;
            string argument_name = name_generator.generate(NameType::methodArgument, method_args);
            class_method_args[current_class][method_name].push_back(pair<string, string>(argument_name, argument_type));
            method_args.push_back(argument_name);
          }

        } else {

          // Generate method name/type.
          string method_name = name_generator.generate(NameType::method, unavailable_names);
          string method_type = possible_types[rand() % possible_types.size()];

          // Update data structures.
          unavailable_names.push_back(method_name);
          class_method_names[current_class].push_back(method_name);
          class_method_types[current_class][method_name] = method_type;
          class_method_args[current_class][method_name] = vector<pair<string, string> >();

          // Generate arguments.
          vector<string> method_args = vector<string>();
          int num_method_args = rand() % (max_num_method_args + 1);
          for (int k = 0; k < num_method_args; k++) {
            string argument_name = name_generator.generate(NameType::methodArgument, method_args);
            method_args.push_back(argument_name);
            string argument_type = class_names[rand() % class_names.size()];
            class_method_args[current_class][method_name].push_back(pair<string, string>(argument_name, argument_type));
          }
        }
      }
    }
  }
}

// FUNCTION: Generates class information.
void ClassTree::generate_class_information() {
  generate_class_tree();
  generate_class_attributes();
	generate_class_methods();
}

// FUNCTION: Checks if child <= parent.
// NOTES: Input is assumed to not be SELF_TYPE.
bool ClassTree::is_child_of(string child, string parent) {
  if (child == parent) return true;
  for (int i = 0; i < class_ancestors[child].size(); i++) {
    string ancestor = class_ancestors[child][i];
    if (ancestor == parent) return true;
  }
  return false;
}
