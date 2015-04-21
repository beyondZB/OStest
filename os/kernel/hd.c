#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "global.h"
#include "proto.h"
#include "hd.h"

PRIVATE u8	hd_status;
PRIVATE u8	hdbuf[SECTOR_SIZE * 2];

PUBLIC void init_hd();
PUBLIC int sysHdIdentify(int drive);
PUBLIC void kernelHdIdentify(int drive);
PRIVATE void print_identify_info(u16* hdinfo);
PRIVATE void hd_cmd_out(struct hd_cmd* cmd);
PRIVATE int waitfor(int mask, int val, int timeout);
PUBLIC void hd_handler(int irq);



PUBLIC void init_hd()
{
	/* Get the number of drives from the BIOS data area */
	u8* pNrDrives = (u8*)(0x475);
	disp_str("NrDrives:");
	disp_int(*pNrDrives);
	disp_str("\n");

	put_irq_handler(AT_WINI_IRQ, hd_handler);
	enable_irq(CASCADE_IRQ);	//打开级联中断线
	enable_irq(AT_WINI_IRQ);	//打开硬盘中断线
}


PUBLIC int sysHdIdentify(int drive)
{
//	disp_str("sysHdIdentify@@@@\n");
	kernelHdIdentify(drive);
	return 0;
}

/* @param drive  When drive == 0, master is selected
		When drive == 1 slave is selected*/
PUBLIC void kernelHdIdentify(int drive)
{
	struct hd_cmd cmd;
	cmd.device = MAKE_DEVICE_REG(0, drive, 0);
	cmd.command = ATA_IDENTIFY;
	hd_cmd_out(&cmd);
	kernelBlock();
	asm("sti");
	port_read(REG_DATA, hdbuf, SECTOR_SIZE);

	print_identify_info((u16*)hdbuf);
}


PRIVATE void print_identify_info(u16* hdinfo)
{
	disp_str("print hd info: \n");
	disp_int((void*) print_identify_info);
	int i, k;
	char s[64];

	struct iden_info_ascii{
		int idx;
		int len;
		char* desc;
	}iinfo[] = {{10, 20, "HD SN"}, /* Serial number in ASCII */
		    {27, 40, "HD Model"} /* Model number in ASCII */};
	for(k = 0; k < sizeof(iinfo) / sizeof(iinfo[0]); k++){
		char* p = (char*)&hdinfo[iinfo[k].idx];

		for(i = 0; i < iinfo[k].len / 2; i++){
			s[ i * 2 + 1] = *p++;		//这里是按字为单位的,一个字两个字节,小端存储,虽易要把每个字中的两个字节调换顺序;
			s[ i * 2] = *p++;
		}
		s[i * 2] = 0;	//'/0'
		//printf("%s: %s\n", iinfo[k],descm s);
		disp_str(iinfo[k].desc);
		disp_str(": ");
		disp_str(s);
		disp_str("\n");
	}

	int capabilities = hdinfo[49];
	//printf("LBA48 supported: %s\n", 
	//	(capabilities & 0x0200) ? "YES" : "NO");
	disp_str("LBA supported: ");
	disp_str((capabilities & 0x0200) ? "YES" : "NO");
	disp_str("\n");

	int cmd_set_supported = hdinfo[83];
	//printf("LBA48 supported: %s\n",
	//	(cmd_set_supported & 0x0400) ? "YES" : "NO");
	disp_str("LBA48 supported: ");
	disp_str((cmd_set_supported & 0x0400) ? "YES" : "NO");
	disp_str("\n");

	int sectors = ((int)hdinfo[61] << 16) + hdinfo[60];
	//printf("HD size: %dMB\n", sectors * 512 / 1000000);
	disp_str("HD size: ");
	disp_int(sectors * 512 / 1024 / 1024);
	disp_str("MB\n");
}


PRIVATE void hd_cmd_out(struct hd_cmd* cmd)
{
	/* For all commands, the host must first check if BSY=1,
	   and should proceed no further unless and until BSY = 0*/
	if(!waitfor(STATUS_BSY, 0, HD_TIMEOUT))
		disp_str("hd error.");
	
	/* Activate the Interrupt Enable (nIEN) bit */
	out_byte(REG_DEV_CTRL, 0);
	/* Load required parameters in the Command Block Registers */
	out_byte(REG_FEATURES, cmd->features);
	out_byte(REG_NSECTOR, cmd->count);
	out_byte(REG_LBA_LOW, cmd->lba_low);
	out_byte(REG_LBA_MID, cmd->lba_mid);
	out_byte(REG_LBA_HIGH, cmd->lba_high);
	out_byte(REG_DEVICE, cmd->device);
	asm("cli");
	/* Write the command code to the Command Register */
	out_byte(REG_CMD, cmd->command);
}

/*********************************************************
		waitfor
**********************************************************
		
@param	mask		Status mask
@param	val		Required status
@param	timeout		Timeout in milliseconds

@return One if success, zero if timeout
**********************************************************/
PRIVATE int waitfor(int mask, int val, int timeout)
{
	int t = kernelGetTicks();

	while(((kernelGetTicks() - t) * 1000 / HZ) < timeout)
		if((in_byte(REG_STATUS) & mask) == val)
			return 1;
	return 0;
}

/**********************************************************
			hd_handler
***********************************************************
 interrupt handler

@param irq IRQ nr of the disk interrupt
***********************************************************/
PUBLIC void hd_handler(int irq)
{
	disp_str("hd handler\n");
	hd_status = in_byte(REG_STATUS);

	kernelUnBlock(1);
}



