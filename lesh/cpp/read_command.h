#ifndef _READ_COMMAND_H
#define _READ_COMMAND_H
#include <string>
#include <map>
#include <iostream>
#include <limits.h>
#define MAX_ARG 10
using namespace std;


extern string Parameters[MAX_ARG];
extern int para_length;
extern map<string , string > mapstring;
enum state {syntax_error, excute_success};


extern int read_recommand();
extern state outer_command();
extern state set();
extern state echo();
extern state var_extention(string &str);
extern state var_replace(int &left_pos,int &right_pos,int &ext_pos,string &str);
extern state var_cat(string &str);
//extern state cat_analyze(int &left_quo,int &right_quo,string &str);
extern state var_substitute(string &str);
#endif
