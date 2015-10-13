#ifndef _API_H_
#define _API_H_
#include "type.h"
#include "string.h"

void createProcess(const char* processName);
int exitProcess();
void  ttyOutputStr(const char* str);
int system_call(u32 eax, u32 edx,u32 esi, u32 edi, u32 ecx, u32 ebx);

void *malloc(unsigned int num_bytes);
void free(void* ptr);


#endif //_API_H_
