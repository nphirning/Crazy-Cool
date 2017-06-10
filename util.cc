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
                      "then", "while", "case", "esac", "new", "of",
                      "not", "object", "io", "int", "string", "bool", "main"};
vector<string> class_keyword_vec (class_keywords, class_keywords + 22);

// True if words are equal ignoring case.
bool compare_case_insensitive(string a, string b) {
  if (a.length() != b.length()) return false;
  for (int i = 0; i < a.length(); i++) {
    if (tolower(a[i]) != tolower(b[i])) return false;
  }
  return true;
}

// True if word is a class keyword.
bool is_class_keyword(string str) {
  for(int i = 0; i < class_keyword_vec.size(); i++) {
    if (compare_case_insensitive(class_keyword_vec[i], str)) {
      return true;
    }
  }
  return false;
}

// Generates a valid COOL class name of length @len.
// NOTE: will not return a class with the name "Main".
string generate_class_name(int length) {
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

    // Exit if class name is not a keyword.
    if (!is_class_keyword(class_name)) break;
  }

  return class_name;
}
