#include "string.h"

int strcmp(const char* str1, const char* str2){
	while(*str1 == *str2){
		if(*str1 == '\0')
			return 0;

		str1++;
		str2++;
	}
	return *str1 - *str2;
}


char* strncpy(char* dest, const char* src, int len)
{
	char* temp=dest;
	
	int i=0;
	while(i < len  && (*temp = *src)!='\0')
	{
		i++;
		temp++;
		src++;
	}

	if(*(temp)!='\0')
		*temp='\0';

	return dest;
}
