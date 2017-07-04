// File         : util.h
// Description  : Declaration of a variety of useful functions.

#include <string>
#include <stdlib.h>

using namespace std;

string generate_random_string(int len);
string generate_class_name(int len, vector<string> illegal_words = vector<string>());
string generate_feature_name(int len, vector<string> illegal_words = vector<string>());
string get_current_working_directory();
void cache_corpus(string corpus_path);
string extract_class_name(string corpus_path, vector<string> illegal_words);
string extract_feature_name(string corpus_path, vector<string> illegal_words);
