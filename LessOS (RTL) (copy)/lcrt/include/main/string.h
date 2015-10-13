#ifndef	_STRING_H
#define _STRING_H

#include "type.h"

void* memcpy(void* p_dst, void* p_src, int size);
void memset(void* p_dst, char ch, int size);
char* strcpy(char* p_dst, char* p_src);
int strcmp(const char* str1, const char* str2);
char* strncpy(char* dest, const char* src, int len);


#endif	//_STRING_H
