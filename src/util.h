// File         : util.h
// Description  : Declaration of a variety of useful functions.

#include <string>
#include <stdlib.h>

using namespace std;

string generate_random_string(int len);
string generate_class_name(int len, vector<string> illegal_words = vector<string>());
string generate_feature_name(int len, vector<string> illegal_words = vector<string>());
