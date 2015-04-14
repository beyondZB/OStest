#include "type.h"
#include "const.h"
#include "protect.h"

PUBLIC void* memcpy(void* pDst, void* pSec, int iSize);

PUBLIC void  disp_str(char* pszInfo);

PUBLIC u8		gdt_ptr[6];	/*0~15:Limit	16~47:Base*/
PUBLIC DESCRIPTOR	gdt [GDT_SIZE];	/*新的全局描述符表*/

PUBLIC void cstart()
{
	disp_str("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
		 "-----\"cstart\" begins-----\n");

	//将LOADER中的GDT复制到新的GDT中
	memcpy(&gdt,
		(void*)(*((u32*)(&gdt_ptr[2]))),
		*((u16*)(&gdt_ptr[0])) + 1
		);

	//gdt_ptr[6] 共6个字节:0~15:Limit 16~47:Base 用作sgdt/lgdt的参数
	u16* p_gdt_limit = (u16*)(&gdt_ptr[0]);
	u32* p_gdt_base = (u32*)(&gdt_ptr[2]);
	*p_gdt_limit = GDT_SIZE * sizeof(DESCRIPTOR) - 1;
	*p_gdt_base = (u32)&gdt;

	disp_str("-----\"cstart\" end--------\n");
}
