// File         : Util.cc
// Description  : Variety of useful functions.

#include <string>
#include <stdlib.h>
#include <vector>
#include <cctype>

using namespace std;


// Characters for variable, attribute, and method names.
static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";

// Disallowed class keywords.
string class_keywords[] = {"class", "else", "fi", "if", "in",
                            "inherits", "isvoid", "let", "loop", "pool",
                            "then", "while", "case", "esac", "new",
                            "of", "not", "object", "io", "int",
                            "string", "bool", "main", "self_type"};
vector<string> class_keyword_vec (class_keywords, class_keywords + 24);

// Disallowed attribute keywords.
string attribute_keywords[] = {"class", "else", "fi", "if", "in",
                                "inherits", "isvoid", "let", "loop", "pool",
                                "then", "while", "case", "esac", "new",
                                "of", "not", "true", "false", "self"};
vector<string> attribute_keyword_vec (attribute_keywords, attribute_keywords + 20);


// True if words are equal ignoring case.
bool compare_case_insensitive(string a, string b) {
  if (a.length() != b.length()) return false;
  for (int i = 0; i < a.length(); i++) {
    if (tolower(a[i]) != tolower(b[i])) return false;
  }
  return true;
}

// True if @str is contained in @word_vector ignoring case.
bool string_vector_contains(string str, vector<string> word_vector) {
  for(int i = 0; i < word_vector.size(); i++) {
    if (compare_case_insensitive(word_vector[i], str)) {
      return true;
    }
  }
  return false;
}

// Generates a valid COOL class name of length @length
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

// Generates a valid COOL class attribute of length @length
// that is not inside the vector @illegal_words.
// NOTE: This ignores the case of the strings in @illegal_words.
string generate_attribute_name(int length, vector<string> illegal_words) {
  if (length <= 0) throw "Nonpositive name length";
  string attribute_name = "";

  while (true) {
    // Generate first character lowercase.
    char first = alphanum[(rand() % 26) + 26];
    attribute_name += first;

    // Generate rest of class name.
    for (int i = 0; i < length - 1; i++) {
      char c = alphanum[rand() % 63];
      attribute_name += c;
    }

    // Exit if class name is not a keyword and not in list of illegal words.
    if (!(string_vector_contains(attribute_name, attribute_keyword_vec) &&
                  string_vector_contains(attribute_name, illegal_words))) break;
  }

  return attribute_name;
}
