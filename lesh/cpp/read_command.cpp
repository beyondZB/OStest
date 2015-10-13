#include "read_command.h"
#include <string>
#include <map>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;


string Parameters[MAX_ARG];
int para_length = 0;
map<string , string > mapstring;


int read_recommand()
{

	
	para_length = 0;
	
	cin >> Parameters[para_length];
	para_length++;
	if(Parameters[0] == "set")set();
	else if(Parameters[0] == "echo")echo();
	else outer_command();	
	
	
	/*while(cin.peek()!='\n')
	{::
		cin >> Parameters[para_length];
		para_length++;
	}*/
	//cerr << "我想看看我输入了什么命令:";
	//for(int i = 0; i <para_length; i++ ) cerr << Parameters[i] <<" ";
	//执行之后命令清除
	//cerr << endl;
	for(int i = 0;i <para_length;i++)Parameters[i].clear();

	//这个地方一定要刷新
	cin.clear();
    cin.ignore(INT_MAX, '\n');


	return 0;
}
state outer_command()
{
//命令名我已经有了  只要 exec 一下就行了，接下来考虑
	string temp;
	while(cin.peek()!='\n')
	{
		cin >> Parameters[para_length];
		if(Parameters[para_length][0]=='\"')
		{
			while(true)
			{
				cin >>temp; 
				Parameters[para_length]+=" ";
				Parameters[para_length]+=temp;
				if(temp[temp.length()-1]=='\"')
						break;
				if(cin.peek()=='\n')
				{
					cerr << "wrong parament"<<endl;
					return syntax_error;
				}
			}
			
		}
		if(var_extention(Parameters[para_length])==syntax_error)return syntax_error;
		cerr << "para is " << Parameters[para_length]<<endl;
		para_length++;	
	}
	cerr <<"para length is " << para_length<<endl;
	int rtn;
    if ( fork() == 0 ) 
    {
		switch(para_length)
		{
			case 1:
					{
					
						execlp(Parameters[0].c_str(),Parameters[0].c_str(), NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			case 2:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(), NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			case 3:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(),Parameters[2].c_str(), NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			case 4:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(),Parameters[2].c_str(),Parameters[3].c_str(), NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			case 5:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(),Parameters[2].c_str(),Parameters[3].c_str(),
									   	Parameters[4].c_str(), NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			case 6:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(),Parameters[2].c_str(),Parameters[3].c_str(),
									   	Parameters[4].c_str(), Parameters[5].c_str(),NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			case 7:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(),Parameters[2].c_str(),Parameters[3].c_str(),
									   	Parameters[4].c_str(), Parameters[5].c_str(),Parameters[6].c_str(),NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
	   		case 8:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(),Parameters[2].c_str(),Parameters[3].c_str(),
									   	Parameters[4].c_str(), Parameters[5].c_str(),Parameters[6].c_str(),Parameters[7].c_str(),NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			case 9:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(),Parameters[2].c_str(),Parameters[3].c_str(),
									   	Parameters[4].c_str(), Parameters[5].c_str(),Parameters[6].c_str(),Parameters[7].c_str(),Parameters[8].c_str(),NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			case 10:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(),Parameters[2].c_str(),Parameters[3].c_str(),
									   	Parameters[4].c_str(), Parameters[5].c_str(),Parameters[6].c_str(),Parameters[7].c_str(),Parameters[8].c_str(),
										Parameters[9].c_str(),NULL );
   	   				 /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			
		
		}
		 /* 子进程程序 */
    //	execlp( "vim","vim","b.cpp", NULL );
   	    /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
       // perror( "vim" );
       // exit( errno ); 
	 }
    else 
    {
       /* 父进程程序*/
	   	wait ( &rtn );
		
 
    }
}

state echo()
{
	char temp;
	while(cin.peek()!='\n')
	{
		cin >> noskipws;
		cin.get(temp);
		Parameters[1] += temp;
	}
	for(int pos = 0; Parameters[1][pos] == ' ';)
		Parameters[1].erase(pos,1);
	para_length++;
	cin >>skipws;
	string str = Parameters[1];
	if(var_substitute(str)== syntax_error)return syntax_error;
	if(var_cat(str)==syntax_error)return syntax_error;

	int quo_pos = 0;
	quo_pos = str.find("\"",quo_pos);
	str.erase(quo_pos,1);
	quo_pos = str.find("\"",quo_pos);
	str.erase(quo_pos,1);
	cout << str<<endl;
	return excute_success;
}
state set()
{
	cin >> Parameters[1] >> Parameters[2];
	if(Parameters[2]!= "=")
	{
		cerr << "error:wrong command" << endl;
		return syntax_error;
	}
	para_length += 2;
	char temp;
	while(cin.peek()!='\n')
	{
		cin >> noskipws;
		cin.get(temp);
		Parameters[3] += temp;
	}

	for(int pos = 0; Parameters[3][pos] == ' ';)
			Parameters[3].erase(pos,1);
	para_length++;
	cin >>skipws;
	
	//para[2]必须是等号
	//if(var_extention(Parameters[3])== syntax_error)return syntax_error;
	//开始进行字符串的拼接
	//if(var_cat(Parameters[3])==syntax_error)return syntax_error;
	if(var_substitute(Parameters[3])== syntax_error)return syntax_error;
	var_cat(Parameters[3]);	
	mapstring.insert(pair<string,string>(Parameters[1],Parameters[3]));
	//cerr <<"我想看看映射的值"<< mapstring[Parameters[1]]<<endl;
	


	return excute_success;
}

state var_substitute(string &str)
{
	//找加号 引号。想想看怎么循环。
	//erase the space at the end of the string
	for(int i = str.length();str[i] == ' ';)
		str.erase(i,1);
	int pos = 0;
	int left_quo = 0;
	int right_quo = 0;
	int plus_pos = 0;

	string s;
	while(true)
	{
		//should break
	//	if(pos == str.length())break;
		left_quo = str.find('\"',pos);
		plus_pos = str.find('+',pos);
		//这种时候没有找到加号也没有找到引号。两种情况，一种是只有一个变量，一种是到了末尾
		if(plus_pos == str.npos && left_quo == str.npos)
		{
			//should break
		//	if(str_replace(pos,str.length()-pos,s,str)==syntax_error)
	//			return syntax_error;
		
			
			s = str.substr(pos, str.length()-pos);
			int templength = s.length();
			
			for(int i = 0;s[i] == ' ';)
					s.erase(i,1);
			int count = s.length()-1;
			for(;s[count]==' ';count--)
			for(int i = count;s[i]==' ';)		
					s.erase(i,1);
			if(s.empty())
			{	
				cerr << "error:wrong parament"<<endl;
				return syntax_error;
			}		
			//wrong parament
			for(int i = 0;i <= s.length();i++)
				if(s[i] == ' ') 
				{	
					cerr << "error:wrong parament" << endl;
					return syntax_error;
				}
			map<string,string>::iterator s_it;
			s_it = mapstring.find(s);
			if(s_it == mapstring.end())
			{
				cerr << "error:not defined variable" <<endl;
				return syntax_error;
			}
			str.replace(pos,templength,mapstring[s]);
			/*	for(int i = 0; s[i] == ' ';)
				s.erase(i,1);
			for(int i = s.length(); s[i] == ' ';)
				s.erase(i,1);

			//wrong parament
			for(int i = 0;i <= s.length();i++)
				if(s[i] == ' ') 
					cerr << "wrong parament" << endl;
			
			map<string, string>::iterator s_it;
			s_it = mapstring.find(s);
			if(s_it == mapstring.end())
			{
				cerr << "error:not defined variable" << s <<endl;
				return syntax_error;
			}
			str.replace(pos,s.length(),mapstring[s]);
		*/	
			break;
			//pos = str.length();

		//	if(pos == str.length())break;
			
		}
		else if(plus_pos !=str.npos && left_quo == str.npos)
		{
			//if(str_replace(pos,plus_pos-pos,s,str) == syntax_error)
			//	return syntax_error;
						
			s = str.substr(pos, plus_pos-pos);
			int templength = s.length();
			for(int i = 0;s[i] == ' ';)
					s.erase(i,1);
			
			int count = s.length()-1;
			for(;s[count]==' ';count--)
			for(int i = count;s[i]==' ';)		
					s.erase(i,1);
			
			if(s.empty())
			{
				cerr << "error:wrong patament"<<endl;
				return syntax_error;
			}
				map<string,string>::iterator s_it;
			s_it = mapstring.find(s);
			if(s_it == mapstring.end())
			{
				cerr << "error:not defined variable:"<<s<<endl;
				return syntax_error;
			}
			str.replace(pos,templength,mapstring[s]);
		
			plus_pos += mapstring[s].length() - templength;
			pos = plus_pos+1;
			
	//		while(str[pos]== ' ')
	//			str.erase(pos,1);
			if(pos == str.length())break;
			continue;
		}
		else if(left_quo!=str.npos && plus_pos == str.npos)
		{
			
			string check_s = str.substr(pos,left_quo-pos);
			//cerr << "please tell me check_s is "<<check_s<< endl;
			bool check = true ;
			if(check_s.empty())
				check = true;
			else
			{
				for(int i = 0; i< check_s.length();i++)
					if(check_s[i]!=' ')
						check =false;
				
			}
				
			if(!check)
			{	
				cerr << "error:wrong parament" << endl;
				return syntax_error;
			}
			right_quo = str.find('\"',left_quo+1);
			if(right_quo == str.npos)
			{
				cerr<<"error:right quotation not found"<<endl;
				return syntax_error;
			}
			pos = right_quo+1;
			for(int i = pos;str[pos]==' ';)
				str.erase(i,1);
			if(str[pos] !='\0')
			{
				cerr << "error: wrong parament"<<endl;
				return syntax_error;
			}	
			
	//		while(str[pos]== ' ')
	//			str.erase(pos,1);
			if(pos == str.length())break;
			continue;
				
			//pos = str.length();
		}
		//加号在左引号的左边，这个时候把加号左边的变量提取出来
		else
		{	if(plus_pos < left_quo)
			{
				string check_s = str.substr(pos,left_quo-pos);
			//	cerr << "please tell me check_s is "<<check_s<< endl;
				bool check = true ;
				if(check_s.empty())
					check = true;
				else
				{
					for(int i = 0; i< check_s.length();i++)
						if(check_s[i]!=' ')
							check =false;
				
				}
				
				if(!check)
				{	
					cerr << "error:wrong parament" << endl;
					return syntax_error;
				}
				//str_replace(pos ,plus_pos-pos-1,s,str);
				s = str.substr(pos,plus_pos-pos-1);
				
				int templength = s.length();
				for(int i = 0; s[i] == ' ';)
					s.erase(i,1);
				int count = s.length()-1;
				for(;s[count]==' ';count--)
				for(int i = count;s[i]==' ';)		
						s.erase(i,1);
				if(s.empty())
				{	
					cerr << "error :wrong parament"<<endl;
					return syntax_error;				
				}
				for(int i = 0;i <= s.length();i++)
					if(s[i] == ' ') 
					{	
						cerr << "error:wrong parament" << endl;
						return syntax_error;
					}
				map<string ,string>::iterator s_it;
				s_it = mapstring.find(s);
				if(s_it == mapstring.end())
				{
					cerr << "error:not defined variable:"<<s<<endl;
					return syntax_error;
				}
				str.replace(pos,s.length(),mapstring[s]);	
				//cerr << "first" << str<<endl;
				plus_pos += mapstring[s].length() - templength;
				pos = plus_pos+1;
				
			//	while(str[pos]== ' ')
			//		str.erase(pos,1);
				if(pos == str.length())break;
				continue;
			}
		
		//这段代码欠妥应该是碰到引号直接跳，不用考虑它和加号之间的关系
			else if(left_quo < plus_pos)
			{
				right_quo = str.find('\"',left_quo+1);
				if(right_quo == str.npos)
				{
					cerr<<"erro:right quotation not found"<<endl;
					return syntax_error;
				}
				else if(plus_pos < right_quo)
				{
					
					pos = right_quo+1;

			//		while(str[pos]== ' ')
			//			str.erase(pos,1);
					continue;
				}
				else if(plus_pos > right_quo)
				{
					
					string check_ls = str.substr(pos,left_quo-pos);
			//		cerr << "please tell me check_ls is "<<check_ls<< endl;
					bool checkl = true ;
					if(check_ls.empty())
						checkl = true;
					else
					{
						for(int i = 0; i< check_ls.length();i++)
							if(check_ls[i]!=' ')
								checkl =false;
				
					}
				
					if(!checkl)
					{	
						cerr << "error:wrong parament" << endl;
						return syntax_error;
					}

					string check_rs = str.substr(right_quo+1,plus_pos-right_quo-1);
			//		cerr << "please tell me check_rs is "<<check_rs <<endl;
					bool checkr = true ;
					if(check_rs.empty())
						checkr = true;
					else
					{
						for(int i = 0; i< check_rs.length();i++)
							if(check_rs[i]!=' ')
								checkr =false;
				
					}
				
					if(!checkr)
					{	
						cerr << "error:wrong parament" << endl;
						return syntax_error;
					}
					/*
					s = str.substr(right_quo+1, plus_pos-right_quo-1);
					for(int i = 0; s[i] == ' ';)
					s.erase(i,1);
					for(int i = s.length();s[i]== ' '; )
					s.erase(i,1);
			
					map<string ,string >::iterator s_it;
					cerr << "show me s  is" << s << endl;
					s_it = mapstring.find(s);
					if(s_it == mapstring.end())
					{
						cerr << "here?error:not defined variable:"<<s<<endl;
						return syntax_error;
					}
					str.replace(pos,s.length(),mapstring[s]);*/
					pos = plus_pos+1;
				//	while(str[pos]== ' ')
				//		str.erase(pos,1);
					if(pos == str.length())break;
					continue;
				}
			
			}
		}
	}
//	cerr << "I want to see" << str <<endl;
	return excute_success;
}

state var_extention(string &str)
{
	if(str[0]=='$')
	{
		int ext_pos = 0;
		int left_pos = 0;
		int right_pos =0;
		if(var_replace(left_pos,right_pos,ext_pos,str)== syntax_error)return syntax_error;
	}
	return excute_success;

}
/*
state var_extention(string &str)
{
	//找到左括号，再找右括号，从右括号开始找下一个左括号
	//找$ 或者左双引号
	int pos = 0;
	int left_quo = 0;
	int right_quo = 0;
	int ext_pos = 0;
	int left_pos = 0;
	int right_pos = 0;
	
	//cerr << "在进行变量扩展之前，我得到了这样的参数"<<str<<endl;
	//如果双引号在$左边这个时候应该处理双引号里面的变量扩展。
	while(true)
	{
		left_quo = str.find('\"',pos);
		ext_pos = str.find('$',pos);
		if(left_quo == str.npos && ext_pos ==str.npos)
		{	
			//cerr << "没找到双引号  也没找到变量扩展" <<endl;
			cerr << "error：命令参数不符合语义规定" <<endl;
			return syntax_error;
		}
		else if(ext_pos == str.npos && left_quo != str.npos)
		{
			if(str.find('\"',left_quo+1) ==str.npos)
			{
				cerr<<"error：未找到匹配的右引号"<<endl;
				return syntax_error;
			}
			//cerr<<"我什么都没做"<<endl;
			break;
		}
		//替换
		else if(ext_pos != str.npos && left_quo == str.npos)
		{
			//cerr << "找到了变量扩展这个时候应该把变量展开" << endl;
			
			if(var_replace(left_pos,right_pos,ext_pos,str) == syntax_error)return syntax_error;
		}
		else if(left_quo != str.npos && ext_pos !=str.npos)
		{
			//cerr << "找到了双引号，也找到了变量扩展" << endl;
			if(left_quo < ext_pos)
			{
				right_quo = str.find('\"',(left_quo+1));
				//讨论一下$符号与右双引号之间的位置关系
				if(right_quo == str.npos)
				{
					cerr<<"error:未找到匹配的右引号"<<endl;
					return syntax_error;
				}
				//此刻是引号内部的变量扩展不管它 就继续
				else if(ext_pos < right_quo)
				{
					pos = right_quo + 1;
					if(str.find('\"',pos) == str.npos && str.find('$',pos)==str.npos)break;

					continue;					
				}
		
				//这个时候$符号在右双引号的右边，仍然是引号外面的变量扩展
				if (right_quo < ext_pos)
					if(var_replace(left_pos,right_pos,ext_pos,str)==syntax_error)return syntax_error;
				
			}
	
			//如果$在双引号左边，这个时候进行括号外的变量扩展
			if(ext_pos < left_quo)
				if(var_replace(left_pos,right_pos,ext_pos,str)==syntax_error)return syntax_error;
		}
		//这段好像是多余代码，考虑到整形的扩展，应该从这里开始
		else
		{
			cerr << "syntax_error" <<endl;
			return syntax_error;
		}
	}
	//cerr << "在变量扩展之后，我得到了这样的参数" << str<<endl;
	
	return  excute_success;
}*/
state var_replace(int &left_pos,int &right_pos,int &ext_pos,string &str)
{
	left_pos = str.find('{',ext_pos);
		if(left_pos == str.npos)
		{
			cerr << "error:wrong var extention"<<endl;
			return syntax_error;
		}
		else if((right_pos = str.find('}',left_pos)) == str.npos)
		{
			cerr << "error:wrong var extention" << endl;
			return syntax_error;
		}	
		else 
		{
			string s = str.substr(ext_pos + 2,(right_pos - ext_pos - 2));
			map<string ,string >::iterator s_it;
			s_it = mapstring.find(s);
			if(s_it == mapstring.end())
			{
				cerr << "error:not defined variable:"<<s<<endl;
				return syntax_error;
			}
			str.replace(ext_pos,s.length()+3,mapstring[s]);
			

		}
		return excute_success;

}
state var_cat(string &str)
{
//	cerr << " before the str cat:"<< str<<endl;
	int left_quo = 0;
	int right_quo = 0;
	string s;
	s += "\"";
	left_quo = str.find('\"',left_quo);
/*	if(left_quo != 0)
	{
		cerr << "error:未识别的参数" << endl;
		return syntax_error;
	}
	*/
	while(true)
	{
		right_quo = str.find('\"',left_quo+1);
		s += str.substr(left_quo + 1, right_quo - left_quo -1);
		left_quo = str.find('\"',right_quo + 1);

		if(left_quo == str.npos)
		{
			//if(str[str.length()-1]== '\n') cerr << "是结尾符" <<endl;
			//cerr << "right is " <<right_quo << "  " <<"strlength is " <<str.length()<<endl;
		/*	for(int i = right_quo+1; i < str.length(); i++ )
				if(str[i] !=  ' ')
				{
					cerr <<"error: 未识别的参数" << endl;
					return syntax_error;
				}*/
			break;
		}
/*		if(cat_analyze(right_quo,left_quo,str)==syntax_error)
		{
			//cerr << "error:未识别的参数" << endl;
			return syntax_error;
		}
		
*/	
	}
	s += "\"";
	str = s;
//	cerr << "after the string cat:" <<str<<endl;
	return excute_success;
}
/*state cat_analyze(int &right_quo,int &left_quo,string &str)
{
	
	int op_num; 
	op_num= 0;
	for(int i = right_quo+1; i < left_quo;i++)
	{
		if(str[i]!=' ' && str[i] != '+')
		{
			
			cerr << "error:未识别的参数"<<endl;
			return syntax_error;
		}
		if(str[i] == '+')op_num++;
	
	}
	//cerr << "op_num的值是" << op_num <<endl;
	if(op_num > 1 || op_num == 0)
	{
		cerr << "error:未识别的参数"<<endl;
		return syntax_error;
	}
	return excute_success;
}
*/




