#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "lofs.h"
#include "proc.h"
#include "global.h"
#include "proto.h"
#include "tty.h"
#include "hd.h"

PRIVATE u8		hd_status;
PRIVATE u8		hdbuf[SECTOR_SIZE * 2];
PRIVATE struct hd_info	hd_info[1];

#define DRV_OF_DEV(dev) (dev <= MAX_PRIM ? \
			dev / NR_PRIM_PER_DRIVE: \
			(dev - MINOR_hd1a))

PUBLIC void init_hd();
PUBLIC int sysHdOpen(int drive);
PUBLIC void kernelHdIdentify(int drive);
PRIVATE void analysis_identify_info(u16* hdinfo);
PRIVATE void hd_cmd_out(struct hd_cmd* cmd);
PRIVATE int waitfor(int mask, int val, int timeout);
PUBLIC void hd_handler(int irq);
PRIVATE void	get_part_table		(int drive, int sect_nr, struct part_ent * entry);
PRIVATE void	partition		(int device, int style);
PRIVATE void	print_hdinfo		(struct hd_info * hdi);
PUBLIC int sysHdClose(u32 device);
PUBLIC void kernelHdClose(u32 device);
PUBLIC int sysHdRead(u32 device, u64 position, u32 count, char* buf);
PUBLIC void kernelHdRead(u32 device, u64 position, u32 count, char* buf);
PUBLIC int sysHdIoctl(u32 device, u32 request, char* buf);
PUBLIC void kernelHdIoctl(u32 device, u32 request, char* buf);
PUBLIC int sysHdWrite(u32 device, u64 position, u32 count, char* buf);
PUBLIC void kernelHdWrite(u32 device, u64 position, u32 count, char* buf);

PUBLIC void init_hd()
{
	/* Get the number of drives from the BIOS data area */
	u8* pNrDrives = (u8*)(0x475);
//	disp_str("NrDrives:");
//	disp_int(*pNrDrives);
//	disp_str("\n");

	put_irq_handler(AT_WINI_IRQ, hd_handler);
	enable_irq(CASCADE_IRQ);	//打开级联中断线
	enable_irq(AT_WINI_IRQ);	//打开硬盘中断线
	
	int i = 0;
	for(i = 0; i < (sizeof(hd_info) / sizeof(hd_info[0])); i++)
		memset(&hd_info[i], 0, sizeof(hd_info[0]));
	hd_info[0].open_cnt = 0;
}


PUBLIC struct hd_info* kernelHdOpen(int device)
{
	int drive = DRV_OF_DEV(device);
	if(drive != 0)
	{
		disp_str("hd open error\n");
	}

	kernelHdIdentify(drive);

	if(hd_info[drive].open_cnt++ == 0){
		partition(drive * (NR_PART_PER_DRIVE + 1), P_PRIMARY);
		//print_hdinfo(&hd_info[drive]);
	}

	return &(hd_info[0]);
}

PUBLIC int sysHdClose(u32 device)
{
	kernelHdClose(device);
	return 0;
}

PUBLIC void kernelHdClose(u32 device)
{
	int drive = DRV_OF_DEV(device);
	if(drive != 0)
		disp_str("drive != 0");
	
	hd_info[drive].open_cnt--;
	
}

PUBLIC int sysHdRead(u32 device, u64 position, u32 count, char* buf)
{
	kernelHdRead(device, position, count, buf);
	return 0;
}

PUBLIC void kernelHdRead(u32 device, u64 position, u32 count, char* buf)
{
//	hd_status = in_byte(REG_STATUS);
//	disp_str("before hd_status=");
//	disp_int(hd_status);
//	disp_str("\n");
	device = 0;
	int drive = DRV_OF_DEV(device);
//	disp_str("hdRead position: ");
//	disp_int(position);
//	if((position & 0x1FF) != 0)
//		disp_str("we only allow to R/W from a SECTOR boundary\n");
	int offset_in_block = position & 0x1FF;
	
	u32 sect_nr = (u32)(position >> SECTOR_SIZE_SHIFT);
	int logidx = (device - MINOR_hd1a) % NR_SUB_PER_DRIVE;
	sect_nr += device < MAX_PRIM ?
			hd_info[drive].primary[device].base :
			hd_info[drive].logical[logidx].base;
	
	struct hd_cmd cmd;
	cmd.features = 0;
	cmd.count = (offset_in_block + count + SECTOR_SIZE - 1) / SECTOR_SIZE;
	cmd.lba_low = sect_nr & 0xFF;
	cmd.lba_mid = (sect_nr >> 8) & 0xFF;
	cmd.lba_high = (sect_nr >> 16) & 0xFF;
	cmd.device = MAKE_DEVICE_REG(1, drive, (sect_nr >> 24) & 0xF);
	cmd.command = ATA_READ;
	hd_cmd_out(&cmd);
	asm("cli");
	int bytes_left = count;
//	disp_int(count);
	char** pbuf = &buf;
	char blockBuf[SECTOR_SIZE] = {'\0'};
	int firstFlag = 1;

	while(bytes_left > 0){
//		disp_str("in hd loop@@@");	
//		disp_str("hd, byte_lefs=");
//		disp_int(bytes_left);
		int bytes = ((SECTOR_SIZE - offset_in_block) < bytes_left )? (SECTOR_SIZE - offset_in_block) : bytes_left;
//		disp_str("hd loop,bytes=");
//		disp_int(bytes);
//
//		disp_str("hd loop,beytes_left=");
//		disp_int(bytes_left);

		hdIdentifyBlockEip = p_proc_running - proc_table;
		kernelBlock();

		port_read(REG_DATA, blockBuf, SECTOR_SIZE);
		if(!firstFlag)
			offset_in_block = 0;
		else
			firstFlag = 0;
			
		memcpy(*pbuf, &(blockBuf[offset_in_block]), bytes);

		bytes_left -= bytes;
		*pbuf += bytes;
//		disp_str("hd loop,beytes_left=");
//		disp_int(bytes_left);
	}
}

PUBLIC int sysHdWrite(u32 device, u64 position, u32 count, char* buf)
{	
	kernelHdWrite(device, position, count, buf);
	return 0;
}

PUBLIC void kernelHdWrite(u32 device, u64 position, u32 count, char* buf)
{

//	disp_int((void*) kernelHdWrite);
//	hd_status = hd_status & 0xf7;
//	disp_str("after hd_status=");
//	disp_int(hd_status);
//	disp_str("\n");
//	out_byte(REG_STATUS, hd_status);
//	hd_status = in_byte(REG_STATUS);

//	int drive = DRV_OF_DEV(device);
//
//
//	if((position & 0x1FF) != 0)
//	{
//		disp_str("we only allow to R/W from a SECTOR boundary\n");
//	}
//	
//	u32 sect_nr = (u32)(position >> SECTOR_SIZE_SHIFT);
//	int logidx = (device - MINOR_hd1a) % NR_SUB_PER_DRIVE;
//	sect_nr += device < MAX_PRIM ?
//			hd_info[drive].primary[device].base :
//			hd_info[drive].logical[logidx].base;
//	
//	struct hd_cmd cmd;
//	cmd.features = 0;
//	cmd.count = (count + SECTOR_SIZE - 1) / SECTOR_SIZE;
//	disp_str("count: ");
//	disp_int(cmd.count);
//	cmd.lba_low = sect_nr & 0xFF;
//	disp_str("\nlba_low: ");
//	disp_int(cmd.lba_low);
//	cmd.lba_mid = (sect_nr >> 8) & 0xFF;
//	disp_str("\nlba_mid: ");
//	disp_int(cmd.lba_mid);
//	cmd.lba_high = (sect_nr >> 16) & 0xFF;
//	disp_str("\nlba_high: ");
//	disp_int(cmd.lba_high);
//	cmd.device = MAKE_DEVICE_REG(1, drive, (sect_nr >> 24) & 0xF);
//	disp_str("\ndevice: ");
//	disp_int(cmd.device);
//	cmd.command = ATA_WRITE;
//	disp_str("\ncommand: ");
//	disp_int(cmd.command);
//	disp_str("\n");
//	hd_cmd_out(&cmd);
//	asm("sti");
//	
//	disp_str("cmd out");
//	int bytes_left = count;
//	while(bytes_left > 0){
//		int bytes = (SECTOR_SIZE <  bytes_left) ? SECTOR_SIZE : bytes_left;
//
//		if(!waitfor(STATUS_DRQ, STATUS_DRQ, HD_TIMEOUT))
//			disp_str("hd writing error.\n");
//
//		port_write(REG_DATA, buf, bytes);
//		hdIdentifyBlockEip = p_proc_running - proc_table;
//		disp_int(hdIdentifyBlockEip);
//		disp_str("before block!\n");
//		asm("cli");
//		kernelBlock();
//		asm("sti");
//		disp_str("after block!\n");
//		bytes_left -= bytes;
//		buf += bytes;
//	}
	int drive = DRV_OF_DEV(device);

	u64 pos = position;

	u32 sect_nr = (u32)(pos >> SECTOR_SIZE_SHIFT);
	int logidx = (device - MINOR_hd1a) % NR_SUB_PER_DRIVE;
	sect_nr += device < MAX_PRIM ?
		hd_info[drive].primary[device].base :
		hd_info[drive].logical[logidx].base;
	
	struct hd_cmd cmd;
	cmd.features = 0;
	cmd.count = (count + SECTOR_SIZE - 1) / SECTOR_SIZE;
	cmd.lba_low = sect_nr & 0xFF;
	cmd.lba_mid = (sect_nr >> 8) & 0xFF;
	cmd.lba_high = (sect_nr >> 16) & 0xFF;
	cmd.device = MAKE_DEVICE_REG(1, drive, (sect_nr >> 24) & 0xF);
	cmd.command = ATA_WRITE;
	hd_cmd_out(&cmd);

	int bytes_left = count;
	while(bytes_left > 0){
		int bytes = (SECTOR_SIZE < bytes_left) ? SECTOR_SIZE : bytes_left;

		if(!waitfor(STATUS_DRQ, STATUS_DRQ, HD_TIMEOUT))
			disp_str("hd writing error.\n");

		port_write(REG_DATA, buf, bytes);
		hdIdentifyBlockEip = p_proc_running - proc_table;
		kernelBlock();
		
		bytes_left -= bytes;
		buf += bytes;
	}
}



PUBLIC int sysHdIoctl(u32 device, u32 request, char* buf)
{
	kernelHdIoctl(device, request, buf);
	return 0;
}

PUBLIC void kernelHdIoctl(u32 device, u32 request, char* buf)
{
	int drive = DRV_OF_DEV(device);

	struct hd_info * hdi = &hd_info[drive];

	if(request == DIOCTL_GET_GEO){
		memcpy(buf, 
			device < MAX_PRIM ? &hdi->primary[device] : &hdi->logical[(device - MINOR_hd1a) % NR_SUB_PER_DRIVE],
			sizeof(struct part_info));
	}
	else
		disp_str("no such command\n");
}

/*****************************************************************************
 *                                get_part_table
 *****************************************************************************
 * <Ring 1> Get a partition table of a drive.
 * 
 * @param drive   Drive nr (0 for the 1st disk, 1 for the 2nd, ...)n
 * @param sect_nr The sector at which the partition table is located.
 * @param entry   Ptr to part_ent struct.
 *****************************************************************************/
PRIVATE void get_part_table(int drive, int sect_nr, struct part_ent * entry)
{
	struct hd_cmd cmd;
	cmd.features = 0;
	cmd.count = 1;
	cmd.lba_low = sect_nr & 0xFF;
	cmd.lba_mid = (sect_nr >> 8) & 0xFF;
	cmd.lba_high = (sect_nr >> 16) & 0xFF;
	cmd.device = MAKE_DEVICE_REG(1, /* LBA mode */
				     drive, 
				     (sect_nr >> 24) & 0xF);
	cmd.command = ATA_READ;
	hd_cmd_out(&cmd);
	hdIdentifyBlockEip = p_proc_running - proc_table;
	kernelBlock();
	asm("sti");
	
	port_read(REG_DATA, hdbuf, SECTOR_SIZE);
	memcpy(entry,
		hdbuf + PARTITION_TABLE_OFFSET, 
		sizeof(struct part_ent) * NR_PART_PER_DRIVE);
}

/*****************************************************************************
 *                                partition
 *****************************************************************************
 * <Ring 1> This routine is called when a device is opened. It reads the
 * partition table(s) and fills the hd_info struct.
 * 
 * @param device Device nr.
 * @param style  P_PRIMARY or P_EXTENDED.
 *****************************************************************************/
PRIVATE void partition(int device, int style)
{
	int i;
	int drive = DRV_OF_DEV(device);
	struct hd_info* hdi = &hd_info[drive];

	struct part_ent part_table[NR_SUB_PER_DRIVE];

	if(style == P_PRIMARY){
		get_part_table(drive, drive, part_table);

		int nr_prim_parts = 0;
		for(i = 0; i < NR_PART_PER_DRIVE; i++){
			if(part_table[i].sys_id == NO_PART)
				continue;

			nr_prim_parts++;
			int dev_nr = i + 1;			/* 1~4*/
			hdi->primary[dev_nr].base = part_table[i].start_sect;
			hdi->primary[dev_nr].size = part_table[i].nr_sects;

			if(part_table[i].sys_id == EXT_PART)	/* extended */
				partition(device + dev_nr, P_EXTENDED);
		}
	}
	else if(style == P_EXTENDED){
		int j = device % NR_PRIM_PER_DRIVE;	/* 1~4 */
		int ext_start_sect = hdi->primary[j].base;
		int s = ext_start_sect;
		int nr_1st_sub = (j - 1) * NR_SUB_PER_PART; /* 0/16/32/48 */

		for(i = 0; i < NR_SUB_PER_PART; i++){
			int dev_nr = nr_1st_sub + i;/* 0~15/16~31/32~47/48~63 */

			get_part_table(drive, s, part_table);

			hdi->logical[dev_nr].base = s + part_table[0].start_sect;
			hdi->logical[dev_nr].size = part_table[0].nr_sects;

			s = ext_start_sect + part_table[1].start_sect;

			if(part_table[1].sys_id == NO_PART)
				break;
		}
	}
	else
		disp_str("con't hadle such TYPE of partition\n");
}

/*****************************************************************************
 *                                print_hdinfo
 *****************************************************************************/
/**
 * <Ring 1> Print disk info.
 * 
 * @param hdi  Ptr to struct hd_info.
 *****************************************************************************/
PRIVATE void print_hdinfo(struct hd_info* hdi)
{
	int i;
	for(i = 0; i < NR_PART_PER_DRIVE + 1; i++){
//		printl("%sPART_%d: base %d(0x%x), size %d(0x%x) (in sector)\n",
//			i == 0 ? " " : "     ",
//			i,
//			hdi->primary[i].base,
//			hdi->primary[i].base,
//			hdi->primary[i].size,
//			hdi->primary[i].size);

		disp_str(i == 0 ? " " : "     ");
		disp_str("PART_");
		disp_int(i);
		disp_str(": base ");
		disp_int(hdi->primary[i].base);
		disp_str(", size ");
		disp_int(hdi->primary[i].size);
		disp_str("\n");
	}
	for(i = 0; i < NR_SUB_PER_DRIVE; i++){
		if(hdi->logical[i].size == 0)
			continue;
//		printl("         "
//		       "%d: base %d(0x%x), size %d(0x%x) (in sector)\n",
//		       i,
//		       hdi->logical[i].base,
//		       hdi->logical[i].base,
//		       hdi->logical[i].size,
//		       hdi->logical[i].size);
		disp_str("         ");
		disp_int(i);
		disp_str(": base ");
		disp_int(hdi->logical[i].base);
		disp_str(", size ");
		disp_int(hdi->logical[i].size);
	}
}

PUBLIC int sysHdOpen(int device)
{
//	disp_str("sysHdOpen@@@@\n");
	return (int)kernelHdOpen(device);
}

/* @param drive  When drive == 0, master is selected
		When drive == 1 slave is selected*/
PUBLIC void kernelHdIdentify(int drive)
{
	struct hd_cmd cmd;
	cmd.device = MAKE_DEVICE_REG(0, drive, 0);
	cmd.command = ATA_IDENTIFY;
	hd_cmd_out(&cmd);
	hdIdentifyBlockEip = p_proc_running - proc_table;
	kernelBlock();
	port_read(REG_DATA, hdbuf, SECTOR_SIZE);

	//analysis_identify_info((u16*)hdbuf);
}

PRIVATE void analysis_identify_info(u16* hdinfo)
{
	disp_str("print hd info: \n");
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
	hd_info[0].lba_support = (capabilities & 0x0200);
	//printf("LBA48 supported: %s\n", 
	//	(capabilities & 0x0200) ? "YES" : "NO");
	disp_str("LBA supported: ");
	disp_str((capabilities & 0x0200) ? "YES" : "NO");
	disp_str("\n");

	int cmd_set_supported = hdinfo[83];
	hd_info[0].lba48_support = (cmd_set_supported & 0x0400);
	//printf("LBA48 supported: %s\n",
	//	(cmd_set_supported & 0x0400) ? "YES" : "NO");
	disp_str("LBA48 supported: ");
	disp_str((cmd_set_supported & 0x0400) ? "YES" : "NO");
	disp_str("\n");

	int sectors = ((int)hdinfo[61] << 16) + hdinfo[60];
	hd_info[0].hd_size = sectors * 512;
	//printf("HD size: %dMB\n", sectors * 512 / 1000000);
	disp_str("HD size: ");
	disp_int(sectors * 512 / 1024 / 1024);
	disp_str("MB\n");
}


PRIVATE void hd_cmd_out(struct hd_cmd* cmd)
{
	asm("cli");
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
//	disp_str("hd handler!!  ");
	hd_status = in_byte(REG_STATUS);
//	disp_int((void*)hd_handler);
//	disp_str("\nhd_status=");
//	disp_int(hd_status);
//	disp_str("\n");
//
//	disp_int(hdIdentifyBlockEip);
	kernelUnBlock(hdIdentifyBlockEip);
}
