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
	//cerr << "���뿴����������ʲô����:";
	//for(int i = 0; i <para_length; i++ ) cerr << Parameters[i] <<" ";
	//ִ��֮���������
	//cerr << endl;
	for(int i = 0;i <para_length;i++)Parameters[i].clear();

	//����ط�һ��Ҫˢ��
	cin.clear();
    cin.ignore(INT_MAX, '\n');


	return 0;
}
state outer_command()
{
//���������Ѿ�����  ֻҪ exec һ�¾����ˣ�����������
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
   	   				 /* ���exec�������أ�����û������ִ�������ӡ������Ϣ*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			case 2:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(), NULL );
   	   				 /* ���exec�������أ�����û������ִ�������ӡ������Ϣ*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			case 3:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(),Parameters[2].c_str(), NULL );
   	   				 /* ���exec�������أ�����û������ִ�������ӡ������Ϣ*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			case 4:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(),Parameters[2].c_str(),Parameters[3].c_str(), NULL );
   	   				 /* ���exec�������أ�����û������ִ�������ӡ������Ϣ*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			case 5:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(),Parameters[2].c_str(),Parameters[3].c_str(),
									   	Parameters[4].c_str(), NULL );
   	   				 /* ���exec�������أ�����û������ִ�������ӡ������Ϣ*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			case 6:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(),Parameters[2].c_str(),Parameters[3].c_str(),
									   	Parameters[4].c_str(), Parameters[5].c_str(),NULL );
   	   				 /* ���exec�������أ�����û������ִ�������ӡ������Ϣ*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			case 7:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(),Parameters[2].c_str(),Parameters[3].c_str(),
									   	Parameters[4].c_str(), Parameters[5].c_str(),Parameters[6].c_str(),NULL );
   	   				 /* ���exec�������أ�����û������ִ�������ӡ������Ϣ*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
	   		case 8:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(),Parameters[2].c_str(),Parameters[3].c_str(),
									   	Parameters[4].c_str(), Parameters[5].c_str(),Parameters[6].c_str(),Parameters[7].c_str(),NULL );
   	   				 /* ���exec�������أ�����û������ִ�������ӡ������Ϣ*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			case 9:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(),Parameters[2].c_str(),Parameters[3].c_str(),
									   	Parameters[4].c_str(), Parameters[5].c_str(),Parameters[6].c_str(),Parameters[7].c_str(),Parameters[8].c_str(),NULL );
   	   				 /* ���exec�������أ�����û������ִ�������ӡ������Ϣ*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			case 10:
					{
					
    					execlp(Parameters[0].c_str(),Parameters[0].c_str(),Parameters[1].c_str(),Parameters[2].c_str(),Parameters[3].c_str(),
									   	Parameters[4].c_str(), Parameters[5].c_str(),Parameters[6].c_str(),Parameters[7].c_str(),Parameters[8].c_str(),
										Parameters[9].c_str(),NULL );
   	   				 /* ���exec�������أ�����û������ִ�������ӡ������Ϣ*/
       					perror(Parameters[0].c_str());
       					exit( errno ); 
						break;
					};
			
		
		}
		 /* �ӽ��̳��� */
    //	execlp( "vim","vim","b.cpp", NULL );
   	    /* ���exec�������أ�����û������ִ�������ӡ������Ϣ*/
       // perror( "vim" );
       // exit( errno ); 
	 }
    else 
    {
       /* �����̳���*/
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
	
	//para[2]�����ǵȺ�
	//if(var_extention(Parameters[3])== syntax_error)return syntax_error;
	//��ʼ�����ַ�����ƴ��
	//if(var_cat(Parameters[3])==syntax_error)return syntax_error;
	if(var_substitute(Parameters[3])== syntax_error)return syntax_error;
	var_cat(Parameters[3]);	
	mapstring.insert(pair<string,string>(Parameters[1],Parameters[3]));
	//cerr <<"���뿴��ӳ���ֵ"<< mapstring[Parameters[1]]<<endl;
	


	return excute_success;
}

state var_substitute(string &str)
{
	//�ҼӺ� ���š����뿴��ôѭ����
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
		//����ʱ��û���ҵ��Ӻ�Ҳû���ҵ����š����������һ����ֻ��һ��������һ���ǵ���ĩβ
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
		//�Ӻ��������ŵ���ߣ����ʱ��ѼӺ���ߵı�����ȡ����
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
		
		//��δ���Ƿ��Ӧ������������ֱ���������ÿ������ͼӺ�֮��Ĺ�ϵ
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
	//�ҵ������ţ����������ţ��������ſ�ʼ����һ��������
	//��$ ������˫����
	int pos = 0;
	int left_quo = 0;
	int right_quo = 0;
	int ext_pos = 0;
	int left_pos = 0;
	int right_pos = 0;
	
	//cerr << "�ڽ��б�����չ֮ǰ���ҵõ��������Ĳ���"<<str<<endl;
	//���˫������$������ʱ��Ӧ�ô���˫��������ı�����չ��
	while(true)
	{
		left_quo = str.find('\"',pos);
		ext_pos = str.find('$',pos);
		if(left_quo == str.npos && ext_pos ==str.npos)
		{	
			//cerr << "û�ҵ�˫����  Ҳû�ҵ�������չ" <<endl;
			cerr << "error�������������������涨" <<endl;
			return syntax_error;
		}
		else if(ext_pos == str.npos && left_quo != str.npos)
		{
			if(str.find('\"',left_quo+1) ==str.npos)
			{
				cerr<<"error��δ�ҵ�ƥ���������"<<endl;
				return syntax_error;
			}
			//cerr<<"��ʲô��û��"<<endl;
			break;
		}
		//�滻
		else if(ext_pos != str.npos && left_quo == str.npos)
		{
			//cerr << "�ҵ��˱�����չ���ʱ��Ӧ�ðѱ���չ��" << endl;
			
			if(var_replace(left_pos,right_pos,ext_pos,str) == syntax_error)return syntax_error;
		}
		else if(left_quo != str.npos && ext_pos !=str.npos)
		{
			//cerr << "�ҵ���˫���ţ�Ҳ�ҵ��˱�����չ" << endl;
			if(left_quo < ext_pos)
			{
				right_quo = str.find('\"',(left_quo+1));
				//����һ��$��������˫����֮���λ�ù�ϵ
				if(right_quo == str.npos)
				{
					cerr<<"error:δ�ҵ�ƥ���������"<<endl;
					return syntax_error;
				}
				//�˿��������ڲ��ı�����չ������ �ͼ���
				else if(ext_pos < right_quo)
				{
					pos = right_quo + 1;
					if(str.find('\"',pos) == str.npos && str.find('$',pos)==str.npos)break;

					continue;					
				}
		
				//���ʱ��$��������˫���ŵ��ұߣ���Ȼ����������ı�����չ
				if (right_quo < ext_pos)
					if(var_replace(left_pos,right_pos,ext_pos,str)==syntax_error)return syntax_error;
				
			}
	
			//���$��˫������ߣ����ʱ�����������ı�����չ
			if(ext_pos < left_quo)
				if(var_replace(left_pos,right_pos,ext_pos,str)==syntax_error)return syntax_error;
		}
		//��κ����Ƕ�����룬���ǵ����ε���չ��Ӧ�ô����￪ʼ
		else
		{
			cerr << "syntax_error" <<endl;
			return syntax_error;
		}
	}
	//cerr << "�ڱ�����չ֮���ҵõ��������Ĳ���" << str<<endl;
	
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
		cerr << "error:δʶ��Ĳ���" << endl;
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
			//if(str[str.length()-1]== '\n') cerr << "�ǽ�β��" <<endl;
			//cerr << "right is " <<right_quo << "  " <<"strlength is " <<str.length()<<endl;
		/*	for(int i = right_quo+1; i < str.length(); i++ )
				if(str[i] !=  ' ')
				{
					cerr <<"error: δʶ��Ĳ���" << endl;
					return syntax_error;
				}*/
			break;
		}
/*		if(cat_analyze(right_quo,left_quo,str)==syntax_error)
		{
			//cerr << "error:δʶ��Ĳ���" << endl;
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
			
			cerr << "error:δʶ��Ĳ���"<<endl;
			return syntax_error;
		}
		if(str[i] == '+')op_num++;
	
	}
	//cerr << "op_num��ֵ��" << op_num <<endl;
	if(op_num > 1 || op_num == 0)
	{
		cerr << "error:δʶ��Ĳ���"<<endl;
		return syntax_error;
	}
	return excute_success;
}
*/




