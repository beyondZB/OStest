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

/** @file       mbr.h
 *  @brief      MBR解析库，可以提取MBR磁盘分区表
 *  @date       2014年11月07日 15时57分28秒
 *  @author     kinuxroot, kinuxroot@163.com
 *  @group      LessOS Development Group
 *  @version    1.0
 *  @note
 *
 */

#ifndef _MBR_H
#define _MBR_H

#include "type.h"
#include "lib/device.h"

typedef struct {
    u8 boot;
    u8 type;
    u32 beginSector;
    u32 sectorCount;
} MbrPartTableItem;

typedef struct {
    MbrPartTableItem currentPartItem;
    MbrPartTableItem nextPartItem;
} MbrLogicalPartTable;

#define MBR_PRIMARY_PART_COUNT  4
#define MBR_LOGICAL_PART_COUNT  100

typedef struct {
    MbrPartTableItem primaryPartTable[MBR_PRIMARY_PART_COUNT];
    u32 logicalTableBeginSector;
    MbrLogicalPartTable logicalPartTable[MBR_LOGICAL_PART_COUNT];
} MbrPartTable;

#define EXTEND_PART_TYPE        5

PUBLIC void readMbrPartTable(Device disk, MbrPartTable* partTable);
PUBLIC void printMbrPartTable(MbrPartTable* partTable);

#define SECTOR_BYTES            512
#define SECTOR2BYTES(sector)    ((sector)*SECTOR_BYTES)

#define MBR_PART_TABLE_OFFSET           0x1be
#define SEEK_TO_MBR_PART_TABLE(address) ((address) + MBR_PART_TABLE_OFFSET)

#endif //_MBR_H
