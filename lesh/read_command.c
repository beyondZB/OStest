#include "read_command.h"
#include "mystring.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
char Parameters[MAX_ARG][MAX_PARA_LENGTH];
int para_num;
map mapstring;

state read_command()
{
	para_num = 0;
	scanf("%40s",Parameters[0]);
	para_num++;
	if(!strcmp(Parameters[0], "set"))set();
	else if(!strcmp(Parameters[0], "echo"))echo();
	else outercommand();	
	int i;
	for(i = 0; i < para_num;i++)
		printf("the para %d is %s\n",i,Parameters[i]);
	
	int parc;
	for(parc = 0; parc < para_num; parc++)
		strclear(Parameters[parc]);
	fflush(NULL);
	return 	SUCCESS;
}
state echo()
{
	para_num++;
	if(mysinput(Parameters[1],'\n')==FAILURE)return FAILURE;	
	if(var_substitute(Parameters[1])== FAILURE)return FAILURE;
	var_cat(Parameters[1]);
	
	int quo_pos = 0;
	quo_pos = sfind(Parameters[1],'\"',quo_pos);
	erase(Parameters[1],quo_pos);
	quo_pos = sfind(Parameters[1],'\"',quo_pos);
	erase(Parameters[1],quo_pos);
	puts(Parameters[1]);
	return SUCCESS;
}
state outercommand()
{
	char temp;
	temp = getchar();	
	while(TRUE)
	{
		if(temp == '\n')break;	
		temp = getchar();
		if(temp==' ')continue;
		if(temp=='\n')break;
		if(temp=='\"')
		{
			do
			{
				
				charcat(Parameters[para_num],temp);
				temp = getchar();
				if(temp == '\n')
				{
					puts("erro:wrong parament");
					return FAILURE;
				}	
			}while(temp!='\"');
			charcat(Parameters[para_num],temp);
			para_num++;
		}	
		else
		{
			do
			{
				charcat(Parameters[para_num],temp);
				temp = getchar();
				if(temp == '\n')break;
				
			
			}while(temp!=' ');
			para_num++;
				
		}
	}
	int i;
	for(i = 1; i < para_num;i++)
		var_extention(Parameters[i]);
	int rtn;
    if ( fork() == 0 ) 
    {
		switch(para_num)
		{
			case 1:
					{
					
						execlp(Parameters[0],Parameters[0], NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0]);
       					exit( errno ); 
						break;
					};
			case 2:
					{
					
    					execlp(Parameters[0],Parameters[0],Parameters[1], NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0]);
       					exit( errno ); 
						break;
					};
			case 3:
					{
					
    					execlp(Parameters[0],Parameters[0],Parameters[1],Parameters[2], NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0]);
       					exit( errno ); 
						break;
					};
			case 4:
					{
					
    					execlp(Parameters[0],Parameters[0],Parameters[1],Parameters[2],Parameters[3], NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0]);
       					exit( errno ); 
						break;
					};
			case 5:
					{
					
    					execlp(Parameters[0],Parameters[0],Parameters[1],Parameters[2],Parameters[3],
									   	Parameters[4], NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0]);
       					exit( errno ); 
						break;
					};
			case 6:
					{
					
    					execlp(Parameters[0],Parameters[0],Parameters[1],Parameters[2],Parameters[3],
									   	Parameters[4], Parameters[5],NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0]);
       					exit( errno ); 
						break;
					};
			case 7:
					{
					
    					execlp(Parameters[0],Parameters[0],Parameters[1],Parameters[2],Parameters[3],
									   	Parameters[4], Parameters[5],Parameters[6],NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0]);
       					exit( errno ); 
						break;
					};
	   		case 8:
					{
					
    					execlp(Parameters[0],Parameters[0],Parameters[1],Parameters[2],Parameters[3],
									   	Parameters[4], Parameters[5],Parameters[6],Parameters[7],NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0]);
       					exit( errno ); 
						break;
					};
			case 9:
					{
					
    					execlp(Parameters[0],Parameters[0],Parameters[1],Parameters[2],Parameters[3],
									   	Parameters[4], Parameters[5],Parameters[6],Parameters[7],Parameters[8],NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0]);
       					exit( errno ); 
						break;
					};
			case 10:
					{
					
    					execlp(Parameters[0],Parameters[0],Parameters[1],Parameters[2],Parameters[3],
									   	Parameters[4], Parameters[5],Parameters[6],Parameters[7],Parameters[8],
										Parameters[9],NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0]);
       					exit( errno ); 
						break;
					};
		}

	}
	else
	{
		wait(&rtn);
	}	
}
state set()
{
	int parc;
	para_num++;
	if(mysinput(Parameters[1],'=')==FAILURE)return FAILURE;	
	
	para_num++;
	if(charcat(Parameters[2],getchar())==NULL)return FAILURE;

	if(strcmp(Parameters[2],"="))return FAILURE;
	
	para_num++;	
	if(mysinput(Parameters[3],'\n')==FAILURE)return FAILURE;				
	//查看参量
	if(var_substitute(Parameters[3])== FAILURE)return FAILURE;
	var_cat(Parameters[3]);	

	node *nod = find(&mapstring,Parameters[1]);
	if(nod == NULL)
		insert(&mapstring,Parameters[1],Parameters[3]);
	else
		strcpy(nod->value,Parameters[3]);	
	if(find(&mapstring,"b")!=NULL)
		printf("the value of b is %s\n",find(&mapstring,"b")->value);
	return SUCCESS;
}
state var_extention(char *str)
{
	if(str[0]=='$')
	{
		int ext_pos = 0;
		int left_pos = 0;
		int right_pos =0;
		char *s;
		left_pos = sfind(str,'{',ext_pos);
		if(left_pos == STR_NPOS)
		{
			puts("error:wrong var extention");
			return FAILURE;
		}
		else if((right_pos = sfind(str,'}',left_pos)) == STR_NPOS)
		{
				puts("error:wrong var extention");
				return FAILURE;
		}	
		else 
		{
			s = substr(str,ext_pos + 2,(right_pos - ext_pos - 2));
			node *nod = find(&mapstring,s);
			if(nod == NULL )
			{
				puts("error:not defined variable:");
				free(s);
				return FAILURE;
			}
			sreplace(str,ext_pos,strlen(s)+3,nod->value);
			

		}
			
		int quo_pos = 0;
		quo_pos = sfind(str,'\"',quo_pos);
		erase(str,quo_pos);
		quo_pos = sfind(str,'\"',quo_pos);
		erase(str,quo_pos);
		free(s);
	}
	return SUCCESS;

}
//直接用指针操作，找到对应的位置，取子串，
state var_substitute(char *str)
{
	int  pos = 0;
	int  left_quo = 0;
	int  right_quo = 0;
	int  plus_pos = 0;
	int loop_i;
	int count;

	char*s;
	while(TRUE)
	{
		left_quo = sfind(str,'\"',pos);
		plus_pos = sfind(str,'+',pos);
	
		if(plus_pos == STR_NPOS && left_quo == STR_NPOS)
		{
			s = substr(str,pos,strlen(str)-pos);
			int templength = strlen(s);	
			for(loop_i = 0;s[loop_i] == ' ';)
				erase(s,loop_i);
			count = strlen(s)-1;
			for(;s[count]==' ';count--);
			for(loop_i = count+1;s[loop_i]==' ';)
				erase(s,loop_i);
			if(s[0]=='\0')
			{
				puts("error:wrong parament");
				free(s);
				return FAILURE;
			}
			for(loop_i = 0; loop_i <= strlen(s);loop_i++)
				if(s[loop_i] == ' ')
				{
					puts("error:wrong parament");
					free(s);
					return FAILURE;
				
				}
			node* nod = find(&mapstring,s);
			if(nod == NULL)
			{
				puts("the variable is not defined");
				free(s);
				return FAILURE;
			}		
			sreplace(str,pos,templength,nod->value);	
		
			free(s);
			break;
		}
		else if(plus_pos != STR_NPOS && left_quo == STR_NPOS)
		{
			s = substr(str,pos, plus_pos-pos);	
			int templength = strlen(s);	
			for(loop_i = 0;s[loop_i] == ' ';)
				erase(s,loop_i);
			count = strlen(s)-1;
			for(;s[count]==' ';count--);
			for(loop_i = count+1;s[loop_i]==' ';)
				erase(s,loop_i);
			if(s[0]=='\0')
			{
				puts("error:wrong parament");
				free(s);
				return FAILURE;
			}
			node* nod = find(&mapstring,s);
			if(nod == NULL)
			{
				puts("the variable is not defined");
				free(s);
				return FAILURE;
			}		
			sreplace(str,pos,templength,nod->value);	
			plus_pos += strlen(nod->value) - templength;
			pos = plus_pos+1;
			free(s);
			if(pos == strlen(str))
			{
				puts("error:wrong parament");
				return FAILURE;
			}
			
			continue;	
		}
		else if(plus_pos == STR_NPOS && left_quo != STR_NPOS)
		{
			char *check_s = substr(str,pos,left_quo - pos);
			bool check =true;
			if(check_s[0]=='\0')
				check =true;
			else
			{
				for(loop_i = 0;loop_i < strlen(check_s);loop_i++)
					if(check_s[loop_i]!=' ')
						check = false;
			}
			if(!check)
			{
				puts("error:wrong parament");
				free(check_s);
				return FAILURE;
			}
			free(check_s);
			right_quo= sfind(str,'\"',left_quo+1);
			if(right_quo == STR_NPOS)
			{
				puts("error:right quotation not found");
				return FAILURE;
			}
			pos = right_quo+1;
			for(loop_i = pos;str[pos]==' ';)
				erase(str,loop_i);
			if(str[pos]!='\0')
			{
				puts("error:wrong parament");
				return FAILURE;
			}
			if(pos == strlen(str))break;
			continue;
		}	
	
		else
		{
			if(plus_pos < left_quo)
			{
				char * check_s = substr(str,plus_pos+1,left_quo-plus_pos-1);					
				bool check =true;
			//	printf("checks is %s\n",check_s);
				if(check_s[0]=='\0')
					check =true;
				else
				{
					printf("check string is %s\n",check_s);
					for(loop_i = 0;loop_i < strlen(check_s);loop_i++)
						if(check_s[loop_i]!=' ')
							check = false;
				}
				if(!check)
				{
					puts("error:wrong parament");
					free(check_s);
					return FAILURE;
				}
				free(check_s);

				s = substr(str,pos,plus_pos-pos);

				int templength = strlen(s);
				
				for(loop_i = 0;s[loop_i] == ' ';)
					erase(s,loop_i);
				count = strlen(s)-1;
				for(;s[count]==' ';count--);
				for(loop_i = count+1;s[loop_i]==' ';)
					erase(s,loop_i);
				if(s[0]=='\0')
				{
					puts("error:wrong parament");
					free(s);
					return FAILURE;
				}
				for(loop_i = 0;loop_i <= strlen(s);loop_i++)
					if(s[loop_i]==' ')
					{
						puts("error:wrong parament");
						free(s);
						return FAILURE;
					}
				
				node* nod = find(&mapstring,s);
				if(nod == NULL)
				{
					puts("the variable is not defined");
					free(s);
					return FAILURE;
				}		
				sreplace(str,pos,templength,nod->value);	
				plus_pos += strlen(nod->value) - templength;
				pos = plus_pos +1;
				free(s);
				
				if(pos == strlen(str))break;
				continue;	
			}
			else if(left_quo < plus_pos)
			{
				right_quo = sfind(str,'\"',left_quo+1);
				if(right_quo == STR_NPOS)
				{
					puts("error:right quotation not found");
					return FAILURE;
				}
				else if(plus_pos <right_quo)
				{
					pos = right_quo+1;
					continue;
				}
				else if(plus_pos > right_quo)
				{
				
					char *check_ls = substr(str,pos,left_quo - pos);
					bool checkl =true;
					if(check_ls[0]=='\0')
						checkl =true;
					else
					{
						for(loop_i = 0;loop_i < strlen(check_ls);loop_i++)
							if(check_ls[loop_i]!=' ')
								check_ls = false;
					}
					if(!checkl)
					{
						free(check_ls);
						puts("error:wrong parament");
						return FAILURE;
					}
					free(check_ls);
				
					char *check_rs = substr(str,pos,left_quo - pos);
					bool checkr =true;
					if(check_rs[0]=='\0')
						checkr =true;
					else
					{
						for(loop_i = 0;loop_i < strlen(check_rs);loop_i++)
							if(check_rs[loop_i]!=' ')
								checkr = false;
					}
					if(!checkr)
					{
						puts("error:wrong parament");
						free(check_rs);
						return FAILURE;
					}
					free(check_rs);
					pos = plus_pos +1;
					if(pos == strlen(str))break;
						continue;
					
				}
			}	
		
		
		}
	}
	return SUCCESS;

}

state var_cat(char *str)
{
	int left_quo = 0;
	int right_quo = 0;
	char *s = malloc(sizeof(char)*(2+strlen(str)));
	strclear(s);
	char *temp;
	charcat(s,'\"');
	left_quo = sfind(str,'\"',left_quo);
	while(TRUE)
	{
		right_quo = sfind(str,'\"',left_quo+1);
		temp = substr(str,left_quo+1,right_quo-left_quo-1);
		strcat(s,temp);
		left_quo = sfind(str,'\"',right_quo+1);	
		free(temp);
		if(left_quo == STR_NPOS)break;
								
	
	}
	charcat(s,'\"');
	strcpy(str,s);
	free(s);
	return SUCCESS;
}



