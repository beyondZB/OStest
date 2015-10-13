#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include"read_command.h"
#include"mystring.h"

char * charcat( char * str, char ch)
{
	int len = strlen(str);
	if(len == STR_MAX_LENGTH-1)
	{
		puts("too long parament");
		return NULL;
	
	}
	str[len] = ch;
	str[len+1] = '\0';
	
	return str;

}

state mysinput(char *str, char stopchar)
{
	char temp;
	if((temp = getchar())== '\n')
	{
		puts("error:you should not use a command like this!");
		return FAILURE;	
	}
	ungetc(temp,stdin);

	while((temp = getchar())==' ');
	ungetc(temp,stdin);
			
	if((temp = getchar())== '\n')
	{
		puts("error:you should not use a command like this!");
		return FAILURE;	
	}
	ungetc(temp,stdin);
	
	do
	{
		if(stopchar!='\n')
		{
			if((temp = getchar())=='\n')
			{
				puts("error:you should not use a command like this!");
				return FAILURE;
			}
			ungetc(temp,stdin);
		}
		if((temp = getchar())==stopchar)break;
		if(temp == ' ')continue;
		if(charcat(str,temp)==NULL)
			return FAILURE;
	}while(TRUE);
	
	ungetc(temp,stdin);

	return SUCCESS;

}
void strclear(char *str)
{
	memset(str,'\0',(int)strlen(str));

}
char *substr(char const *str,int pos, int length)
{
	char *subs = (char*)malloc((length+1)*sizeof(char));
	strclear(subs);
	int index;
	int subindex;
	for(index = pos,subindex = 0;index < pos + length;subindex++,index++)
	{
		subs[subindex] = str[index];
	}
	return subs;
}
void sreplace(char *str,int pos, int length,char *toreplaces)
{
	char *temp = malloc((strlen(str)-pos-length+1)*sizeof(char));
	int index;
	int tindex;
	int len;
	
	len = strlen(str);
	for(tindex = 0,index = pos+length;index <= len;index++,tindex++)
		temp[tindex] = str[index];
	len = strlen(toreplaces);
	for(index = pos,tindex = 0;tindex < len;index++,tindex++)
		str[index] = toreplaces[tindex];
	len = strlen(temp);
	
	for(index,tindex = 0;tindex <= len;index++,tindex++)
		str[index] = temp[tindex];
	
	free(temp);
}
int sfind(char const *str ,char ch,int pos)
{
	int index;
	
	for(index = pos;index < strlen(str);index++)
	{
		if(str[index] == ch)
			return index;
	}
	return STR_NPOS;
}
void erase(char *str,int pos)
{
	int index;
	int len = (int)strlen(str);
	for(index = pos;index <= len;index++)
		str[index] = str[index+1];

}
