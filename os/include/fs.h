#ifndef _FS_H
#define _FS_H
#include "lofs.h"

struct file_descriptor{
	u32	fd_mode;
	u32	fd_position;
	struct IndexNode* inode;
}FILE_DESCRIPTOR;

typedef int FILE_MODE
#define	FILE_READ_ONLY	0
#define FILE_READ_WRITE	1

#endif
