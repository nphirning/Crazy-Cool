// File         : Util.cc
// Description  : Variety of useful functions.

#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <cctype>

using namespace std;

// Cached corpus information.
bool is_corpus_cached = false;
vector<string> corpus = vector<string>();


// Characters for variable, attribute, and method names.
static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";

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


// FUNCTION: Returns true if words are equal ignoring case.
bool compare_case_insensitive(string a, string b) {
  if (a.length() != b.length()) return false;
  for (int i = 0; i < a.length(); i++) {
    if (tolower(a[i]) != tolower(b[i])) return false;
  }
  return true;
}

// FUNCTION: Returns true if @str is contained in @word_vector ignoring case.
bool string_vector_contains(string str, vector<string> word_vector) {
  for(int i = 0; i < word_vector.size(); i++) {
    if (compare_case_insensitive(word_vector[i], str)) {
      return true;
    }
  }
  return false;
}

// FUNCTION: Generates a random alphanumeric string.
string generate_random_string(int length) {
  string str = "";
  for(int i = 0; i < length; i++) {
    str += alphanum[rand() % 63];
  }
  return str;
}

// FUNCTION: Generates a valid COOL class name of length @length
// that is not inside the vector @illegal_words.
// NOTE: This ignores the case of the strings in @illegal_words.
// NOTE: This will not return a class with the name "Main".
string generate_class_name(int length, vector<string> illegal_words) {
  if (length <= 0) throw "Nonpositive name length";
  string class_name = "";

  while (true) {
    // Generate first character uppercase.
    char first = alphanum[rand() % 26];
    class_name += first;

    // Generate rest of class name.
    for (int i = 0; i < length - 1; i++) {
      char c = alphanum[rand() % 63];
      class_name += c;
    }

    // Exit if class name is not a keyword and not in list of illegal words.
    if (!(string_vector_contains(class_name, class_keyword_vec) &&
                  string_vector_contains(class_name, illegal_words))) break;
  }

  return class_name;
}

// FUNCTION: Generates a valid COOL class feature of length @length
// that is not inside the vector @illegal_words.
// NOTE: This ignores the case of the strings in @illegal_words.
string generate_feature_name(int length, vector<string> illegal_words) {
  if (length <= 0) throw "Nonpositive name length";
  string feature_name = "";

  while (true) {
    // Generate first character lowercase.
    char first = alphanum[(rand() % 26) + 26];
    feature_name += first;

    // Generate rest of class name.
    for (int i = 0; i < length - 1; i++) {
      char c = alphanum[rand() % 63];
      feature_name += c;
    }

    // Exit if class name is not a keyword and not in list of illegal words.
    if (!(string_vector_contains(feature_name, feature_keyword_vec) &&
                  string_vector_contains(feature_name, illegal_words))) break;
  }

  return feature_name;
}

// FUNCTION: Returns the absolute path to the current working directory
// NOTE: Doesn't include the trailing slash.
string get_current_working_directory() {
  long size;
  char* buf;
  char* ptr;
  size = pathconf(".", _PC_PATH_MAX);
  if ((buf = (char *)malloc((size_t)size)) != NULL) {
    ptr = getcwd(buf, (size_t)size);
  }

  // Convert char* to string.
  if (ptr == NULL) {
    return "";
  }
  return string(ptr);
}

// FUNCTION: Caches the corpus at @corpus_path.
void cache_corpus(string corpus_path) {
  ifstream file(corpus_path);
  string word;
  while(file >> word) {
    corpus.push_back(word);
  }
}

// FUNCTION: Extracts a class name from the corpus.
// NOTE: See generate_class_name for specifics.
string extract_class_name(string corpus_path, vector<string> illegal_words) {

  if (!is_corpus_cached) {
    cache_corpus(corpus_path);
  }

  string class_name = "";

  while(true) {
    class_name = corpus[rand() % corpus.size()];
    if (class_name.length() == 0) continue;

    // Change capitalization.
    class_name[0] = toupper(class_name[0]);
    for (int i = 1; i < class_name.length(); i++) {
      class_name[i] = tolower(class_name[i]);
    }

    // Exit if class name is not a keyword and not in list of illegal words.
    if (!(string_vector_contains(class_name, class_keyword_vec) &&
                  string_vector_contains(class_name, illegal_words))) break;
  }

  return class_name;
}

string extract_feature_name(string corpus_path, vector<string> illegal_words) {
  if (!is_corpus_cached) {
    cache_corpus(corpus_path);
  }

  string feature_name = "";

  while(true) {
    feature_name = corpus[rand() % corpus.size()];
    if (feature_name.length() == 0) continue;

    // Change capitalization.
    for (int i = 0; i < feature_name.length(); i++) {
      feature_name[i] = tolower(feature_name[i]);
    }

    // Exit if class name is not a keyword and not in list of illegal words.
    if (!(string_vector_contains(feature_name, feature_keyword_vec) &&
                  string_vector_contains(feature_name, illegal_words))) break;
  }

  return feature_name;
}
