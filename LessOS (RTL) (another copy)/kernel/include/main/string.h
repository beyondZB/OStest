#ifndef	_STRING_H
#define _STRING_H

#include "type.h"
#include "const.h"

PUBLIC void* memcpy(void* p_dst, void* p_src, int size);
PUBLIC void memset(void* p_dst, char ch, int size);
PUBLIC char* strcpy(char* p_dst, char* p_src);
PUBLIC int strcmp(const char* str1, const char* str2);
PUBLIC char* strncpy(char* dest, const char* src, int len);


#endif	//_STRING_H
