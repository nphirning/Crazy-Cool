// File         : NameGenerator.cc
// Description  : The implementation of the NameGenerator class. 

#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <cctype>
#include "util.h"
#include "NameGenerator.h"

#define NUM_VALID_CHARACTERS 63

using namespace std;

// Convenient list of alphanumeric characters (allowed in COOL) for random generation.
// NOTE: We keep the alphabetic characters at the front so we can iterate over the alphabet easily.
static const char valid_characters[NUM_VALID_CHARACTERS + 1] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";

// Disallowed class keywords.
string class_keywords[] = {"class", "else", "fi", "if", "in",
                            "inherits", "isvoid", "let", "loop", "pool",
                            "then", "while", "case", "esac", "new",
                            "of", "not", "object", "io", "int",
                            "string", "bool", "main", "self_type"};
vector<string> class_keyword_vec (class_keywords, class_keywords + 24);

// Disallowed feature keywords.
string feature_keywords[] = {"class", "else", "fi", "if", "in",
                                "inherits", "isvoid", "let", "loop", "pool",
                                "then", "while", "case", "esac", "new",
                                "of", "not", "true", "false", "self"};
vector<string> feature_keyword_vec (feature_keywords, feature_keywords + 20);

// FUNCTION: Constructor. Also caches the corpus.
NameGenerator::NameGenerator( string corpus_path,
                              int class_name_length,
                              int attribute_name_length,
                              int method_name_length, 
                              int method_arg_name_length) {
  
  // If we are supplied a corpus, parse
  // the absolute path and cache it.
  
  if (corpus_path != "") {
    // Store correct corpus path.
    if (corpus_path[0] == '/') {
      this->corpus_path = corpus_path;
    } else {
      this->corpus_path = get_current_working_directory() + '/' + corpus_path;
    }

    // Cache corpus.
    if (corpus_path.length() > 0) {
      cache_corpus();
    }
  }

  // Set name length configurations.
  this->class_name_length = class_name_length;
  this->attribute_name_length = attribute_name_length;
  this->method_name_length = method_name_length; 
  this->method_arg_name_length = method_arg_name_length;
}

// FUNCTION: Generates a COOL name that is not contained in @illegal_names.
// The supported types are given by the enum NameTypes defined in the header.
string NameGenerator::generate(NameType type, vector<string> illegal_names) const {

  // Corpus extraction.
  if (corpus_path.length() > 0) {
    if (type == className) {
      return extract_class_name(illegal_names);
    } 
    return extract_feature_name(illegal_names);
  }

  // Random generation.
  if (type == className) {
    return generate_random_class_name(class_name_length, illegal_names);
  } else if (type == attribute) {
    return generate_random_feature_name(attribute_name_length, illegal_names);
  } else if (type == method) {
    return generate_random_feature_name(method_name_length, illegal_names);
  }
  return generate_random_feature_name(method_arg_name_length, illegal_names);
}

// FUNCTION: Generates a random COOL class name of the desired length.
string NameGenerator::generate_random_class_name(int length, vector<string> illegal_words) const {
  if (length <= 0) throw "Nonpositive name length";
  string class_name = "";

  int iterations = 0;

  while (true) {
    // Generate first character uppercase.
    char first = valid_characters[rand() % 26];
    class_name += first;

    // Generate rest of class name.
    for (int i = 0; i < length - 1; i++) {
      char c = valid_characters[rand() % 63];
      class_name += c;
    }

    // Exit if class name is not a keyword and not in list of illegal words.
    if (!(string_vector_contains(class_name, class_keyword_vec) ||
                  string_vector_contains(class_name, illegal_words))) break;

    // Throw exception on max iterations limit.
    iterations++;
    if (iterations > 100) {
      throw "Reached max iteration limit while generating class name.";
    }
  }

  return class_name;
}

// FUNCTION: Generates a random COOL feature name of the desired length.
string NameGenerator::generate_random_feature_name(int length, vector<string> illegal_words) const {
  if (length <= 0) throw "Nonpositive name length";
  string feature_name = "";

  int iterations = 0;

  while (true) {
    // Generate first character lowercase.
    char first = valid_characters[(rand() % 26) + 26];
    feature_name += first;

    // Generate rest of class name.
    for (int i = 0; i < length - 1; i++) {
      char c = valid_characters[rand() % 63];
      feature_name += c;
    }

    // Exit if class name is not a keyword and not in list of illegal words.
    if (!(string_vector_contains(feature_name, feature_keyword_vec) ||
                  string_vector_contains(feature_name, illegal_words))) break;

    // Throw exception on max iterations limit.
    iterations++;
    if (iterations > 100) {
      throw "Reached max iteration limit while generating feature name.";
    }
  }

  return feature_name;
}

// FUNCTION: Caches the corpus into the corpus class member variable.
void NameGenerator::cache_corpus() {
  ifstream file(corpus_path);
  string word;
  while(file >> word) {
    corpus.push_back(word);
  }
}

// FUNCTION: Extracts a COOL class name from the corpus.
string NameGenerator::extract_class_name(vector<string> illegal_words) const {

  string class_name = "";
  int iterations = 0;

  while(true) {
    class_name = corpus[rand() % corpus.size()];
    if (class_name.length() == 0) continue;

    // Change capitalization.
    class_name[0] = toupper(class_name[0]);
    for (int i = 1; i < class_name.length(); i++) {
      class_name[i] = tolower(class_name[i]);
    }

    // Exit if class name is not a keyword and not in list of illegal words.
    if (!(string_vector_contains(class_name, class_keyword_vec) ||
                  string_vector_contains(class_name, illegal_words))) break;

    // Throw exception on max iterations limit.
    iterations++;
    if (iterations > 100) {
      throw "Reached max iteration limit while extracting class name. Expand your corpus!";
    }
  }

  return class_name;
}

// FUNCTION: Extracts a COOL feature name from the corpus.
string NameGenerator::extract_feature_name(vector<string> illegal_words) const {
  string feature_name = "";
  int iterations = 0;

  while(true) {
    feature_name = corpus[rand() % corpus.size()];
    if (feature_name.length() == 0) continue;

    // Change capitalization.
    for (int i = 0; i < feature_name.length(); i++) {
      feature_name[i] = tolower(feature_name[i]);
    }

    // Exit if class name is not a keyword and not in list of illegal words.
    if (!(string_vector_contains(feature_name, feature_keyword_vec) ||
                  string_vector_contains(feature_name, illegal_words))) break;

    // Throw exception on max iterations limit.
    iterations++;
    if (iterations > 100) {
      throw "Reached max iteration limit while extracting feature name. Expand your corpus!";
    }
  }

  return feature_name;
}

// FUNCTION: Validates that a name contains only alphanumeric characters + underscores.
bool NameGenerator::validate_name(string name) {
  for (int i = 0; i < name.length(); i++) {
    char character = name[i];
    bool flag = false;
    for (int j = 0; j < NUM_VALID_CHARACTERS; j++) {
      char valid_character = valid_characters[j];
      if (character == valid_character) {
        flag = true;
      }
    }
    if (!flag) return false;
  }
  return true;
}
