#include "api.h"

#define NR_SYS_GET_TICKS	0
#define NR_SYS_READ_KEY		1
#define NR_SYS_HD_OPEN		2
#define NR_SYS_BLOCK		3
#define NR_SYS_UNBLOCK		4
#define NR_SYS_HD_CLOSE		5
#define NR_SYS_HD_READ		6
#define NR_SYS_HD_WRITE		7
#define NR_SYS_HD_IOCTL		8
#define NR_SYS_FILE_OPEN	9
#define NR_SYS_FILE_READ	10
#define NR_SYS_FILE_CLOSE	11
#define NR_SYS_CREATE_PROCESS	12
#define NR_SYS_EXIT_PROCESS	13
#define NR_SYS_TTY_OUTPUT_STR	14


PUBLIC int getTicks()
{
	return system_call(NR_SYS_GET_TICKS, 0, 0, 0, 0, 0);
}

PUBLIC VirtualKey readKey()
{
	return (VirtualKey)( system_call(NR_SYS_READ_KEY, 0, 0, 0, 0, 0) );
}

PUBLIC struct hd_info* hdOpen(u32 device)
{
	return (struct hd_info*)system_call(NR_SYS_HD_OPEN, device, 0, 0, 0, 0);
}

PUBLIC void block()
{
	system_call(NR_SYS_BLOCK, 0, 0, 0, 0, 0);
}

PUBLIC void unBlock(u32 pid)
{
	system_call(NR_SYS_UNBLOCK, pid, 0, 0, 0, 0);
}

PUBLIC void hdClose(u32 device)
{
	system_call(NR_SYS_HD_CLOSE, device, 0, 0, 0, 0);
}

PUBLIC void hdRead(u32 device, u64 position, u32 count, char* buf)
{
	system_call(NR_SYS_HD_READ, device, position & 0xFFFFFFFF, (position >> 32) & 0xFFFFFFFF, count, (u32)buf);
}

PUBLIC void hdWrite(u32 device, u64 position, u32 count, char* buf)
{
	system_call(NR_SYS_HD_WRITE, device, position & 0xFFFFFFFF, (position >> 32) & 0xFFFFFFFF, count, (u32)buf);
}

PUBLIC void hdIoctl(u32 device, u32 request, char* buf)
{
	system_call(NR_SYS_HD_IOCTL, device, request, (u32)buf, 0, 0);
}

PUBLIC struct hd_info* fileOpen(int deviceNo, const char* path, FILE_MODE mode)
{
	return (struct hd_info*)system_call(NR_SYS_FILE_OPEN, deviceNo, (u32)path, mode, 0, 0);
}

PUBLIC int fileRead(int fd, char* buf, int size)
{
	return system_call(NR_SYS_FILE_READ, fd, (u32)buf, size, 0, 0);
}

PUBLIC void fileClose(int fd)
{
	system_call(NR_SYS_FILE_CLOSE, (u32)fd, 0, 0, 0, 0);
}

PUBLIC int createProcess(const char* processName)
{
	return (int)system_call(NR_SYS_CREATE_PROCESS, (u32)processName, 0, 0, 0, 0);
}

PUBLIC int exitProcess()
{
	return system_call(NR_SYS_EXIT_PROCESS, 0, 0, 0, 0, 0);
}

PUBLIC void  ttyOutputStr(const char* str)
{
	system_call(NR_SYS_TTY_OUTPUT_STR, (u32)str, 0, 0, 0, 0);
}

//====================================================================================================
MemNode* getFreeMemNode()
{
	int i = 0;
	for(i = 0; i < NR_MEM_NODE; i++)
	{
		if(freeMemNode[i].size == 0)
			return (freeMemNode + i);
	}
	return NULL;
}

MemNode* delMemNode(MemNode* node)
{
	memset(node, 0, sizeof(MemNode));
}

void insertMemList(MemList* ml, MemNode* node)
{
	node->next = ml->head;
	ml->head = node;
	ml->size += node->size;
}

MemNode* dropMenList(MemList* ml, u32 pos)
{
	MemNode* prepmn = NULL;
	MemNode* pmn = ml->head;
	while(pmn != NULL){
		if(pmn->pos == pos)
		{
			prepmn = pmn->next;
			ml->size -= pmn->size;
			return pmn;
		}
		prepmn = pmn;
		pmn = pmn->next;
	}
	return NULL;
}

void init_heap()
{
	int i = 0;
	for(i = 0; i < NR_MEM_NODE; i++){
		memset(freeMemNode + i, 0, sizeof(MemNode));
	}

	MemNode* memNode = getFreeMemNode();
	memNode->pos = 0x800000;
	memNode->size = 0x1200000 - 0x800000;
	memNode->next = NULL;

	freeMemList.head = memNode;
	freeMemList.size = memNode->size;
}

void *malloc(unsigned int num_bytes)
{
	if(freeMemList.size < 0)
		return NULL;

	MemNode* pmn = freeMemList.head;
	while(pmn != NULL)
	{
		if(pmn->size >= num_bytes)
		{
			//新建一个节点
			MemNode* newNode = getFreeMemNode();
			newNode->size = num_bytes;
			newNode->pos = pmn->pos;

			//维护freeMemList
			pmn->pos += num_bytes;
			pmn->size -= num_bytes;
			freeMemList.size -= num_bytes;
			//若该节点size已经为0，则从freeMemList中删除
			if(pmn->size == 0)
			{
				memset(dropMenList(&freeMemList, pmn->pos), 0, sizeof(MemNode));
			}

			//向usedMemList插入新的节点。
			insertMemList(&usedMemList, newNode);
			return (void*) newNode->pos;
		}
		pmn = pmn->next;
	}
	return (void*)NULL;
}

void free(void* ptr)
{
	MemNode* node = dropMenList(&usedMemList, (u32)ptr);
	insertMemList(&freeMemList, node);
}

