// File         : NameGenerator.h
// Description  : Header file for the NameGenerator class, which
//                allows generation of COOL names.

#ifndef NAMEGENERATOR_H_
#define NAMEGENERATOR_H_

#include <string>
#include <vector>
#include <stdlib.h>

// ENUM NameType
// -------------
// Enumerates the different types of "names" that we
// need to generate in COOL code. These are the names
// of classes, class attributes, class methods, and
// class method arguments.
enum NameType { className, attribute, method, methodArgument, variable};


// CLASS NameGenerator
// -------------------
// This class is intended to generate names.
//
// The general idea is that you wil construct this class
// and then call generate on the instance to return a string
// that fits the COOL guidelines and will also not be contained
// in the list of illegal names that you provided.
//
// There are two main options for construction of this class:
//
// 1. Using randomly generated names. In this case, when
//    constructing the NameGenerator, you should supply
//    an empty string for the parameter corpus_path. Then
//    when generate is called, a random string of alphanumeric
//    characters with the length you supplied will be generated.
// 2. Using a corpus. If you supply the absolute or relative path
//    to a file where each separate line contains a word, then the
//    names will be drawn from that corpus. They will be capitalized
//    correctly, but otherwise lowercase. As a note, I will check
//    that words in the corpus contain only alphanumeric characters,
//    numbers, or underscores and that the first letter is always a
//    letter. If this is not the case, an exception will be thrown.
//
// NOTES: - An exception will also be thrown if we loop generate/drawn
//          more than 100 words from the corpus without finding one that
//          is both not a disallowed keyword and not inside the list of
//          illegal words. If this occurs, consider either making your
//          corpus bigger or using a larger length.
//
//        - This does not generate any of the basic classes (Object, IO,
//          Int, String, Bool) and it won't generate the Main class. However,
//          we do allow generation of the method "main". This is just for
//          convenience, and you can easily pass "main" into the illegal_names
//          if you don't want it.
class NameGenerator {
public:

  // FUNCTION: Constructor.
  // ----------------------
  // Parameters:
  //        String corpus_path
  //              The relative or absolute path to the
  //              corpus from which we should pull COOL
  //              names. If no corpus should be used, pass
  //              the empty string.
  //        Int class_name_length
  //              The length of class names in the case
  //              that a corpus is NOT used.
  //        Int attribute_name_length
  //              The length of attribute names in the case
  //              that a corpus is NOT used.
  //        Int method_name_length
  //              The length of method names in the case
  //              that a corpus is NOT used.
  //        Int method_arg_name_length
  //              The length of method argument names in
  //              the case that a corpus is NOT used.
  NameGenerator(std::string corpus_path,
                int class_name_length,
                int attribute_name_length,
                int method_name_length,
                int method_arg_name_length,
                int variable_name_length);

  // FUNCTION generate.
  // ------------------
  // Generates a name.
  //
  // Parameters:
  //        NameType type
  //              The type of the name to generate. The
  //              options are given in the enum NameType
  //              defined above.
  //        [String] illegal_names
  //              List of illegal_names that we cannot return.
  //              For example, if you're generating class names,
  //              fill this with other class names that the NameGenerator
  //              instance is not aware of so as to ensure
  //              none of those names are chosen/generated.
  // Returns:
  //        A string representing the generated/chosen name.
  std::string generate(NameType type, std::vector<std::string> illegal_names) const;

  // FUNCTION generate_random_string.
  // --------------------------------
  // Generates a random string of valid COOL characters.
  //
  // Parameters:
  //      Int length
  //            The length of the string to generate. Must be > 0.
  // Returns:
  //      The randomly generated string.
  std::string generate_random_string(int length);

private:

  // Internal functions.
  bool validate_name(std::string name);
  std::string generate_random_class_name(int len, std::vector<std::string> illegal_words = std::vector<std::string>()) const;
  std::string generate_random_feature_name(int len, std::vector<std::string> illegal_words = std::vector<std::string>()) const;
  std::string extract_class_name(std::vector<std::string> illegal_words) const;
  std::string extract_feature_name(std::vector<std::string> illegal_words) const;

  // Corpus handling.
  void cache_corpus();
  std::string corpus_path;
  std::vector<std::string> corpus = std::vector<std::string>();

  // Name lengths.
  int class_name_length;
  int attribute_name_length;
  int method_name_length;
  int method_arg_name_length;
  int variable_name_length;
};

#endif
