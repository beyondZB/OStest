/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 kinuxroot
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/** @file       lofs.h
 *  @brief      LOFS文件系统功能实现
 *  @date       2014年11月07日 15时57分28秒
 *  @author     kinuxroot, kinuxroot@163.com
 *  @group      LessOS Development Group
 *  @version    1.0
 *  @note       
 *              支持功能：
 *                  清除磁盘
 *                  格式化分区
 *                  放入文件
 *                  取出文件
 *                  删除文件
 *                  写入引导代码
 *
 */

#ifndef _LOFS_H
#define _LOFS_H

#include "type.h"
#include "const.h"
#include "lib/device.h"

#define	FILE_READ_ONLY	0
#define FILE_READ_WRITE	1


#define DISK_SIZE   1474560
#define BLOCK_SECTORS   1
#define SECTOR_BYTES    512
#define BYTE_BITS       8
#define BLOCK_BYTES     (BLOCK_SECTORS * SECTOR_BYTES)


#define INODE_SIZE  32

#define FILEITEM_FILENAME_MAX_SIZE  (14)



typedef unsigned char FILE_MODE;

typedef struct {
    //  代号        起始偏移    占用字节    解释
    //  version     0           1           文件系统版本
    u8 version;
    //  meta-block  1           2           元数据占用块数
    u16 metaBlocks;
    //  total-size  3           4           分区大小
    u32 totalSize;
    //  total-block 7           2           分区块数
    u16 totalBlocks;
    //  bfree-start 9           2           空闲块记录区域起始编号
    u16 blockFreeStart;
    //  bfree-block 11          2           空闲块记录区域占用块数
    u16 blockFreeBlocks;
    //  i-count     13          2           i-node个数
    u16 inodeCount;
    //  ifree-start 15          2           空闲i-node记录区域起始编号
    u16 inodeFreeStart;
    //  ifree-block 17          2           空闲i-node记录区域占用块数
    u16 inodeFreeBlocks;
    //  i-start     19          2           i-node起始块编号
    u16 inodeStart;
    //  i-block     21          2           i-node占用块数
    u16 inodeBlocks;
    //  data-start  23          2           数据区域起始块编号
    u16 dataStart;
    //  data-block  25          2           数据区域占用块数
    u16 dataBlocks;
} Metadata;

#define INODE_FILETYPE_NORMAL       (0)
#define INODE_FILETYPE_DIRECTORY    (1)

#define INODE_DBIDX_COUNT   4
#define INODE_1IBIDX_COUNT   2
#define INODE_2IBIDX_COUNT   1

#define ROOT_USER_ID    0
#define ROOT_GROUP_ID   0

//  i-node
typedef struct {
    //  名称        起始偏移    占用字节    解释
    //  filetype    0           1           文件类型
    u8 fileType;
    //  link        1           1           链接数
    u8 link;
    //  uid         2           2           属主UID
    u16 uid;
    //  gid         4           2           属主GID
    u16 gid;
    //  size        6           4           文件大小
    u32 size;
    //  ctime       10          4           创建时间
    u32 createTime;
    //  mtime       14          4           修改时间
    u32 modifiedTime;
    //  dbidx       18          8           4个直接索引块
    u16 directBlockIndex[INODE_DBIDX_COUNT];
    //  1ibidx      26          4           2个一级间接索引块
    u16 l1IndirectBlockIndex[INODE_1IBIDX_COUNT];
    //  2ibidx      30          2           1个二级间接索引块
    u16 l2IndirectBlockIndex[INODE_2IBIDX_COUNT];
} IndexNode;

typedef struct file_descriptor{
	u32	fd_mode;
	u32	fd_position;
	char	file_name[FILEITEM_FILENAME_MAX_SIZE + 1];
	IndexNode inode;
	Device	device;
}FILE_DESCRIPTOR;

typedef struct {
    //  名称        起始偏移    占用字节    解释
    //  filename    0           14          文件名
    char filename[FILEITEM_FILENAME_MAX_SIZE + 1];
    //  i-node      14          2           i-node索引
    u16 indexNode;
} FileItem;

PUBLIC void lofsFormatDevice(Device device);
PUBLIC void lofsListFiles(Device device);
PUBLIC u8 lofsPutFile(Device device, const char* filename);
PUBLIC u8 lofsGetFile(Device device, const char* filename);
PUBLIC u8 lofsRemoveFile(Device device, const char* filename);

#define LOFS_PUTFILE_SUCCESSFUL             0
#define LOFS_PUTFILE_FILE_EXIST             1
#define LOFS_PUTFILE_LOCALFILE_NOT_EXIST    2

#define LOFS_GETFILE_SUCCESSFUL     0
#define LOFS_GETFILE_FILE_NOT_EXIST 1

#define LOFS_REMOVEFILE_SUCCESSFUL      0
#define LOFS_REMOVEFILE_FILE_NOT_EXIST  1

#endif /*_LOFS_H*/
