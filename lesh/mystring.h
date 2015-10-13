#ifndef _MYSTRING_H
#define _MYSTRING_H
#include "read_command.h"
#define STR_MAX_LENGTH 40
#define STR_NPOS 666666

extern char *charcat(char *str, char ch);
extern state mysinput(char *str, char stopchar);
extern void strclear(char * str);
extern char *substr(char const*str,int pos, int length);
extern void sreplace(char *str,int pos,int length,char *toreplaces);
extern int  sfind(char const *str,char ch,int pos);

#endif
