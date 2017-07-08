// File         : NameGenerator.h
// Description  : Declaration of a variety of useful functions.

#ifndef NAMEGENERATOR_H_
#define NAMEGENERATOR_H_

#include <string>
#include <vector>
#include <stdlib.h>

enum NameType { className, attribute, method, methodArgument };

class NameGenerator {
public:

  NameGenerator(std::string corpus_path,
                int class_name_length,
                int attribute_name_length,
                int method_name_length, 
                int method_arg_name_length);

  std::string generate(NameType type, std::vector<std::string> illegal_names) const;

private:

  std::string generate_random_class_name(int len, std::vector<std::string> illegal_words = std::vector<std::string>()) const;
  std::string generate_random_feature_name(int len, std::vector<std::string> illegal_words = std::vector<std::string>()) const;
  std::string extract_class_name(std::vector<std::string> illegal_words) const;
  std::string extract_feature_name(std::vector<std::string> illegal_words) const;

  void cache_corpus();
  std::string corpus_path;
  std::vector<std::string> corpus = std::vector<std::string>();

  // Length of names.
  int class_name_length;
  int attribute_name_length;
  int method_name_length;
  int method_arg_name_length;
};

#endif
