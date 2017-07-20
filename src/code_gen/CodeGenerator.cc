// File       : CodeGenerator.cc
// Description: Implementation of COOL code generation.

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "ClassTree.h"
#include "CodeGenerator.h"
#include "SymbolTable.h"
#include "NameGenerator.h"

using namespace std;

int spaces_per_tab = 4; // Used to keep track of line length.

// FUNCTION: Constructor.
CodeGenerator::CodeGenerator(int num_classes, string word_corpus)
    : output_file("output.cl")
    , writer(output_file)
    , class_name_length(10)
    , class_attribute_length(5)
    , class_method_length(5)
    , class_method_arg_length(5)
    , class_variable_length(10)
    , name_generator(word_corpus, class_name_length, class_attribute_length, class_method_length,
    class_method_arg_length, class_variable_length)
    , num_attributes_per_class(3)
    , num_methods_per_class(3)
    , max_num_method_args(5)
    , probability_repeat_method_name(0.2)
    , tree(name_generator, num_classes, num_attributes_per_class, num_methods_per_class,
      max_num_method_args, probability_repeat_method_name) {

  // Internal configuration.
  this->max_recursion_depth = 5;
  this->probability_initialized = 0.75;
  this->max_block_length = 5;
  this->max_let_defines = 4;
  this->max_case_branches = 7;
  this->max_line_length = 80;
  this->current_line_length = 0;
  this->max_expression_count = 1000000000; // 1 billion.

  // Initialization of internal state.
  this->indentation_tabs = 0;
  this->recursive_depth = 0;
  this->expression_count = 0;
  this->identifiers = SymbolTable();

  // Initialize the class tree.
  tree.generate_class_information();

  // Create map from expansion name -> expansion weight.
  vector<float> expression_weights = vector<float>(NUM_EXPRESSION_TYPES, 1.0);
  this->expression_map = map<ExpansionType, float>();
  for (int i = ExpansionType::New; i < NUM_EXPRESSION_TYPES; i++) {
    this->expression_map[static_cast<ExpansionType>(i)] = expression_weights[i];
  }
}

// FUNCTION: Generates an expansion of the given name. This
//  is different from generate_expression in the sense that the input
//  to this function is a type of expression expansion. For
//  example, "dispatch" is a type of expansion, and it may
//  evaluate to an expression of type "Int".
void CodeGenerator::generate_expansion(ExpansionType expansion, string expression_type) {
  if (expansion == New) {
    generate_new(expression_type);
  } else if (expansion == Bool) {
    generate_bool();
  } else if (expansion == String) {
    generate_string();
  } else if (expansion == Int) {
    generate_int();
  } else if (expansion == Identifier) {
    generate_identifier(expression_type, false);
  } else if (expansion == Assignment) {
    generate_assignment(expression_type, false);
  } else if (expansion == SelfDispatch) {
    write_dispatch("self");
  } else if (expansion == StaticDispatch) {
    write_dispatch("static");
  } else if (expansion == Dispatch) {
    write_dispatch("regular");
  } else if (expansion == Conditional) {
    generate_conditional(expression_type);
  } else if (expansion == Loop) {
    generate_loop();
  } else if (expansion == Block) {
    generate_block(expression_type);
  } else if (expansion == IsVoid) {
    generate_isvoid();
  } else if (expansion == Arithmetic) {
    generate_arithmetic();
  } else if (expansion == Comparison) {
    generate_comparison();
  } else if (expansion == IntComplement) {
    generate_int_complement();
  } else if (expansion == BoolComplement) {
    generate_bool_complement();
  } else if (expansion == Let) {
    generate_let(expression_type);
  } else if (expansion == Case) {
    generate_case(expression_type);
  } else {
    throw "Internal error: chosen expression type not a possible expansion.";
  }
}

// FUNCTION: This is a fairly tricky function. We describe
//  the parameters separately and in detail below:
//
//  [String] possible_expansions:
//          This should be passed in as an empty vector and
//          passed by reference. At the end, it will be filled
//          with the names of possible valid expansions, given
//          the fact that the resulting expression must satisfy
//          expression_type, and also stay within the recursion
//          depth limit and max number of expressions.
//  [Float] probability_cutoffs:
//          This should be passed in as an empty vector and
//          passed by reference. As we populate the possible_expansions
//          vector, we will also populate probability cutoffs
//          with the float weights that are pulled from expression_map
//          (this is configured when the class is created and denotes
//          the amount of weight each expansion should have -- for
//          instance, if you set "new" to zero then you will get no
//          "new" expansions).
//  String expression_type:
//          A string representing the class we are trying to generate. This
//          means that for the expansion to be valid, there must be an expansion
//          of the given type that will generate a static type <= expression_type.
//
//  Returns:
//       We return the sum of the entries in probability cutoffs. This can be used
//       for normalization after.
//
//  Example:
//        Suppose there are four expansion types: ["new", "constant", "assign", "dispatch"].
//        The user has configured the corresponding weights: [1.5, 0.5, 1.2, 12.3].
//        It turns out, that only the first three are valid if we are trying to generate
//        an "Int" (that is, we cannot generate an expression <= Int using a dispatch). Then,
//        we might end up with:
//
//        possible_expansions = ["new", "assign", "constant"]
//        probability_cutoffs = [1.5, 1.2, 0.5]
//        return value        = 3.2
float CodeGenerator::populate_possible_expansions(vector<ExpansionType>& possible_expansions,
      vector<float>& probability_cutoffs, string expression_type) {

  // New.
  float normalization_factor = expression_map[New];
  possible_expansions.push_back(New);
  probability_cutoffs.push_back(expression_map[New]);

  // Bool constants.
  if (tree.is_child_of("Bool", expression_type)) {
    normalization_factor += expression_map[Bool];
    possible_expansions.push_back(Bool);
    probability_cutoffs.push_back(expression_map[Bool]);
  }

  // String constants.
  if (tree.is_child_of("String", expression_type)) {
    normalization_factor += expression_map[String];
    possible_expansions.push_back(String);
    probability_cutoffs.push_back(expression_map[String]);
  }

  // Int constants.
  if (tree.is_child_of("Int", expression_type)) {
    normalization_factor += expression_map[Int];
    possible_expansions.push_back(Int);
    probability_cutoffs.push_back(expression_map[Int]);
  }

  // Identifiers.
  if (generate_identifier(expression_type, true)) {
    normalization_factor += expression_map[Identifier];
    possible_expansions.push_back(Identifier);
    probability_cutoffs.push_back(expression_map[Identifier]);
  }

  // Assignment.
  if (generate_assignment(expression_type, true) && recursive_depth < max_recursion_depth
                                                  && expression_count < max_expression_count) {
    normalization_factor += expression_map[Assignment];
    possible_expansions.push_back(Assignment);
    probability_cutoffs.push_back(expression_map[Assignment]);
  }

  // Dispatch.
  generate_dispatch_structures(expression_type);
  if (recursive_depth < max_recursion_depth && expression_count < max_expression_count) {
    if (self_dispatches.size() != 0) {
      normalization_factor += expression_map[SelfDispatch];
      possible_expansions.push_back(SelfDispatch);
      probability_cutoffs.push_back(expression_map[SelfDispatch]);
    }
    if (static_dispatches.size() != 0) {
      normalization_factor += expression_map[StaticDispatch];
      possible_expansions.push_back(StaticDispatch);
      probability_cutoffs.push_back(expression_map[StaticDispatch]);
    }
    if (dispatches.size() != 0) {
      normalization_factor += expression_map[Dispatch];
      possible_expansions.push_back(Dispatch);
      probability_cutoffs.push_back(expression_map[Dispatch]);
    }
  }

  // Conditional.
  if (recursive_depth < max_recursion_depth && expression_count < max_expression_count) {
    normalization_factor += expression_map[Conditional];
    possible_expansions.push_back(Conditional);
    probability_cutoffs.push_back(expression_map[Conditional]);
  }

  // Loop.
  if (expression_type == "Object" && recursive_depth < max_recursion_depth
                                    && expression_count < max_expression_count) {
    normalization_factor += expression_map[Loop];
    possible_expansions.push_back(Loop);
    probability_cutoffs.push_back(expression_map[Loop]);
  }

  // Block.
  if (recursive_depth < max_recursion_depth && expression_count < max_expression_count) {
    normalization_factor += expression_map[Block];
    possible_expansions.push_back(Block);
    probability_cutoffs.push_back(expression_map[Block]);
  }

  // isVoid.
  if (tree.is_child_of("Bool", expression_type) && recursive_depth < max_recursion_depth
                                            && expression_count < max_expression_count) {
    normalization_factor += expression_map[IsVoid];
    possible_expansions.push_back(IsVoid);
    probability_cutoffs.push_back(expression_map[IsVoid]);
  }

  // Arithmetic.
  if (tree.is_child_of("Int", expression_type) && recursive_depth < max_recursion_depth
                                            && expression_count < max_expression_count) {
    normalization_factor += expression_map[Arithmetic];
    possible_expansions.push_back(Arithmetic);
    probability_cutoffs.push_back(expression_map[Arithmetic]);
  }

  // Comparison.
  if (tree.is_child_of("Bool", expression_type) && recursive_depth < max_recursion_depth
                                            && expression_count < max_expression_count) {
    normalization_factor += expression_map[Comparison];
    possible_expansions.push_back(Comparison);
    probability_cutoffs.push_back(expression_map[Comparison]);
  }

  // Integer complement.
  if (tree.is_child_of("Int", expression_type) && recursive_depth < max_recursion_depth
                                            && expression_count < max_expression_count) {
    normalization_factor += expression_map[IntComplement];
    possible_expansions.push_back(IntComplement);
    probability_cutoffs.push_back(expression_map[IntComplement]);
  }

  // Boolean complement.
  if (tree.is_child_of("Bool", expression_type) && recursive_depth < max_recursion_depth
                                            && expression_count < max_expression_count) {
    normalization_factor += expression_map[BoolComplement];
    possible_expansions.push_back(BoolComplement);
    probability_cutoffs.push_back(expression_map[BoolComplement]);
  }

  // Let statement.
  if (recursive_depth < max_recursion_depth && expression_count < max_expression_count) {
    normalization_factor += expression_map[Let];
    possible_expansions.push_back(Let);
    probability_cutoffs.push_back(expression_map[Let]);
  }

  // Case.
  if (recursive_depth < max_recursion_depth && expression_count < max_expression_count) {
    normalization_factor += expression_map[Case];
    possible_expansions.push_back(Case);
    probability_cutoffs.push_back(expression_map[Case]);
  }

  return normalization_factor;
}

// FUNCTION: Generates an expression of the given type.
void CodeGenerator::generate_expression(string expression_type) {

  // Increase recursive depth.
  recursive_depth++;

  // Increase expression count.
  expression_count++;

  // Compute possible expansions and keep track of weights.
  vector<ExpansionType> possible_expansions = vector<ExpansionType>();
  vector<float> probability_cutoffs = vector<float>();
  float normalization_factor = populate_possible_expansions(possible_expansions,
                                                            probability_cutoffs,
                                                            expression_type);

  // Choose expansion.
  float probability_sum = 0.0;
  for (int i = 0; i < probability_cutoffs.size(); i++) {
    probability_sum += probability_cutoffs[i];
    probability_cutoffs[i] = probability_sum;
  }
  transform(probability_cutoffs.begin(), probability_cutoffs.end(), probability_cutoffs.begin(),
                                 [normalization_factor](float i){return i / normalization_factor;});
  double probability_cutoff = ((double) rand() / (RAND_MAX));
  int expansion_index = 0;
  for (; expansion_index < probability_cutoffs.size(); expansion_index++) {
    if (probability_cutoff < probability_cutoffs[expansion_index]) break;
  }
  ExpansionType expansion = possible_expansions[expansion_index];

  // Generate code corresponding to chosen expansion.
  generate_expansion(expansion, expression_type);

  // Reduce recursive depth.
  recursive_depth--;
}

// FUNCTION: Prints the number of tabs indicated by global indentation_tabs.
// NOTES: - This should only be used on a new line. Otherwise, current_line_length
//          will be incorrect.
void CodeGenerator::print_tabs() {
  current_line_length = indentation_tabs * spaces_per_tab;
  writer << string(indentation_tabs, '\t');
}

// FUNCTION: Prints one attribute.
void CodeGenerator::print_attribute(string class_name, string attribute_name, string attribute_type) {
  print_tabs();
  indentation_tabs++;

  writer << attribute_name << ": " << attribute_type;
  current_line_length += attribute_name.length() + attribute_type.length() + 2;

  // Generate initialization based on initialization probability.
  double cutoff = ((double) rand() / (RAND_MAX));
  if (cutoff >= probability_initialized) {
    writer << ";" << endl;
  } else {
    writer << " <- (";
    current_line_length += 5;
    generate_expression(attribute_type);
    writer << ");" << endl;
  }

  indentation_tabs--;
}

// FUNCTION: Prints one method.
// NOTES: Handles updating the identifiers vector with
//        all the arguments in one method.
void CodeGenerator::print_method(string class_name, string method_name, string method_type) {

  // Update identifiers.
  identifiers.enter_scope();
  map<string, vector<pair<string, string> > > class_method_args = tree.class_method_args[class_name];
  vector<pair<string, string> > method_args = class_method_args[method_name];
  for (int i = 0; i < method_args.size(); i++) {
    identifiers.add_id(method_args[i].first, method_args[i].second);
  }

  // Tabs + method name.
  print_tabs();
  writer << method_name << "(";

  // Print arguments.
  vector<pair<string, string> > args = tree.class_method_args[class_name][method_name];
  for (int i = 0; i < args.size(); i++) {
    if (i == args.size() - 1) {
      writer << args[i].first << ": " << args[i].second;
    } else {
      writer << args[i].first << ": " << args[i].second << ", ";
    }
  }

  // Print return type.
  writer << ") : " << method_type << " {" << endl;
  indentation_tabs++;

  // Generate body.
  print_tabs();
  generate_expression(method_type);
  writer << endl;

  // End method declaration.
  indentation_tabs--;
  print_tabs();
  writer << "};" << endl;

  // Remove arguments from identifiers.
  identifiers.exit_scope();
}

// FUNCTION: Prints one class.
// NOTES: - updates identifiers vectors with attributes + self.
//        - updates current_class as well.
void CodeGenerator::print_class(string class_name) {

  current_class = class_name;

  // Update identifiers vector with local variables.
  vector<string> attribute_holders = tree.class_ancestors[class_name];
  attribute_holders.push_back(class_name);
  for (int i = 0; i < attribute_holders.size(); i++) {
    vector<pair<string, string> > current_attribute_pairs = tree.class_attributes[attribute_holders[i]];
    for (int j = 0; j < current_attribute_pairs.size(); j++) {
      identifiers.add_id(current_attribute_pairs[j].first, current_attribute_pairs[j].second);
    }
  }
  identifiers.add_id("self", class_name);

  // Print class declaration line.
  string parent = tree.class_ancestors[class_name][0];
  print_tabs();
  if (parent == "Object") {
    writer << "class " << class_name << " {" << endl;
  } else {
    writer << "class " << class_name << " inherits " << parent << " {" << endl;
  }
  indentation_tabs++;

  // Print attributes.
  for (int i = 0; i < tree.class_attributes[class_name].size(); i++) {
    string attribute_name = tree.class_attributes[class_name][i].first;
    string attribute_type = tree.class_attributes[class_name][i].second;
    print_attribute(class_name, attribute_name, attribute_type);
  }

  // One line between methods and attributes.
  writer << endl;

  // Print methods.
  for (int i = 0; i < tree.class_method_names[class_name].size(); i++) {
    string method_name = tree.class_method_names[class_name][i];
    string method_type = tree.class_method_types[class_name][method_name];
    print_method(class_name, method_name, method_type);
  }

  // Print class end.
  indentation_tabs--;
  print_tabs();
  writer << "};" << endl << endl;

  // Reset identifiers vector.
  identifiers.exit_scope();
}

// FUNCTION: Main function that generates the output code file.
void CodeGenerator::generate_code() {

  for (int i = 0; i < tree.class_names.size(); i++) {
    string class_name = tree.class_names[i];
    if (class_name == "Object" || class_name == "Bool" ||
        class_name == "String" || class_name == "Int" ||
        class_name == "IO") continue;
    print_class(tree.class_names[i]);

    if (i % 10 == 0 && i > 0) cout << i << " classes generated." << endl;
  }
}
