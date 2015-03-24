//1.读取元数据
//2.找到数据区域起始编号,将文件指针指向那个块.
//3.遍历文件项表(数据块的起始块),找到loader,读取文件i-node
//4.将文件内容拷贝到当前目录下的同名文件中

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define PARTITION_BEGIN_BLOCK 1
#define FILENAME_MAX_SIZE 14
#define INODE_SIZE 32
#define INDEX_DBIDX_COUNT 4
#define INDEX_L1IDBIDX_COUNT 2
#define INDEX_L2IDBIDX_COUNT 1
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define TRUE 1
#define FALSE 0
#define bool char

typedef struct
{
	u8 version;
	u16 metaBlock;
	u32 totalSize;
	u16 totalBlock;
	u16 bfreeStart;
	u16 bfreeBlock;
	u16 iCount;
	u16 ifreeStart;
	u16 ifreeBlock;
	u16 istart;
	u16 iblock;
	u16 dataStart;
	u16 dataBlock;	
}MetaData;

typedef struct 
{
	u8 fileType;
	u8 link;
	u16 uid;
	u16 gid;
	u32 size;
	u32 cTime;
	u32 mTime;
	u16 directBlockIndex[INDEX_DBIDX_COUNT];
	u16 l1IndirectBlockIndex[INDEX_L1IDBIDX_COUNT];
	u16 l2IndirectBlockIndex[INDEX_L2IDBIDX_COUNT];
}INode;

 void seekToPartationBlock(FILE* device,  u16 blockIndex);
void getMetaData(FILE* device, MetaData* metaData);
void getInode(FILE* device, MetaData* metaData, INode* inode, u16 inodeNo);
bool findFile(FILE* device, MetaData* metaData, char* fileName, u16* inodeNo);
void copyFileToLocal(FILE* device, INode* inode, char* localFileName);
void copyContentInDirectIndex(FILE* device, FILE* localFile, u16 * directBlockIndex, u8 dbidxCount, int* remainingSize);
void copyContentInl1IndirectIndex(FILE* device, FILE* localFile, u16 * l1IndirectBlockIndex, u8 l1idbidxCount, int* remainingSize);
void copyContentInl2IndirectIndex(FILE* device, FILE* localFile, u16 * l2IndirectBlockIndex, u8 l2idbidxCount, int* remainingSize);
void copyBlockContent(FILE* device, FILE *localFile, u16 block, int* remainingSize);

int main(int argc, char** argv)
{
	if(argc != 4)
	{
		printf("usage: findLoder <the device> <the file you want to find and copy in the img> <file name you want to copy to on your host>\n");
		return 1;
	}

	//打开镜像文件
	FILE* device;
	device = fopen(argv[1], "rb");

	//取得数据区开始块号(根目录)
	MetaData metaData;
	getMetaData(device, &metaData);

	//从目录找文件，并且拷贝
	u16 fileInodeNo = 0;
	INode inode;
	if(findFile(device, &metaData, argv[2], &fileInodeNo))
	{
		getInode(device, &metaData, &inode, fileInodeNo);
		copyFileToLocal(device, &inode, argv[3]);
		printf("%s has been copied to %s.", argv[2], argv[3]);
	}
	else
		printf("%s is not exist.\n", argv[2]);

	printf("\n");
	return 0;
}

void getInode(FILE* device, MetaData* metaData,INode* inode, u16 inodeNo)
{
	seekToPartationBlock(device, metaData->istart);
	fseek(device, inodeNo * INODE_SIZE, SEEK_CUR);
	fread(&inode->fileType, sizeof(inode->fileType), 1, device);
	fread(&inode->link, sizeof(inode->link), 1, device);
	fread(&inode->uid, sizeof(inode->uid), 1, device);
	fread(&inode->gid, sizeof(inode->gid), 1, device);
	fread(&inode->size, sizeof(inode->size), 1, device);
	fread(&inode->cTime, sizeof(inode->cTime), 1, device);
	fread(&inode->mTime, sizeof(inode->mTime), 1, device);
	fread(&inode->directBlockIndex, sizeof(u16), INDEX_DBIDX_COUNT, device);
	fread(&inode->l1IndirectBlockIndex, sizeof(u16), INDEX_L1IDBIDX_COUNT, device);
	fread(&inode->l2IndirectBlockIndex, sizeof(u16), INDEX_L2IDBIDX_COUNT, device);
}

void getMetaData(FILE* device, MetaData* metaData)
{
	char block[BLOCK_SIZE];
	seekToPartationBlock(device, 0);
	fread(&metaData->version, sizeof(metaData->version), 1, device);
	fread(&metaData->metaBlock, sizeof(metaData->metaBlock), 1, device);
	fread(&metaData->totalSize, sizeof(metaData->totalSize), 1, device);
	fread(&metaData->totalBlock, sizeof(metaData->totalBlock), 1, device);
	fread(&metaData->bfreeStart, sizeof(metaData->bfreeStart), 1, device);
	fread(&metaData->bfreeBlock, sizeof(metaData->bfreeBlock), 1, device);
	fread(&metaData->iCount, sizeof(metaData->iCount), 1, device);
	fread(&metaData->ifreeStart, sizeof(metaData->ifreeStart), 1, device);
	fread(&metaData->ifreeBlock, sizeof(metaData->ifreeBlock), 1, device);
	fread(&metaData->istart, sizeof(metaData->istart), 1, device);
	fread(&metaData->iblock, sizeof(metaData->iblock), 1, device);
	fread(&metaData->dataStart, sizeof(metaData->dataStart), 1, device);
	fread(&metaData->dataBlock, sizeof(metaData->dataBlock), 1, device);
}

//blockIndex为块号，设置文件指针为(分区开始位置+blockIndex)*BLOCK_SIZE
void seekToPartationBlock(FILE* device,  u16 blockIndex)
{
	fseek(device, (PARTITION_BEGIN_BLOCK + blockIndex) * BLOCK_SIZE, SEEK_SET);
}

bool findFile(FILE* device, MetaData* metaData, char* fileName, u16* inodeNo)
{
	seekToPartationBlock(device, metaData->dataStart);
	int i;
	char buffName[FILENAME_MAX_SIZE + 1];
	for(i = 0; i < 32; i++)
	{
		fread(buffName, FILENAME_MAX_SIZE, 1, device);
		buffName[FILENAME_MAX_SIZE] = '\0';
		if(strcmp(fileName, buffName) == 0)
		{
			fread(inodeNo, sizeof(u16), 1, device);
			return TRUE;
		}
		else
			fseek(device, sizeof(u16), SEEK_CUR);
	}
	return FALSE;
}

void copyFileToLocal(FILE* device, INode* inode, char* localFileName)
{
	FILE* localFile = fopen(localFileName, "wb");
	int remainingSize = inode->size;

	if(remainingSize)
		copyContentInDirectIndex(device, localFile, inode->directBlockIndex, INDEX_DBIDX_COUNT,  &remainingSize);

	if(remainingSize)
		copyContentInl1IndirectIndex(device, localFile, inode->l1IndirectBlockIndex, INDEX_L1IDBIDX_COUNT, &remainingSize);

	if(remainingSize)
		copyContentInl2IndirectIndex(device, localFile, inode->l2IndirectBlockIndex, INDEX_L2IDBIDX_COUNT, &remainingSize);

	fclose(localFile);
}

void copyContentInl2IndirectIndex(FILE* device,FILE* localFile, u16 * l2IndirectBlockIndex, u8 l2idbidxCount, int* remainingSize)
{
	u16 buffer[BLOCK_SIZE / 2];
	int i, j;
	for(i = 0; i < l2idbidxCount; i++)
	{
		if(!* remainingSize)
			break;
		if(l2IndirectBlockIndex[i])
		{
			seekToPartationBlock(device, l2IndirectBlockIndex[j]);
			fread(buffer, 1, BLOCK_SIZE, device);
			for(j = 0; j < BLOCK_SIZE / 2; j++)
			{	
				if(!* remainingSize)
					break;
				if(buffer[j])
					copyContentInl1IndirectIndex(device, localFile, buffer + j, 1, remainingSize);
			}
		}
	}
}

void copyContentInl1IndirectIndex(FILE* device,FILE* localFile, u16 * l1IndirectBlockIndex, u8 l1idbidxCount, int* remainingSize)
{
	u16 buffer[BLOCK_SIZE / 2];
	int i, j;
	for(i = 0; i < l1idbidxCount; i++)
	{
		if(!* remainingSize)
			break;
		if(l1IndirectBlockIndex[i])
		{
			seekToPartationBlock(device, l1IndirectBlockIndex[i]);
			fread(buffer, 1, BLOCK_SIZE, device);
			for(j = 0; j < BLOCK_SIZE / 2; j++)
			{
				if(!* remainingSize)
					break;
				if(buffer[j])
					copyContentInDirectIndex(device, localFile, buffer + j, 1, remainingSize);
			}
		}
	}
}

void copyContentInDirectIndex(FILE* device,FILE* localFile, u16 * directBlockIndex, u8 dbidxCount, int* remainingSize)
{
	int i;
	for(i = 0; i < dbidxCount; i++)
	{	
		if(!*remainingSize)
			break;
		if(directBlockIndex[i])
			copyBlockContent(device, localFile, directBlockIndex[i], remainingSize);
	}
}

void copyBlockContent(FILE* device, FILE *localFile, u16 block, int* remainingSize)
{
	int sizeToCopy = (BLOCK_SIZE < *remainingSize) ? BLOCK_SIZE : *remainingSize;
	seekToPartationBlock(device, block);
	char buffer[sizeToCopy];
	fread(buffer, sizeToCopy, 1, device);
	fwrite(buffer, sizeToCopy, 1, localFile);
	*remainingSize -= sizeToCopy;
}