/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 kinuxroot
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the " Software" ), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED " AS IS" , WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/** @file       lofs.c
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

#include "type.h" 
#include "proto.h" 
#include "const.h" 
#include "protect.h" 
#include "string.h" 
#include "lofs.h" 
#include "proc.h" 
#include "global.h" 
#include "hd.h" 

// i-node数量
#define INODE_COUNT 256
// 根目录i-node编号
#define ROOT_INODE_INDEX    0

// 执行整数除法并取上底
#define CEIL_DIV(x, y) ((x) % (y) ? (x) / (y) + 1 : (x) / (y))
// 取最小值
#define MIN(x, y) ((x) < (y) ? (x) : (y))

// 分区起始位置块编号（相对整个设备）
#define PARTITION_BEGIN_BLOCK 3

PRIVATE void initMetadata(Device device, Metadata* metadata);
PRIVATE void initBlockFreeArea(Device device, Metadata* metadata);
PRIVATE void initIndexNodeFreeArea(Device device, Metadata* metadata);
PRIVATE void initIndexNodeArea(Device device, Metadata* metadata);
PRIVATE void initDataArea(Device device, Metadata* metadata);

PRIVATE void fillMetadata(Metadata* metadata);
//PRIVATE void printMetadata(Metadata* metadata);
PRIVATE void writeMetadata(Device device, Metadata* metadata);
PRIVATE void readMetadata(Device device, Metadata* metadata);
PRIVATE Size getRemainingBlocks(Device device, Metadata* metadata);
PRIVATE Size getRemainingSpace(Device device, Metadata* metadata);
PRIVATE Size getRemainingIndexNodes(Device device, Metadata* metadata);

PRIVATE void initBlockBitmap(Device device, Metadata* metadata);
PRIVATE void initIndexNodeBitmap(Device device, Metadata* metadata);

PRIVATE void initRootIndexNode(Device device, Metadata* metadata);
PRIVATE void fillRootIndexNode(Metadata* metadata, IndexNode* inodeOfRoot);

PRIVATE void initRootFile(Device device, Metadata* metadata);
PRIVATE void fillRootFileItem(FileItem* rootFileItem);
PRIVATE void fillRootParentFileItem(FileItem* rootParentFileItem);


PRIVATE void setIndexNodeUsed(Device device, Metadata* metadata, u16 inodeIndex);
PRIVATE void resetIndexNodeUsed(Device device, Metadata* metadata, u16 inodeIndex);
PRIVATE u8 isIndexNodeUsed(Device device, Metadata* metadata, u16 inodeIndex);

PRIVATE u16 occupyAvaliableIndexNode(Device device, Metadata* metadata);
PRIVATE void initIndexNode(IndexNode *inode);
PRIVATE void readIndexNode(Device device, Metadata* metadata, u16 inodeIndex, IndexNode* inode);
PRIVATE void writeIndexNode(Device device, Metadata* metadata, u16 inodeIndex, IndexNode* inode);
PRIVATE void releaseIndexNode(Device device, Metadata *metadata, u16 inodeIndex);
PRIVATE void releaseIndexNodeBlocks(Device device, Metadata *metadata, IndexNode *indexNode);

PRIVATE void fillFileItem(FileItem* fileItem, const char* const filename, u16 inodeIndex);
PRIVATE void writeFileItem(Device device, FileItem* fileItem);
PRIVATE void readFileItem(Device device, FileItem* fileItem);
PRIVATE void appendFileItem(Device device, Metadata* metadata, FileItem* fileItem);
PRIVATE u8 findFileItem(Device device, Metadata* metadata, const char* filename, FileItem* fileItem);
PRIVATE void releaseFileItem(Device device, Metadata* metadata, const char *filename);

PRIVATE void setBlockUsed(Device device, Metadata* metadata, u16 blockIndex);
PRIVATE void resetBlockUsed(Device device, Metadata* metadata, u16 blockIndex);
PRIVATE u8 isBlockUsed(Device device, Metadata* metadata, u16 blockIndex);

PRIVATE u16 occupyAvaliableBlock(Device device, Metadata* metadata);
PRIVATE void storeBlockIndex(Device device, Metadata* metadata, IndexNode* inode, u16 nextBlock);
PRIVATE u8 isIndexBlockFull(Device device, Metadata* metadata, u16 block);
PRIVATE u8 storeBlockIndexInDirectList(u16* directBlockIndex, u16 indexCount, u16 nextBlock);
PRIVATE u8 storeBlockIndexInL1IndirectList(
        Device device, Metadata* metadata, u16* l1IndirectBlockIndex, u16 indexCount, u16 nextBlock);
PRIVATE u8 storeBlockIndexInL2IndirectList(
        Device device, Metadata* metadata, u16* l2IndirectBlockIndex, u16 indexCount, u16 nextBlock);
PRIVATE void releaseDirectBlocks(Device device, Metadata *metadata, 
        u16 *directBlockIndex, u16 indexCount);
PRIVATE void releaseL1IndirectBlocks(Device device, Metadata *metadata, 
        u16 *l1IndirectBlockIndex, u16 indexCount);
PRIVATE void releaseL2IndirectBlocks(Device device, Metadata *metadata, 
        u16 *l2IndirectBlockIndex, u16 indexCount);

PRIVATE void seekToPartitionBlock(Device device, u16 blockIndex);
PRIVATE void seekToBlock(Device device, u16 blockIndex);
PRIVATE void clearBlock(Device device, u16 blocks);

PRIVATE void writeU8(Device device, u8 value);
PRIVATE void writeU16(Device device, u16 value);
PRIVATE void writeU32(Device device, u32 value);
PRIVATE void readU8(Device device, u8* value);
PRIVATE void readU16(Device device, u16* value);
PRIVATE void readU32(Device device, u32* value);

PRIVATE u8 isLocalFileExist(const char* filename);

PUBLIC void init_fs();
PUBLIC int kernelOpenFile(int deviceNo, const char* path, FILE_MODE mode);
PUBLIC int sysOpenFile(int deviceNo, const char* path, FILE_MODE mode);
PUBLIC int kernelFileRead(int fd, char * buf, int size);
PUBLIC int sysFileRead(int fd, char* buf, int size);
PUBLIC void kernelCloseFile(int fd);
PUBLIC int sysCloseFile(int fd);
PRIVATE void copyFileContentToBuf(Device device, Metadata *metadata, IndexNode *inode, char* buf, Size size);
PRIVATE void copyContentToBufInDirectIndex(Device device, char** pbuf, u16 *directBlockIndex, u16 indexCount, Size *remainingSize);
PRIVATE void copyContentToBufInL1IndirectIndex(Device device, char** pbuf, u16 *l1IndirectBlockIndex, u16 indexCount, Size *remainingSize);
PRIVATE void copyContentToBufInL2IndirectIndex(Device device, char** pbuf, u16 *l2IndirectBlockIndex, u16 indexCount, Size *remainingSize);
PRIVATE void copyBlockContentToBuf(Device device, char** pbuf, u16 block, Size *remainingSize);


PUBLIC void init_fs(){
	//初始化p_fd文件描述符表
	int i = 0;
	for(i = 0; i < NR_FILE_DESC; i++)
		memset(&file_desc_table[i], 0, sizeof(FILE_DESCRIPTOR));

	//初始化deviceM设备表
	for(i = 0; i < NR_DEVICE_MALLOC; i++)
		memset(&deviceM, 0, sizeof(DeviceObject));
}

PUBLIC int sysOpenFile(int deviceNo, const char* path, FILE_MODE mode)
{
	return kernelOpenFile(deviceNo, path, mode);
}

/**
 * @brief 打开文件操作，需要调用者先设置好Device作为参数，
 *        在函数内，选取空闲的fdp,和file_descriptor，
 *        为file_descriptor,初始化
 *
 * @param device 设备对象
 * @param path	 文件名
 * @param mode	 文件操作模式
 *
 * @return 无返回值
 */
PUBLIC int kernelOpenFile(int deviceNo, const char* filename, FILE_MODE mode)
{
//	disp_str(" kernelOpenFile: " );
//	disp_int((void*)kernelOpenFile);
//	disp_str(" filename: " );
//	disp_str(filename);
	int fd = -1;

	int i = 0;
	//找空闲的fdp
	for(i = 0; i < NR_FDP; i++){
		if(p_proc_running->fdp_table[i] == 0){
			fd = i;
			break;
		}
	}
	if(fd < 0 || fd > NR_FDP)
		return -1;
//	disp_str(" fdp_table get!!!!! " );

	Device device = openDevice("sda" , DEV_OM_READ);
	if(device != NULL)
	{
		p_proc_running->fdp_table[fd]->device = device;
//		disp_str(" device get!!! " );
	}
	else{
//		disp_str(" device not get!!! " );
		return -1;
	}

	//找空闲的file_descriptor
	for(i = 0; i < NR_FILE_DESC; i++){
		if(file_desc_table[i].inode.link == 0){
			p_proc_running->fdp_table[fd] = &(file_desc_table[i]);
			break;
		}
	}
	if(i >= NR_FILE_DESC)
		return -1;
//	disp_str(" file_descriptor get!!! " );

	FileItem fileItem;
	
	Metadata metadata;
	readMetadata(device, &metadata);
	
//	disp_str(" pre file find!! " );
	// 如果指定文件在设备分区根目录下不存在，则返回错误信息
	if ( !findFileItem(device, &metadata, filename, &fileItem) ) {
//		disp_str(" file not exist!! " );
		return -1;
	}
	strncpy(p_proc_running->fdp_table[fd]->file_name, filename, FILEITEM_FILENAME_MAX_SIZE);
	disp_color_str(p_proc_running->fdp_table[fd]->file_name, 0x4F);
	
	// 获取文件项对应的i-node
	readIndexNode(device, &metadata, fileItem.indexNode, &(p_proc_running->fdp_table[fd]->inode));

	device->position = 0;
	device->mode = mode;
	
//	disp_str(" open fd: " );
	disp_int(fd);
	return fd;
}


PUBLIC int sysFileRead(int fd, char* buf, int size)
{
	return kernelFileRead(fd, buf, size);
}

PUBLIC int kernelFileRead(int fd, char * buf, int size)
{
//	disp_str(" fd: " );
//	disp_int(fd);
//	disp_str("  size: " );
//	disp_int(size);
	Device device = p_proc_running->fdp_table[fd]->device;
	IndexNode* inode = &(p_proc_running->fdp_table[fd]->inode);
	char * filename = p_proc_running->fdp_table[fd]->file_name;
	FileItem fileItem;
	Metadata metadata;
	readMetadata(device, &metadata);
	
	// 如果指定文件在设备分区根目录下不存在，则返回错误信息
	if ( !findFileItem(device, &metadata, filename, &fileItem) ) {
	    return LOFS_GETFILE_FILE_NOT_EXIST;
	}
	
//	disp_str(" after find file item!! ");
	// 获取文件项对应的i-node
	readIndexNode(device, &metadata, fileItem.indexNode, inode);
	
	// 将分区文件内容拷贝到buf
	copyFileContentToBuf(device, &metadata, inode, buf, size);
	
	return size;
}

PUBLIC int sysCloseFile(int fd)
{
	kernelCloseFile(fd);
	return 0;
}


PUBLIC void kernelCloseFile(int fd)
{
	memset(p_proc_running->fdp_table[fd], 0, sizeof(FILE_DESCRIPTOR*));
	p_proc_running->fdp_table[fd] = NULL;
}


/**
 * @brief 格式化设备上的分区，建立文件系统
 *        分为以下几步：
 *            1.初始化分区元数据
 *            2.初始化空闲块映射表（根据实际块占用情况初始化）
 *            3.初始化空闲i-node映射表（根据实际i-node占用情况初始化）
 *            4.初始化i-node表，需要初始化根目录的i-node
 *            4.初始化数据区域，需要初始化根目录的数据
 *
 * @param device 设备对象
 *
 * @return 无返回值
 */
PUBLIC void lofsFormatDevice(Device device){
    Metadata metadata;
    initMetadata(device, &metadata);
    initBlockFreeArea(device, &metadata);
    initIndexNodeFreeArea(device, &metadata);
    initIndexNodeArea(device, &metadata);
    initDataArea(device, &metadata);
}

///**
// * @brief 获取设备元数据和其他的统计信息
// *
// * @param device 设备对象
// *
// * @return 无返回值
// */
//PUBLIC void lofsShowMetadata(Device device){
//    Metadata metadata;
//    readMetadata(device, &metadata);
//
//    // 显示元数据
//    printMetadata(&metadata);
//
//    // 显示更多统计信息
//    printf(" \nMore information:\n" );
//    // 剩余块数
//    printf(" Remaining blocks: %ld\n" , getRemainingBlocks(device, &metadata));
//    // 剩余空间
//    printf(" Remaining space: %ld\n" , getRemainingSpace(device, &metadata));
//    // 剩余i-node数
//    printf(" Remaining i-nodes: %ld\n" , getRemainingIndexNodes(device, &metadata));
//}

///**
// * @brief 列出设备分区根目录下的所有文件（包括当前目录和上层目录）
// *        现在只能列出根目录下的32个文件（暂时没有需求超出那么多文件，所以就不写了 = =）
// *
// * @param device 设备对象
// *
// * @return 无返回值
// */
//PUBLIC void lofsListFiles(Device device){
//    Metadata metadata;
//    int fileItemIndex = 0;
//
//    readMetadata(device, &metadata);
//    // 直接移动到根目录数据区，根目录第一个数据块肯定是第分区一个数据块
//    seekToPartitionBlock(device, metadata.dataStart);
//    printf(" Filename\tIndexNode\n=========================\n" );
//    // 遍历根目录的第一个数据块，每个FileItem 16字节，一个数据块512字节，因此最多只有32个文件项
//    for ( fileItemIndex = 0; fileItemIndex < 32; fileItemIndex ++ ) {
//        FileItem fileItem;
//        readFileItem(device, &fileItem);
//
//        if ( fileItem.filename[0] ) {
//            printf(" %-16s%d\n" , fileItem.filename, fileItem.indexNode);
//        }
//    }
//}

///**
// * @brief 将指定的本地文件存放到设备分区中
// *        会在分区根目录下生成同名文件，并复制本地文件数据
// *
// * @param device 设备对象
// * @param filename 本地文件路径
// *
// * @return 错误号，取值参见错误号列表
// */
//PUBLIC u8 lofsPutFile(Device device, const char *filename){
//    // 如果本地文件不存在，则返回错误信息
//    if ( !isLocalFileExist(filename) ) {
//        return LOFS_PUTFILE_LOCALFILE_NOT_EXIST;
//    }
//
//    FileItem fileItem;
//
//    Metadata metadata;
//    readMetadata(device, &metadata);
//
//    // 如果该文件在分区中已存在，则返回错误信息
//    if ( findFileItem(device, &metadata, filename, &fileItem) ) {
//        return LOFS_PUTFILE_FILE_EXIST;
//    }
//
//    // 获取并占用一个空闲i-node
//    u16 inodeIndex = occupyAvaliableIndexNode(device, &metadata);
//
//    // 初始化i-node，复制数据并将初始文件链接数设置成1，写入i-node
//    IndexNode inode;
//    initIndexNode(&inode);
//    copyFileContentFromLocalFile(device, &metadata, &inode, filename);
//    inode.link = 1;
//    writeIndexNode(device, &metadata, inodeIndex, &inode);
//    
//    // 初始化文件项，并添加到根目录下
//    fillFileItem(&fileItem, filename, inodeIndex);
//    appendFileItem(device, &metadata, &fileItem);
//
//    return LOFS_PUTFILE_SUCCESSFUL;
//}

///**
// * @brief 获取指定设备分区上的文件
// *        会在当前目录中生成同名文件，并复制分区中的文件数据
// *
// * @param device 设备对象
// * @param filename 文件名
// *
// * @return 错误号，取值参见错误号列表
// */
//PUBLIC u8 lofsGetFile(Device device, const char* filename){
//    FileItem fileItem;
//
//    Metadata metadata;
//    readMetadata(device, &metadata);
//
//    // 如果指定文件在设备分区根目录下不存在，则返回错误信息
//    if ( !findFileItem(device, &metadata, filename, &fileItem) ) {
//        return LOFS_GETFILE_FILE_NOT_EXIST;
//    }
//
//    // 获取文件项对应的i-node
//    IndexNode inode;
//    readIndexNode(device, &metadata, fileItem.indexNode, &inode);
//
//    // 将分区文件内容拷贝到本地文件
//    copyFileContentToLocalFile(device, &metadata, &inode, filename);
//
//    return LOFS_GETFILE_SUCCESSFUL;
//}

/**
 * @brief 删除设备分区中的指定文件
 *
 * @param device 设备对象
 * @param filename 欲删除的文件
 *
 * @return 错误号，具体取值参见错误号列表
 */
PUBLIC u8 lofsRemoveFile(Device device, const char* filename){
    FileItem fileItem;

    Metadata metadata;
    readMetadata(device, &metadata);

    // 如果指定文件在设备分区根目录下不存在，则返回错误信息
    if ( !findFileItem(device, &metadata, filename, &fileItem) ) {
        return LOFS_REMOVEFILE_FILE_NOT_EXIST;
    }

    // 获取文件项对应的i-node
    IndexNode inode;
    readIndexNode(device, &metadata, fileItem.indexNode, &inode);

    // 释放文件项和索引节点
    releaseFileItem(device, &metadata, filename);
    releaseIndexNode(device, &metadata, fileItem.indexNode);

    return LOFS_REMOVEFILE_SUCCESSFUL;
}

/**
 * @brief 初始化文件系统元数据，并将信息写入设备
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 *
 * @return 无返回值
 */
PRIVATE void initMetadata(Device device, Metadata* metadata){
    fillMetadata(metadata);
    writeMetadata(device, metadata);
    //printMetadata(metadata);
}

/**
 * @brief 初始化空闲块映射表，并将信息写入设备
 *        清除原有空闲块位图并写入新位图
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 *
 * @return 无返回值
 */
PRIVATE void initBlockFreeArea(Device device, Metadata* metadata){
    seekToPartitionBlock(device, metadata->blockFreeStart);
    clearBlock(device, metadata->blockFreeBlocks);
    initBlockBitmap(device, metadata);
}

/**
 * @brief 初始化空闲i-node映射表，并将信息写入设备
 *        清除原有空闲i-node位图并写入新位图
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 *
 * @return 无返回值
 */
PRIVATE void initIndexNodeFreeArea(Device device, Metadata* metadata){
    seekToPartitionBlock(device, metadata->inodeFreeStart);
    clearBlock(device, metadata->inodeFreeBlocks);
    initIndexNodeBitmap(device, metadata);
}

/**
 * @brief 初始化i-node列表，并将信息写入设备
 *        清除原有i-node列表并写入新列表
 *        目前只写入根目录i-node
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 *
 * @return 无返回值
 */
PRIVATE void initIndexNodeArea(Device device, Metadata* metadata){
    seekToPartitionBlock(device, metadata->inodeStart);
    clearBlock(device, metadata->inodeBlocks);
    initRootIndexNode(device, metadata);
}

/**
 * @brief 初始化数据区域，并将信息写入设备
 *        不是完全格式化，所以不用清除数据
 *        初始化根目录文件内容
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 *
 * @return 无返回值
 */
PRIVATE void initDataArea(Device device, Metadata* metadata){
    seekToPartitionBlock(device, metadata->dataStart);
    initRootFile(device, metadata);
}

/**
 * @brief 初始化并填充分区元数据结构体
 *
 * @param metadata 分区元数据结构体指针
 *
 * @return  无返回值
 */
PRIVATE void fillMetadata(Metadata* metadata){
    // 目前的文件系统版本号是1
    metadata->version = 1;
    metadata->metaBlocks = 1;
    // 目前用在软盘上，所以分区总大小是DISK_SIZE - BLOCK_BYTES（减去MBR占用的块）
    metadata->totalSize = DISK_SIZE - BLOCK_BYTES;
    metadata->totalBlocks = CEIL_DIV(metadata->totalSize, BLOCK_BYTES);
    metadata->blockFreeStart = 1;
    metadata->blockFreeBlocks = CEIL_DIV(metadata->totalBlocks, BLOCK_BYTES * BYTE_BITS);

    metadata->inodeCount = 256;
    metadata->inodeFreeStart = metadata->blockFreeStart + metadata->blockFreeBlocks;
    metadata->inodeFreeBlocks = CEIL_DIV(metadata->inodeCount, BLOCK_BYTES * BYTE_BITS);
    metadata->inodeStart = metadata->inodeFreeStart + metadata->inodeFreeBlocks;
    metadata->inodeBlocks = CEIL_DIV(INODE_COUNT * sizeof(IndexNode), BLOCK_BYTES);
    
    metadata->dataStart = metadata->inodeStart + metadata->inodeBlocks;
    metadata->dataBlocks = metadata->totalBlocks - metadata->dataStart;
}

///**
// * @brief 输出分区元数据
// *
// * @param metadata 分区元数据结构体指针
// *
// * @return 无返回值
// */
//PRIVATE void printMetadata(Metadata* metadata){
//    printf(" LOFS Metadata\n" );
//    printf(" version: %d\n" , metadata->version);
//    printf(" metaBlocks: %d\n" , metadata->metaBlocks);
//    printf(" totalSize: %d\n" , metadata->totalSize);
//    printf(" totalBlocks: %d\n" , metadata->totalBlocks);
//    printf(" blockFreeStart: %d\n" , metadata->blockFreeStart);
//    printf(" blockFreeBlocks: %d\n" , metadata->blockFreeBlocks);
//    printf(" inodeCount: %d\n" , metadata->inodeCount);
//    printf(" inodeFreeStart: %d\n" , metadata->inodeFreeStart);
//    printf(" inodeFreeBlocks: %d\n" , metadata->inodeFreeBlocks);
//    printf(" inodeStart: %d\n" , metadata->inodeStart);
//    printf(" inodeBlocks: %d\n" , metadata->inodeBlocks);
//    printf(" dataStart: %d\n" , metadata->dataStart);
//    printf(" dataBlocks: %d\n" , metadata->dataBlocks);
//}

/**
 * @brief 将分区元数据写入设备
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 *
 * @return 无返回值
 */
PRIVATE void writeMetadata(Device device, Metadata* metadata){
    seekToPartitionBlock(device, 0);
    writeU8(device, metadata->version);
    writeU16(device, metadata->metaBlocks);
    writeU32(device, metadata->totalSize);
    writeU16(device, metadata->totalBlocks);
    writeU16(device, metadata->blockFreeStart);
    writeU16(device, metadata->blockFreeBlocks);
    writeU16(device, metadata->inodeCount);
    writeU16(device, metadata->inodeFreeStart);
    writeU16(device, metadata->inodeFreeBlocks);
    writeU16(device, metadata->inodeStart);
    writeU16(device, metadata->inodeBlocks);
    writeU16(device, metadata->dataStart);
    writeU16(device, metadata->dataBlocks);
}

/**
 * @brief 从设备中读取分区元数据
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 *
 * @return 无返回值
 */
PRIVATE void readMetadata(Device device, Metadata* metadata){
//	disp_str(" readMetadata ");
//	disp_int((void*)readMetadata);
    seekToPartitionBlock(device, 0);
    readU8(device, &metadata->version);
    readU16(device, &metadata->metaBlocks);
    readU32(device, &metadata->totalSize);
    readU16(device, &metadata->totalBlocks);
    readU16(device, &metadata->blockFreeStart);
    readU16(device, &metadata->blockFreeBlocks);
    readU16(device, &metadata->inodeCount);
    readU16(device, &metadata->inodeFreeStart);
    readU16(device, &metadata->inodeFreeBlocks);
    readU16(device, &metadata->inodeStart);
    readU16(device, &metadata->inodeBlocks);
    readU16(device, &metadata->dataStart);
    readU16(device, &metadata->dataBlocks);
}

/**
 * @brief 获取分区剩余空闲块数量
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 *
 * @return 分区剩余空闲块数量
 */
PRIVATE Size getRemainingBlocks(Device device, Metadata* metadata){
    Size blockIndex = 0;
    Size freeBlocks = 0;
    for ( blockIndex = 0; blockIndex < metadata->totalBlocks; blockIndex ++ ) {
        if ( !isBlockUsed(device, metadata, blockIndex) ) {
            freeBlocks ++;
        }
    }

    return freeBlocks;
}

/**
 * @brief 获取分区剩余空间大小（字节数）
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 *
 * @return 分区剩余空间大小（字节数）
 */
PRIVATE Size getRemainingSpace(Device device, Metadata* metadata){
    return getRemainingBlocks(device, metadata) * BLOCK_BYTES;
}

/**
 * @brief 获取分区剩余i-node数量
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 *
 * @return 分区剩余i-node数量
 */
PRIVATE Size getRemainingIndexNodes(Device device, Metadata* metadata){
    Size inodeIndex = 0;
    Size freeIndexNodes = 0;
    for ( inodeIndex = 0; inodeIndex < metadata->inodeCount; inodeIndex ++ ) {
        if ( !isIndexNodeUsed(device, metadata, inodeIndex) ) {
            freeIndexNodes ++;
        }
    }

    return freeIndexNodes;
}

///**
// * @brief 获取设备元数据和其他的统计信息
// *
// * @param device 设备对象
// *
// * @return 无返回值
// */
//PUBLIC void lofsShowMetadata(Device device){
//    Metadata metadata;
//    readMetadata(device, &metadata);
//
//    // 显示元数据
//    printMetadata(&metadata);
//
//    // 显示更多统计信息
//    printf(" \nMore information:\n" );
//    // 剩余块数
//    printf(" Remaining blocks: %ld\n" , getRemainingBlocks(device, &metadata));
//    // 剩余空间
//    printf(" Remaining space: %ld\n" , getRemainingSpace(device, &metadata));
//    // 剩余i-node数
//    printf(" Remaining i-nodes: %ld\n" , getRemainingIndexNodes(device, &metadata));
//}

///**
// * @brief 列出设备分区根目录下的所有文件（包括当前目录和上层目录）
// *        现在只能列出根目录下的32个文件（暂时没有需求超出那么多文件，所以就不写了 = =）
// *
// * @param device 设备对象
// *
// * @return 无返回值
// */
//PUBLIC void lofsListFiles(Device device){
//    Metadata metadata;
//    int fileItemIndex = 0;
//
//    readMetadata(device, &metadata);
//    // 直接移动到根目录数据区，根目录第一个数据块肯定是第分区一个数据块
//    seekToPartitionBlock(device, metadata.dataStart);
//    printf(" Filename\tIndexNode\n=========================\n" );
//    // 遍历根目录的第一个数据块，每个FileItem 16字节，一个数据块512字节，因此最多只有32个文件项
//    for ( fileItemIndex = 0; fileItemIndex < 32; fileItemIndex ++ ) {
//        FileItem fileItem;
//        readFileItem(device, &fileItem);
//
//        if ( fileItem.filename[0] ) {
//            printf(" %-16s%d\n" , fileItem.filename, fileItem.indexNode);
//        }
//    }
//}

///**
// * @brief 将指定的本地文件存放到设备分区中
// *        会在分区根目录下生成同名文件，并复制本地文件数据
// *
// * @param device 设备对象
// * @param filename 本地文件路径
// *
// * @return 错误号，取值参见错误号列表
// */
//PUBLIC u8 lofsPutFile(Device device, const char *filename){
//    // 如果本地文件不存在，则返回错误信息
//    if ( !isLocalFileExist(filename) ) {
//        return LOFS_PUTFILE_LOCALFILE_NOT_EXIST;
//    }
//
//    FileItem fileItem;
//
//    Metadata metadata;
//    readMetadata(device, &metadata);
//
//    // 如果该文件在分区中已存在，则返回错误信息
//    if ( findFileItem(device, &metadata, filename, &fileItem) ) {
//        return LOFS_PUTFILE_FILE_EXIST;
//    }
//
//    // 获取并占用一个空闲i-node
//    u16 inodeIndex = occupyAvaliableIndexNode(device, &metadata);
//
//    // 初始化i-node，复制数据并将初始文件链接数设置成1，写入i-node
//    IndexNode inode;
//    initIndexNode(&inode);
//    copyFileContentFromLocalFile(device, &metadata, &inode, filename);
//    inode.link = 1;
//    writeIndexNode(device, &metadata, inodeIndex, &inode);
//    
//    // 初始化文件项，并添加到根目录下
//    fillFileItem(&fileItem, filename, inodeIndex);
//    appendFileItem(device, &metadata, &fileItem);
//
//    return LOFS_PUTFILE_SUCCESSFUL;
//}

///**
// * @brief 获取指定设备分区上的文件
// *        会在当前目录中生成同名文件，并复制分区中的文件数据
// *
// * @param device 设备对象
// * @param filename 文件名
// *
// * @return 错误号，取值参见错误号列表
// */
//PUBLIC u8 lofsGetFile(Device device, const char* filename){
//    FileItem fileItem;
//
//    Metadata metadata;
//    readMetadata(device, &metadata);
//
//    // 如果指定文件在设备分区根目录下不存在，则返回错误信息
//    if ( !findFileItem(device, &metadata, filename, &fileItem) ) {
//        return LOFS_GETFILE_FILE_NOT_EXIST;
//    }
//
//    // 获取文件项对应的i-node
//    IndexNode inode;
//    readIndexNode(device, &metadata, fileItem.indexNode, &inode);
//
//    // 将分区文件内容拷贝到本地文件
//    copyFileContentToLocalFile(device, &metadata, &inode, filename);
//
//    return LOFS_GETFILE_SUCCESSFUL;
//}



///**
// * @brief 从设备中读取分区元数据
// *
// * @param device 设备对象
// * @param metadata 分区元数据结构体指针
// *
// * @return 无返回值
// */
//PRIVATE void readMetadata(Device device, Metadata* metadata){
//    seekToPartitionBlock(device, 0);
//    readU8(device, &metadata->version);
//    readU16(device, &metadata->metaBlocks);
//    readU32(device, &metadata->totalSize);
//    readU16(device, &metadata->totalBlocks);
//    readU16(device, &metadata->blockFreeStart);
//    readU16(device, &metadata->blockFreeBlocks);
//    readU16(device, &metadata->inodeCount);
//    readU16(device, &metadata->inodeFreeStart);
//    readU16(device, &metadata->inodeFreeBlocks);
//    readU16(device, &metadata->inodeStart);
//    readU16(device, &metadata->inodeBlocks);
//    readU16(device, &metadata->dataStart);
//    readU16(device, &metadata->dataBlocks);
//}

/**
 * @brief 初始化空闲块映射表位图
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 *
 * @return 无返回值
 */
PRIVATE void initBlockBitmap(Device device, Metadata* metadata){
    Size blockIndex = 0;
    // Because we need a data block to store root directory
    // so endBlockIndex is dataStart + 1
    Size endBlockIndex = metadata->dataStart + 1;
    for ( blockIndex = 0; blockIndex < endBlockIndex; blockIndex ++ ) {
        setBlockUsed(device, metadata, blockIndex);
    }
}

/**
 * @brief 初始化空闲inode映射表位图
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 *
 * @return  无返回值
 */
PRIVATE void initIndexNodeBitmap(Device device, Metadata* metadata){
    setIndexNodeUsed(device, metadata, ROOT_INODE_INDEX);
}

/**
 * @brief 初始化根目录i-node，根目录的i-node是第0个i-node，并写入设备
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 *
 * @return 无返回值
 */
PRIVATE void initRootIndexNode(Device device, Metadata* metadata){
    IndexNode inodeOfRoot;
    fillRootIndexNode(metadata, &inodeOfRoot);
    writeIndexNode(device, metadata, 0, &inodeOfRoot);
}

/**
 * @brief 初始化根目录i-node，填充i-node结构体数据
 *
 * @param metadata 分区元数据结构体指针
 * @param inodeOfRoot 根目录i-node结构体指针
 *
 * @return 无返回值
 */
PRIVATE void fillRootIndexNode(Metadata* metadata, IndexNode* inodeOfRoot){
    initIndexNode(inodeOfRoot);
    // 根目录文件是个目录
    inodeOfRoot->fileType = INODE_FILETYPE_DIRECTORY;
    inodeOfRoot->link = 1;
    // 目前只有一个块，文件大小为块大小
    inodeOfRoot->size = BLOCK_BYTES;
    inodeOfRoot->directBlockIndex[0] = metadata->dataStart;
}

/**
 * @brief 初始化根目录中的文件项，并写入设备
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 *
 * @return 无返回值
 */
PRIVATE void initRootFile(Device device, Metadata* metadata){
    FileItem fileItem;

    // .，表示当前目录，根目录的当前目录是根目录自身
    fillRootFileItem(&fileItem);
    writeFileItem(device, &fileItem);

    // ..，表示上层目录，根目录的上层目录是根目录自身
    fillRootParentFileItem(&fileItem);
    writeFileItem(device, &fileItem);
}

/**
 * @brief 填充根目录.文件文件项，代表当前目录
 *
 * @param rootFileItem 文件项结构体指针
 *
 * @return 无返回值
 */
PRIVATE void fillRootFileItem(FileItem* rootFileItem){
    fillFileItem(rootFileItem, " ." , 0);
}

/**
 * @brief 填充根目录..文件文件项，代表父目录
 *
 * @param rootParentFileItem 文件项结构体指针
 *
 * @return 无返回值
 */
PRIVATE void fillRootParentFileItem(FileItem* rootParentFileItem){
    fillFileItem(rootParentFileItem, " .." , 0);
}



PRIVATE void copyFileContentToBuf(Device device, Metadata *metadata, 
        IndexNode *inode, char* buf, Size size){
	Size remainingSize = size;
	
	char** pbuf = &buf;
	if ( remainingSize ) {
	    copyContentToBufInDirectIndex(device, pbuf, 
	            inode->directBlockIndex, INODE_DBIDX_COUNT, &remainingSize);
	}
	if ( remainingSize ) {
	    copyContentToBufInL1IndirectIndex(device, pbuf, 
	            inode->l1IndirectBlockIndex, INODE_1IBIDX_COUNT, &remainingSize);
	}
	if ( remainingSize ) {
	    copyContentToBufInL2IndirectIndex(device, pbuf, 
	            inode->l2IndirectBlockIndex, INODE_2IBIDX_COUNT, &remainingSize);
	}
}

/**
 * @brief 将指定i-node设置为已使用状态
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param inodeIndex i-node编号
 *
 * @return 无返回值
 */
PRIVATE void setIndexNodeUsed(Device device, Metadata* metadata, u16 inodeIndex){
    seekToPartitionBlock(device, metadata->inodeFreeStart);
    Size byteOffset = inodeIndex / BYTE_BITS;
    seekDevice(device, DEV_SEEK_CURRENT, byteOffset);
    Size currentDeviceOffset = deviceOffset(device);

    u8 bitmap = 0;
    readU8(device, &bitmap);
    Size bitOffset = inodeIndex % BYTE_BITS;
    bitmap |= 0x80 >> bitOffset;

    seekDevice(device, DEV_SEEK_BEGIN, currentDeviceOffset);
    writeU8(device, bitmap);
}

/**
 * @brief 将指定i-node设置为空闲状态（未使用）
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param inodeIndex i-node编号
 *
 * @return 无返回值
 */
PRIVATE void resetIndexNodeUsed(Device device, Metadata* metadata, u16 inodeIndex){
    seekToPartitionBlock(device, metadata->inodeFreeStart);
    Size byteOffset = inodeIndex / BYTE_BITS;
    seekDevice(device, DEV_SEEK_CURRENT, byteOffset);
    Size currentDeviceOffset = deviceOffset(device);

    u8 bitmap = 0;
    readU8(device, &bitmap);
    Size bitOffset = inodeIndex % BYTE_BITS;
    bitmap &= ~(0x80 >> bitOffset);

    seekDevice(device, DEV_SEEK_BEGIN, currentDeviceOffset);
    writeU8(device, bitmap);
}

/**
 * @brief 获取指定i-node的使用情况
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param inodeIndex i-node编号
 *
 * @return 如果该i-node被使用，返回1，没有使用则返回0
 */
PRIVATE u8 isIndexNodeUsed(Device device, Metadata* metadata, u16 inodeIndex){
    seekToPartitionBlock(device, metadata->inodeFreeStart);
    Size byteOffset = inodeIndex / BYTE_BITS;
    seekDevice(device, DEV_SEEK_CURRENT, byteOffset);

    u8 bitmap = 0;
    readU8(device, &bitmap);
    Size bitOffset = inodeIndex % BYTE_BITS;
    return bitmap & (0x80 >> bitOffset);
}

/**
 * @brief 找到一个空闲i-node并占用该i-node
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 *
 * @return 返回占用i-node编号
 */
PRIVATE u16 occupyAvaliableIndexNode(Device device, Metadata* metadata){
    Size inodeIndex = 0;
    for ( inodeIndex = 0; inodeIndex < metadata->inodeCount; inodeIndex ++ ) {
        if ( !isIndexNodeUsed(device, metadata, inodeIndex) ) {
            setIndexNodeUsed(device, metadata, inodeIndex);
            return inodeIndex;
        }
    }

    return 0;
}

/**
 * @brief 初始化i-node结构体
 *
 * @param inode 待初始化的i-node结构体指针
 *
 * @return 无返回值
 */
PRIVATE void initIndexNode(IndexNode *inode){
    memset(inode, 0, sizeof(IndexNode));

    inode->fileType = INODE_FILETYPE_NORMAL;
    inode->link = 0;
    inode->uid = ROOT_USER_ID;
    inode->gid = ROOT_GROUP_ID;
    inode->size = 0;
    inode->createTime = getTicks();
    inode->modifiedTime = inode->createTime;
}

/**
 * @brief 从分区中读取指定的i-node
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param inodeIndex 欲读取的i-node编号
 * @param inode i-node结构体指针
 *
 * @return 无返回值
 */
PRIVATE void readIndexNode(Device device, Metadata* metadata, u16 inodeIndex, IndexNode* inode){
    seekToPartitionBlock(device, metadata->inodeStart);
    seekDevice(device, DEV_SEEK_CURRENT, INODE_SIZE * inodeIndex);

    readU8(device, &inode->fileType);
    readU8(device, &inode->link);
    readU16(device, &inode->uid);
    readU16(device, &inode->gid);
    readU32(device, &inode->size);
    readU32(device, &inode->createTime);
    readU32(device, &inode->modifiedTime);
    readDevice(device, inode->directBlockIndex, sizeof(u16) * INODE_DBIDX_COUNT);
    readDevice(device, inode->l1IndirectBlockIndex, sizeof(u16) * INODE_1IBIDX_COUNT);
    readDevice(device, inode->l2IndirectBlockIndex, sizeof(u16) * INODE_2IBIDX_COUNT);
}

/**
 * @brief 将指定的i-node写入设备中
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param inodeIndex 欲写入设备的目的i-node编号
 * @param inode 欲写入设备的i-node结构体指针
 *
 * @return 无返回值
 */
PRIVATE void writeIndexNode(Device device, Metadata* metadata, u16 inodeIndex, IndexNode* inode){
    seekToPartitionBlock(device, metadata->inodeStart);
    seekDevice(device, DEV_SEEK_CURRENT, INODE_SIZE * inodeIndex);

    writeU8(device, inode->fileType);
    writeU8(device, inode->link);
    writeU16(device, inode->uid);
    writeU16(device, inode->gid);
    writeU32(device, inode->size);
    writeU32(device, inode->createTime);
    writeU32(device, inode->modifiedTime);
    writeDevice(device, inode->directBlockIndex, sizeof(u16) * INODE_DBIDX_COUNT);
    writeDevice(device, inode->l1IndirectBlockIndex, sizeof(u16) * INODE_1IBIDX_COUNT);
    writeDevice(device, inode->l2IndirectBlockIndex, sizeof(u16) * INODE_2IBIDX_COUNT);
}

/**
 * @brief 释放指定i-node，并释放其占据的所有数据块
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param inodeIndex 欲释放的i-node编号
 *
 * @return 无返回值
 */
PRIVATE void releaseIndexNode(Device device, Metadata *metadata, u16 inodeIndex){
    IndexNode indexNode;
    readIndexNode(device, metadata, inodeIndex, &indexNode);
    releaseIndexNodeBlocks(device, metadata, &indexNode);

    memset(&indexNode, 0, sizeof(IndexNode));
    writeIndexNode(device, metadata, inodeIndex, &indexNode);
    resetIndexNodeUsed(device, metadata, inodeIndex);
}

/**
 * @brief 释放i-node占用的所有数据块
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param indexNode i-node结构体指针
 *
 * @return 无返回值
 */
PRIVATE void releaseIndexNodeBlocks(Device device, Metadata *metadata, IndexNode *indexNode){
    releaseDirectBlocks(device, metadata, indexNode->directBlockIndex, INODE_DBIDX_COUNT);
    releaseL1IndirectBlocks(device, metadata, indexNode->l1IndirectBlockIndex, INODE_1IBIDX_COUNT);
    releaseL2IndirectBlocks(device, metadata, indexNode->l2IndirectBlockIndex, INODE_2IBIDX_COUNT);
}

/**
 * @brief 根据文件名和文件项引用的i-node填充文件项
 *
 * @param fileItem 文件项结构体指针
 * @param filename 文件名
 * @param inodeIndex 引用i-node编号
 *
 * @return 无返回值
 */
PRIVATE void fillFileItem(FileItem* fileItem, const char* const filename, u16 inodeIndex){
    memset(fileItem->filename, 0, sizeof(fileItem->filename));
    strncpy(fileItem->filename, filename, FILEITEM_FILENAME_MAX_SIZE);
    fileItem->indexNode = inodeIndex;
}

/**
 * @brief 将文件项写入设备当前位置
 *
 * @param device 设备对象
 * @param fileItem 文件项结构体指针
 *
 * @return 无返回值
 */
PRIVATE void writeFileItem(Device device, FileItem* fileItem){
    writeDevice(device, fileItem->filename, FILEITEM_FILENAME_MAX_SIZE);
    writeU16(device, fileItem->indexNode);
}

/**
 * @brief 读取设备当前位置的文件项
 *
 * @param device 设备对象
 * @param fileItem 文件项结构体指针
 *
 * @return 无返回值
 */
PRIVATE void readFileItem(Device device, FileItem* fileItem){
    readDevice(device, fileItem->filename, FILEITEM_FILENAME_MAX_SIZE);
    fileItem->filename[FILEITEM_FILENAME_MAX_SIZE] = '\0';
    readU16(device, &fileItem->indexNode);
}

/**
 * @brief 在根目录下添加新文件项
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param fileItem 文件项结构体指针
 *
 * @return 无返回值
 */
PRIVATE void appendFileItem(Device device, Metadata* metadata, FileItem* fileItem){
    seekToPartitionBlock(device, metadata->dataStart);
    int fileItemIndex = 0;
    for ( fileItemIndex = 0; fileItemIndex < 32; fileItemIndex ++ ) {
        FileItem originFileItem;
        Size fileItemOffset = deviceOffset(device);
        readFileItem(device, &originFileItem);

        if ( !originFileItem.filename[0] ) {
            seekDevice(device, DEV_SEEK_BEGIN, fileItemOffset);
            writeFileItem(device, fileItem);
            break;
        }
    }
}

/**
 * @brief 在根目录中查找文件项
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param filename 欲查找的文件名
 * @param fileItem 文件项结构体指针
 *
 * @return 如果文件存在，返回1，否则返回0
 */
PRIVATE u8 findFileItem(Device device, Metadata* metadata, const char* filename, FileItem* fileItem){
    seekToPartitionBlock(device, metadata->dataStart);
//    disp_str(" findFileItem filename: " );
//    disp_str(filename);
    int fileItemIndex = 0;
    for ( fileItemIndex = 0; fileItemIndex < 32; fileItemIndex ++ ) {
        FileItem originFileItem;
        readFileItem(device, &originFileItem);

        if ( strcmp(filename, originFileItem.filename) == 0 ) {
            memcpy(fileItem, &originFileItem, sizeof(FileItem));
            return 1;
        }
    }

    return 0;
}

/**
 * @brief 释放指定的文件项，重置指定文件项内容
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param filename 欲释放的文件文件名
 *
 * @return 无返回值
 */
PRIVATE void releaseFileItem(Device device, Metadata* metadata, const char *filename){
    seekToPartitionBlock(device, metadata->dataStart);
    int fileItemIndex = 0;
    for ( fileItemIndex = 0; fileItemIndex < 32; fileItemIndex ++ ) {
        FileItem originFileItem;
        Size fileItemOffset = deviceOffset(device);
        readFileItem(device, &originFileItem);

        if ( strcmp(originFileItem.filename, filename) == 0 ) {
            seekDevice(device, DEV_SEEK_BEGIN, fileItemOffset);
            memset(&originFileItem, 0, sizeof(originFileItem));
            writeFileItem(device, &originFileItem);
        }
    }
}

/**
 * @brief 将指定块设置为已使用状态
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param blockIndex 块编号
 *
 * @return 无返回值
 */
PRIVATE void setBlockUsed(Device device, Metadata* metadata, u16 blockIndex){
    seekToPartitionBlock(device, metadata->blockFreeStart);
    Size byteOffset = blockIndex / BYTE_BITS;
    seekDevice(device, DEV_SEEK_CURRENT, byteOffset);
    Size currentDeviceOffset = deviceOffset(device);

    u8 bitmap = 0;
    readU8(device, &bitmap);
    Size bitOffset = blockIndex % BYTE_BITS;
    bitmap |= 0x80 >> bitOffset;

    seekDevice(device, DEV_SEEK_BEGIN, currentDeviceOffset);
    writeU8(device, bitmap);
}

/**
 * @brief 将指定块设置为空闲（未使用）状态
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param blockIndex 块编号
 *
 * @return 无返回值
 */
PRIVATE void resetBlockUsed(Device device, Metadata* metadata, u16 blockIndex){
    seekToPartitionBlock(device, metadata->blockFreeStart);
    Size byteOffset = blockIndex / BYTE_BITS;
    seekDevice(device, DEV_SEEK_CURRENT, byteOffset);
    Size currentDeviceOffset = deviceOffset(device);

    u8 bitmap;
    readU8(device, &bitmap);
    Size bitOffset = blockIndex % BYTE_BITS;
    bitmap &= ~(0x80 >> bitOffset);

    seekDevice(device, DEV_SEEK_BEGIN, currentDeviceOffset);
    writeU8(device, bitmap);
}

/**
 * @brief 获取指定块的使用状况
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param blockIndex 块编号
 *
 * @return 如果该块已使用，返回1
 */
PRIVATE u8 isBlockUsed(Device device, Metadata* metadata, u16 blockIndex){
    seekToPartitionBlock(device, metadata->blockFreeStart);
    Size byteOffset = blockIndex / BYTE_BITS;
    seekDevice(device, DEV_SEEK_CURRENT, byteOffset);

    u8 bitmap;
    readU8(device, &bitmap);
    Size bitOffset = blockIndex % BYTE_BITS;
    return bitmap & (0x80 >> bitOffset);
}

/**
 * @brief 找到一个空闲块并占用该块
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 *
 * @return 空闲块编号
 */
PRIVATE u16 occupyAvaliableBlock(Device device, Metadata* metadata){
    Size blockIndex = 0;
    Size freeBlocks = 0;
    for ( blockIndex = 0; blockIndex < metadata->totalBlocks; blockIndex ++ ) {
        if ( !isBlockUsed(device, metadata, blockIndex) ) {
            setBlockUsed(device, metadata, blockIndex);
            return blockIndex;
        }
    }

    return 0;
}

/**
 * @brief 将块索引存放到文件i-node中，视情况放在直接索引或者简介索引块中
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param inode i-node结构体指针
 * @param nextBlock 块编号
 *
 * @return 无返回值
 */
PRIVATE void storeBlockIndex(Device device, Metadata* metadata, IndexNode* inode, u16 nextBlock){
    if ( storeBlockIndexInDirectList(inode->directBlockIndex, INODE_DBIDX_COUNT, nextBlock) ) {
        return;
    }
    if ( storeBlockIndexInL1IndirectList(device, metadata, 
            inode->l1IndirectBlockIndex, INODE_1IBIDX_COUNT, nextBlock) ) {
        return;
    }
    if ( storeBlockIndexInL2IndirectList(device, metadata, 
            inode->l2IndirectBlockIndex, INODE_2IBIDX_COUNT, nextBlock) ) {
        return;
    }

    return;
}

/**
 * @brief 将块索引存放在直接块索引表中
 *
 * @param directBlockIndex 直接块索引表起始指针
 * @param indexCount 直接块索引数量
 * @param nextBlock 块编号
 *
 * @return 如果成功返回1，失败返回0
 */
PRIVATE u8 storeBlockIndexInDirectList(u16* directBlockIndex, u16 indexCount, u16 nextBlock){
    int index = 0;
    for ( index = 0; index < indexCount; index ++ ) {
        if ( !directBlockIndex[index] ) {
            directBlockIndex[index] = nextBlock;
            return 1;
        }
    }

    return 0;
}

/**
 * @brief 将块索引存放在一级间接块索引表中，并写入设备
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param l1IndirectBlockIndex 一级间接块索引表起始指针
 * @param indexCount 一级间接块索引数量
 * @param nextBlock 块编号
 *
 * @return 如果成功返回1，失败返回0
 */
PRIVATE u8 storeBlockIndexInL1IndirectList(
        Device device, Metadata* metadata, u16* l1IndirectBlockIndex, u16 indexCount, u16 nextBlock){
    int index = 0;
    u16 directIndexBlock = 0;
    // Find the avaliable position to store block index of directIndexBlock
    for ( index = 0; index < indexCount; index ++ ) {
        if ( !l1IndirectBlockIndex[index] ) {
            directIndexBlock = occupyAvaliableBlock(device, metadata);
            l1IndirectBlockIndex[index] = directIndexBlock;
            seekToPartitionBlock(device, directIndexBlock);
            clearBlock(device, 1);
            break;
        }
        else {
            if ( !isIndexBlockFull(device, metadata, l1IndirectBlockIndex[index]) ) {
                directIndexBlock = l1IndirectBlockIndex[index];
                break;
            }
        }
    }

    if ( !directIndexBlock ) {
        return 0;
    }

    // Read block, store index of nextBlock, write back
    u16 directBlockIndex[BLOCK_BYTES / sizeof(u16)];
    seekToPartitionBlock(device, directIndexBlock);
    readDevice(device, directBlockIndex, BLOCK_BYTES);

    storeBlockIndexInDirectList(directBlockIndex, 
            BLOCK_BYTES / sizeof(u16), nextBlock);

    seekToPartitionBlock(device, directIndexBlock);
    writeDevice(device, directBlockIndex, BLOCK_BYTES);

    return 1;
}

/**
 * @brief 将块索引存放在二级间接块索引表中，并写入设备
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param l2IndirectBlockIndex 二级间接块索引表起始指针
 * @param indexCount 二级间接块索引数量
 * @param nextBlock 块编号
 *
 * @return 如果成功返回1，失败返回0
 */
PRIVATE u8 storeBlockIndexInL2IndirectList(
        Device device, Metadata* metadata, u16* l2IndirectBlockIndex, u16 indexCount, u16 nextBlock){
    int index = 0;
    u16 l1IndirectIndexBlock = 0;
    for ( index = 0; index < indexCount; index ++ ) {
        if ( !l2IndirectBlockIndex[index] ) {
            l1IndirectIndexBlock = occupyAvaliableBlock(device, metadata);
            l2IndirectBlockIndex[index] = l1IndirectIndexBlock;
            seekToPartitionBlock(device, l1IndirectIndexBlock);
            clearBlock(device, 1);
            break;
        }
        else {
            if ( !isIndexBlockFull(device, metadata, l2IndirectBlockIndex[index]) ) {
                l1IndirectIndexBlock = l2IndirectBlockIndex[index];
                break;
            }
        }
    }

    if ( !l1IndirectIndexBlock ) {
        return 0;
    }

    u16 l1IndirectBlockIndex[BLOCK_BYTES / sizeof(u16)];
    seekToPartitionBlock(device, l1IndirectIndexBlock);
    readDevice(device, l1IndirectBlockIndex, BLOCK_BYTES);

    storeBlockIndexInL1IndirectList(device, metadata,
            l1IndirectBlockIndex, BLOCK_BYTES / sizeof(u16), nextBlock);
    
    seekToPartitionBlock(device, l1IndirectIndexBlock);
    writeDevice(device, l1IndirectBlockIndex, BLOCK_BYTES);

    return 1;
}

///**
// * @brief 将本地文件中的数据复制到直接块索引对应的块中，需要指定一个最大写入字节数
// *
// * @param device 设备对象
// * @param localFile 本地文件指针
// * @param directBlockIndex 直接块索引表起始指针
// * @param indexCount 直接块索引数量
// * @param remainingSize 最大需要写入的字节数
// *                      函数结束的时候会修改这个值（减去本次写入的字节数）
// *
// * @return 无返回值
// */
//PRIVATE void copyContentInDirectIndex(Device device, FILE *localFile,
//        u16 *directBlockIndex, u16 indexCount, Size *remainingSize){
//    int index = 0;
//    for ( index = 0; index < indexCount; index ++ ) {
//        if ( !*remainingSize ) {
//            break;
//        }
//        if ( directBlockIndex[index] ) {
//            copyBlockContent(device, localFile, directBlockIndex[index], remainingSize);
//        }
//    }
//}

PRIVATE void copyContentToBufInDirectIndex(Device device, char** pbuf, 
	u16 *directBlockIndex, u16 indexCount, Size *remainingSize){
	
	int index = 0;
	for(index = 0; index < indexCount; index++){
		if( !*remainingSize){
			break;
		}
		if( directBlockIndex[index]){
			copyBlockContentToBuf(device, pbuf, directBlockIndex[index], remainingSize);
			//disp_str(" copyToBufInD: " );
			//disp_str(*pbuf);
		}
	}
}

///**
// * @brief 将本地文件中的数据复制到一级间接块索引对应的块中，需要指定一个最大写入字节数
// *
// * @param device 设备对象
// * @param localFile 本地文件指针
// * @param l1IndirectBlockIndex 一级间接块索引表起始指针
// * @param indexCount 一级间接块索引数量
// * @param remainingSize 最大需要写入的字节数
// *                      函数结束的时候会修改这个值（减去本次写入的字节数）
// *
// * @return 无返回值
// */
//PRIVATE void copyContentInL1IndirectIndex(Device device, FILE *localFile,
//        u16 *l1IndirectBlockIndex, u16 indexCount, Size *remainingSize){
//    int index = 0;
//    for ( index = 0; index < indexCount; index ++ ) {
//        if ( !*remainingSize ) {
//            break;
//        }
//        if ( l1IndirectBlockIndex[index] ) {
//            u16 indexBuffer[BLOCK_BYTES / sizeof(u16)];
//            seekToPartitionBlock(device, l1IndirectBlockIndex[index]);
//            readDevice(device, indexBuffer, BLOCK_BYTES);
//            copyContentInDirectIndex(device, localFile, 
//                    indexBuffer, BLOCK_BYTES / sizeof(u16), remainingSize);
//        }
//    }
//}

PRIVATE void copyContentToBufInL1IndirectIndex(Device device, char** pbuf, 
	u16 *l1IndirectBlockIndex, u16 indexCount, Size *remainingSize){
	
	int index = 0;
	for ( index = 0; index < indexCount; index ++ ) {
	    if ( !*remainingSize ) {
	        break;
	    }
	    if ( l1IndirectBlockIndex[index] ) {
	        u16 indexBuffer[BLOCK_BYTES / sizeof(u16)];
	        seekToPartitionBlock(device, l1IndirectBlockIndex[index]);
	        readDevice(device, indexBuffer, BLOCK_BYTES);
	        copyContentToBufInDirectIndex(device, pbuf, 
	                indexBuffer, BLOCK_BYTES / sizeof(u16), remainingSize);
	    }
	}
}

///**
// * @brief 将本地文件中的数据复制到二级间接块索引对应的块中，需要指定一个最大写入字节数
// *
// * @param device 设备对象
// * @param localFile 本地文件指针
// * @param l1IndirectBlockIndex 二级间接块索引表起始指针
// * @param indexCount 二级间接块索引数量
// * @param remainingSize 最大需要写入的字节数
// *                      函数结束的时候会修改这个值（减去本次写入的字节数）
// *
// * @return 无返回值
// */
//PRIVATE void copyContentInL2IndirectIndex(Device device, FILE *localFile,
//        u16 *l2IndirectBlockIndex, u16 indexCount, Size *remainingSize){
//    int index = 0;
//    for ( index = 0; index < indexCount; index ++ ) {
//        if ( !*remainingSize ) {
//            break;
//        }
//        if ( l2IndirectBlockIndex[index] ) {
//            u16 indexBuffer[BLOCK_BYTES / sizeof(u16)];
//            seekToPartitionBlock(device, l2IndirectBlockIndex[index]);
//            readDevice(device, indexBuffer, BLOCK_BYTES);
//            copyContentInL1IndirectIndex(device, localFile, 
//                    indexBuffer, BLOCK_BYTES / sizeof(u16), remainingSize);
//        }
//    }
//}


PRIVATE void copyContentToBufInL2IndirectIndex(Device device, char** pbuf,
        u16 *l2IndirectBlockIndex, u16 indexCount, Size *remainingSize){
	int index = 0;
	for ( index = 0; index < indexCount; index ++ ) {
	    if ( !*remainingSize ) {
	        break;
	    }
	    if ( l2IndirectBlockIndex[index] ) {
	        u16 indexBuffer[BLOCK_BYTES / sizeof(u16)];
	        seekToPartitionBlock(device, l2IndirectBlockIndex[index]);
	        readDevice(device, indexBuffer, BLOCK_BYTES);
	        copyContentToBufInL1IndirectIndex(device, pbuf, 
	                indexBuffer, BLOCK_BYTES / sizeof(u16), remainingSize);
	    }
	}
}

///**
// * @brief 将本地文件数据复制到指定块中，需要指定一个最大写入字节数
// *
// * @param device 设备对象
// * @param localFile 本地文件指针
// * @param block 块编号
// * @param remainingSize 最大需要写入的字节数
// *                      函数结束的时候会修改这个值（减去本次写入的字节数）
// *
// * @return 无返回值
// */
//PRIVATE void copyBlockContent(Device device, FILE *localFile, u16 block, Size *remainingSize){
//    Size sizeToCopy = MIN(BLOCK_BYTES, *remainingSize);
//    char buffer[BLOCK_BYTES];
//    seekToPartitionBlock(device, block);
//    readDevice(device, buffer, sizeToCopy);
//    fwrite(buffer, sizeToCopy, 1, localFile);
//
//    *remainingSize -= sizeToCopy;
//}


PRIVATE void copyBlockContentToBuf(Device device, char** pbuf, u16 block, Size *remainingSize){
	Size sizeToCopy = MIN(BLOCK_BYTES, *remainingSize);
	seekToPartitionBlock(device, block);
	readDevice(device, *pbuf, sizeToCopy);

	*remainingSize -= sizeToCopy;
	*pbuf += sizeToCopy;
}


/**
 * @brief 释放直接块索引表
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param directBlockIndex 直接块索引表起始指针
 * @param indexCount 直接块索引数量
 *
 * @return 无返回值
 */
PRIVATE void releaseDirectBlocks(Device device, Metadata *metadata, 
        u16 *directBlockIndex, u16 indexCount){
    u16 index = 0;
    for ( index = 0; index < indexCount; index ++ ) {
        if ( directBlockIndex[index] ) {
            resetBlockUsed(device, metadata, directBlockIndex[index]);
        }
        else {
            break;
        }
    }
}

/**
 * @brief 释放一级间接块索引表
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param directBlockIndex 一级间接块索引表起始指针
 * @param indexCount 一级间接块索引数量
 *
 * @return 无返回值
 */
PRIVATE void releaseL1IndirectBlocks(Device device, Metadata *metadata, 
        u16 *l1IndirectBlockIndex, u16 indexCount){
    u16 index = 0;
    for ( index = 0; index < indexCount; index ++ ) {
        if ( l1IndirectBlockIndex[index] ) {
            u16 directBlockIndex[BLOCK_BYTES / sizeof(u16)];
            seekToPartitionBlock(device, l1IndirectBlockIndex[index]);
            readDevice(device, directBlockIndex, BLOCK_BYTES);

            releaseDirectBlocks(device, metadata, directBlockIndex, BLOCK_BYTES / sizeof(u16));
            resetBlockUsed(device, metadata, l1IndirectBlockIndex[index]);
        }
        else {
            break;
        }
    }
}

/**
 * @brief 释放二级间接块索引表
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param directBlockIndex 二级间接块索引表起始指针
 * @param indexCount 二级间接块索引数量
 *
 * @return 无返回值
 */
PRIVATE void releaseL2IndirectBlocks(Device device, Metadata *metadata, 
        u16 *l2IndirectBlockIndex, u16 indexCount){
    u16 index = 0;
    for ( index = 0; index < indexCount; index ++ ) {
        if ( l2IndirectBlockIndex[index] ) {
            u16 l1IndirectBlockIndex[BLOCK_BYTES / sizeof(u16)];
            seekToPartitionBlock(device, l2IndirectBlockIndex[index]);
            readDevice(device, l1IndirectBlockIndex, BLOCK_BYTES);

            releaseDirectBlocks(device, metadata, l1IndirectBlockIndex, BLOCK_BYTES / sizeof(u16));
            resetBlockUsed(device, metadata, l2IndirectBlockIndex[index]);
        }
        else {
            break;
        }
    }
}

/**
 * @brief 判断某个索引块是否已满
 *
 * @param device 设备对象
 * @param metadata 分区元数据结构体指针
 * @param block 块编号
 *
 * @return 如果索引块已满返回1，否则返回0
 */
PRIVATE u8 isIndexBlockFull(Device device, Metadata* metadata, u16 block){
    u16 blockIndex[BLOCK_BYTES / sizeof(u16)];
    seekToPartitionBlock(device, block);
    readDevice(device, blockIndex, BLOCK_BYTES);

    int index = 0;
    for ( index = 0; index < BLOCK_BYTES / sizeof(u16); index ++ ) {
        if ( !blockIndex[index] ) {
            return 0;
        }
    }

    return 1;
}

/**
 * @brief 将设备指针移动到分区中某个块的起始位置
 *
 * @param device 设备对象
 * @param blockIndex 块编号
 *
 * @return 无返回值
 */
PRIVATE void seekToPartitionBlock(Device device, u16 blockIndex){
//	disp_str(" seekToPartitionBlock ");
//	disp_int((void*) seekToPartitionBlock);
    seekToBlock(device, blockIndex + PARTITION_BEGIN_BLOCK);
//    	disp_str(" seekToPartitionBlock ");
}

/**
 * @brief 将设备指针移动到设备中某个块的起始位置
 *
 * @param device 设备对象
 * @param blockIndex 块编号
 *
 * @return 无返回值
 */
PRIVATE void seekToBlock(Device device, u16 blockIndex){
//    disp_str(" seekToBlock start ");
    Size blockOffset = blockIndex * BLOCK_BYTES;
    seekDevice(device, DEV_SEEK_BEGIN, blockOffset);
//    disp_str(" seekToBlock finish ");
}

/**
 * @brief 从当前设备指针开始清除固定数量块中的数据（全部填充0）
 *
 * @param device 设备对象
 * @param blocks 清除块数量
 *
 * @return 无返回值
 */
PRIVATE void clearBlock(Device device, u16 blocks){
    char zeroBuffer[BLOCK_BYTES];
    memset(zeroBuffer, 0, sizeof(zeroBuffer)); 

    Size writtenSize = 0;
    Size blockFreeAreaSize = blocks * BLOCK_BYTES;
    while ( writtenSize < blockFreeAreaSize ) {
        writeDevice(device, zeroBuffer, sizeof(zeroBuffer));
        writtenSize += BLOCK_BYTES;
    }
}

/**
 * @brief 将无符号8位整数写入设备当前位置
 *
 * @param device 设备对象
 * @param value 写入的值
 *
 * @return 无返回值
 */
PRIVATE void writeU8(Device device, u8 value){
    writeDevice(device, &value, sizeof(u8));
}

/**
 * @brief 将无符号16位整数写入设备当前位置
 *
 * @param device 设备对象
 * @param value 写入的值
 *
 * @return 无返回值
 */
PRIVATE void writeU16(Device device, u16 value){
    writeDevice(device, &value, sizeof(u16));
}

/**
 * @brief 将无符号32位整数写入设备当前位置
 *
 * @param device 设备对象
 * @param value 写入的值
 *
 * @return 无返回值
 */
PRIVATE void writeU32(Device device, u32 value){
    writeDevice(device, &value, sizeof(u32));
}

/**
 * @brief 从设备当前位置读取无符号8位整数
 *
 * @param device 设备对象
 * @param value 整数指针，读取结束后会将读取的值填充进去
 *
 * @return 无返回值
 */
PRIVATE void readU8(Device device, u8* value){
    readDevice(device, value, sizeof(u8));
}

/**
 * @brief 从设备当前位置读取无符号16位整数
 *
 * @param device 设备对象
 * @param value 整数指针，读取结束后会将读取的值填充进去
 *
 * @return 无返回值
 */
PRIVATE void readU16(Device device, u16* value){
    readDevice(device, value, sizeof(u16));
}

/**
 * @brief 从设备当前位置读取无符号32位整数
 *
 * @param device 设备对象
 * @param value 整数指针，读取结束后会将读取的值填充进去
 *
 * @return 无返回值
 */
PRIVATE void readU32(Device device, u32* value){
    readDevice(device, value, sizeof(u32));
}
