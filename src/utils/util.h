// File         : util.h
// Description  : Declaration of a variety of useful functions.

#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

bool compare_case_insensitive(string a, string b);
bool string_vector_contains(string str, vector<string> word_vector);
string get_current_working_directory();
