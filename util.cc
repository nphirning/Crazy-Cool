// File         : Util.cc
// Description  : Variety of useful functions.

#include <string>
#include <stdlib.h>

// Set random seed.
srand(time(NULL))

// Characters for variable, attribute, and method names.
static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";

// Generates a valid COOL class name of length @len.
string generate_class_name(int len) {
  if (len <= 0) throw ClassNameLengthException("Nonpositive name length");
  string class_name = "";

  // Generate first character uppercase.
  char first = alphanum[rand() % 26];
  name += first;

  // Generate rest of class name.
  for (int i = 0; i < len - 1; i++) {
    char c = alphanum[rand() % 63];
    name += c;
  }

  return name;
}
