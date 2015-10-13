#include "RTL.h"

char* readkeybuf = NULL;

int main()
{
//	while(1)
//	{
//		readKey();
//	}

	char* buf[121] = {'\0'};
	memset(buf, 0, 121);
	int fd;
	fd = fileOpen(0, "a.txt", DEV_OM_READ);
	if(fd == -1)
		ttyOutputStr("open failed!!!!!!!!!!!\n");
	else{
		ttyOutputStr("fileOpened!\nfile content:");
		fileRead(fd, buf, 120);
		ttyOutputStr(buf);
	}

	return 0;
}
