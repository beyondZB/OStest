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

/** @file       mbr.c
 *  @brief      MBR解析库，可以提取MBR磁盘分区表
 *  @date       2014年11月07日 15时57分28秒
 *  @author     kinuxroot, kinuxroot@163.com
 *  @group     LessOS Development Group
 *  @version    1.0
 *  @note       
 *
 */

#include "mbr.h"

PRIVATE void readMbrPartTableItem(Device disk, MbrPartTableItem* tableItem);
PRIVATE int readPrimaryPartitions(Device disk, MbrPartTable* partTable);
PRIVATE void readLogicalPartitions(Device disk, int extendPartIndex, MbrPartTable* partTable);
PRIVATE void printMbrPartTableItem(MbrPartTableItem* tableItem);

PUBLIC void readMbrPartTable(Device disk, MbrPartTable* partTable){
    int extendPartIndex = readPrimaryPartitions(disk, partTable);
    if ( extendPartIndex != -1 ) {
        readLogicalPartitions(disk, extendPartIndex, partTable);
    }
}

PUBLIC void printMbrPartTable(MbrPartTable* partTable){
    int partIndex = 0;
    for ( partIndex = 0; partIndex < MBR_PRIMARY_PART_COUNT; partIndex ++ ) {
        printf("Primary Part %d: ", partIndex);
        printMbrPartTableItem(partTable->primaryPartTable + partIndex);
        printf("\n");
    }
    for ( partIndex = 0; partIndex < MBR_LOGICAL_PART_COUNT; partIndex ++ ) {
        printf("Logical Part %d: ", partIndex);
        printMbrPartTableItem(&(partTable->logicalPartTable[partIndex].currentPartItem));
        printf("\n");
        if ( partTable->logicalPartTable[partIndex].nextPartItem.type != EXTEND_PART_TYPE ) {
            break;
        }
    }
}

PRIVATE int readPrimaryPartitions(Device disk, MbrPartTable* partTable){
    seekDevice(disk, MBR_PART_TABLE_OFFSET, SEEK_SET);
    int partIndex = 0;
    for ( partIndex = 0; partIndex < MBR_PRIMARY_PART_COUNT; partIndex ++ ) {
        readMbrPartTableItem(disk, partTable->primaryPartTable + partIndex);
        if ( partTable->primaryPartTable[partIndex].type == EXTEND_PART_TYPE ) {
            return partIndex;
        }
    }
}

PRIVATE void readLogicalPartitions(Device disk, int extendPartIndex, MbrPartTable* partTable){
    partTable->logicalTableBeginSector = partTable->primaryPartTable[extendPartIndex].beginSector;
    u32 nextBeginSector = partTable->logicalTableBeginSector;
    int partIndex = 0;
    for ( partIndex = 0; partIndex < MBR_LOGICAL_PART_COUNT; partIndex ++ ) {
        seekDevice(disk, SEEK_TO_MBR_PART_TABLE(SECTOR2BYTES(nextBeginSector)), SEEK_SET);
        readMbrPartTableItem(disk, &(partTable->logicalPartTable[partIndex].currentPartItem));
        readMbrPartTableItem(disk, &(partTable->logicalPartTable[partIndex].nextPartItem));

        if ( partTable->logicalPartTable[partIndex].nextPartItem.type != EXTEND_PART_TYPE ) {
            break;
        }
        else {
            nextBeginSector = partTable->logicalTableBeginSector +
                partTable->logicalPartTable[partIndex].nextPartItem.beginSector;
        }
    }
}

PRIVATE void readMbrPartTableItem(Device disk, MbrPartTableItem* tableItem){
    char buffer[16];
    readDevice(disk, buffer, 16);
    tableItem->boot = buffer[0];
    tableItem->type = buffer[4];
    tableItem->beginSector = *((u32*)(buffer + 8));
    tableItem->sectorCount = *((u32*)(buffer + 12));
}

PRIVATE void printMbrPartTableItem(MbrPartTableItem* tableItem){
    printf("Boot: %xh; Type: %xh; Begin Sector: %u; Sector Count: %u",
            tableItem->boot,
            tableItem->type,
            tableItem->beginSector,
            tableItem->sectorCount);
}
