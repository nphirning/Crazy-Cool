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
