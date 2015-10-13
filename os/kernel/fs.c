#include "protect.h"
#include "string.h"
#include "proc.h"
#include "global.h"
#include "proto.h"
#include "hd.h"
#include "fs.h"

PUBLIC void init_fs(){
	//初始化p_fd文件描述符表
	int i = 0;
	for(i = 0; i < NR_FILE_DESC; i++)
		memset(&file_desc_table[i], 0, sizeof(FILE_DESCRIPTOR));
}

PUBLIC int kernelOpenFile(Device device, const char* path, FILE_MODE mode)
{
	int dp = -1;
	
	FILE_DESCRIPTOR fd;
	

	return dp;
}
