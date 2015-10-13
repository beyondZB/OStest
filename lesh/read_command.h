#ifndef _READ_COMMAND_H
#define _READ_COMMAND_H
#define MAX_ARG 10
#define MAX_PARA_LENGTH 40
#define TRUE 1
#include "map.h"

typedef enum state{SUCCESS,FAILURE}state;
typedef enum bool{false,true}bool;
extern int para_num;
extern char Parameters[MAX_ARG][MAX_PARA_LENGTH];
extern map mapstring;

extern state set();
extern state echo();
extern state outercommand();
extern state read_command();
extern state var_substitute(char *str);
extern state var_cat(char *str);
extern state var_extention(char *str);
#endif
