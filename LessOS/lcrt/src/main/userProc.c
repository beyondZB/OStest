#include "RTL.h"
#include "api.h"
#include "string.h"

void _exit();
void _start()
{
	main();
	_exit();
}

//int main()
//{
//	int j = 0;
//	for(j = 0; j < 1000000; j++)
//	{
//		char* a = (char*)malloc(15);
//		int i = 0;
//		for(i = 0; i < 14; i++)
//		{
//			a[i] = i + '0';
//		}
//		a[i] = '\0';
//		ttyOutputStr(a);
//		free(a);
//	}
//}

void _exit()
{
	while(1){};
	exitProcess();
}
