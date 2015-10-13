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

/** @file       task.c
 *  @brief      lofsutil功能接口封装，主要任务是调用LOFS内部实现并提供接口。
 *  @date       2014年11月07日 15时57分28秒
 *  @author     kinuxroot, kinuxroot@163.com
 *  @group      LessOS Development Group
 *  @version    1.0
 *  @note       
 *
 */

#include "task.h"
#include "lofs.h"

#include <stdio.h>
#include <string.h>

// MBR字节数
#define MBR_BYTES   512

 //EMBEDDED SECTOR字节数
 #define EMBEDDED_BYTES 1024

PRIVATE void clearDevice(Device device);

// 选项帮助信息数量
#define HELP_OPTDESC_COUNT  10
// 选项帮助信息的选项字段长度
#define HELP_OPTDESC_OPT_LENGTH 20
// 选项帮助信息的选项描述信息长度
#define HELP_OPTDESC_DESC_LENGTH 100

// 选项帮助信息结构体
typedef struct {
    // 选项格式
    char option[HELP_OPTDESC_OPT_LENGTH];
    // 选项解释
    char description[HELP_OPTDESC_DESC_LENGTH];
} HelpOptionDescription;

// 所有的选项帮助信息
PRIVATE HelpOptionDescription helpOptionsDescriptions[HELP_OPTDESC_COUNT] = {
    { "-h", "Show this information." },
    { "-c", "Clear the device(VERY DANGEROUS!!!!)." },
    { "-f", "Format partition and make file system." },
    { "-m", "Show partition file system metadata and statistics." },
    { "-l", "List files under /." },
    { "-p <filename>", "Put file named filename into /." },
    { "-g <filename>", "Get file named filename from / and stored it in current directory." },
    { "-r <filename>", "Remove file named filename from /." },
    { "-b <filename>", "Copy first 512 bytes of binary file into device MBR(VERY DANGEROUS!!!!)" },
    { "-e <filename>", "Copy first 1024 bytes of binary file into device EMBEDDED SECTOR(VERY DANGEROUS!!!!)" }
};

/**
 * @brief 执行帮助任务
 *
 * @return 无返回值
 */
PUBLIC void task_help(){
    printf("lofsutil: A utility used to manage LOFS partition\n");
    printf("Usage: lofsutil <device> [option]\n");
    printf("\nOptions:\n");

    int optionIndex = 0;
    // 遍历选项帮助信息，逐个输出帮助信息
    for ( optionIndex = 0; optionIndex < HELP_OPTDESC_COUNT; optionIndex ++ ) {
        // 输出帮助信息
        // 格式：
        // 选项格式描述     选项解释
        printf("  %-16s%s\n", 
                helpOptionsDescriptions[optionIndex].option,
                helpOptionsDescriptions[optionIndex].description);
    }
}

/**
 * @brief 执行分区格式化任务
 *
 * @param device 设备对象
 *
 * @return 无返回值
 */
PUBLIC void task_format(Device device){
    printf("Formatting ...\n");
    lofsFormatDevice(device);
    printf("Format successfully!\n");
}

/**
 * @brief 执行清除磁盘任务
 *
 * @param device 设备对象
 *
 * @return 无返回值
 */
PUBLIC void task_clear(Device device){
    printf("Clearing ...\n");
    clearDevice(device);
    printf("Clear successfully!\n");
}

/**
 * @brief 执行获取设备元数据任务，也会输出一些额外统计信息
 *
 * @param device 设备对象
 *
 * @return 无返回值
 */
PUBLIC void task_metadata(Device device){
    printf("Metadata:\n");
    lofsShowMetadata(device);
}

/**
 * @brief 执行列出根目录文件任务
 *
 * @param device 设备对象
 *
 * @return 无返回值
 */
PUBLIC void task_list(Device device){
    printf("List files:\n");
    lofsListFiles(device);
}

/**
 * @brief 执行存入本地文件任务，将本地文件放在分区根目录下
 *
 * @param device 设备对象
 * @param filename 需要存入分区的本地文件路径
 *
 * @return 无返回值
 */
PUBLIC void task_put(Device device, const char *filename){
    printf("Put file: %s\n", filename);
    u8 ret = lofsPutFile(device, filename);

    if ( ret == LOFS_PUTFILE_FILE_EXIST ) {
        printf("File %s exists!\n", filename);
    }
    else if ( ret == LOFS_PUTFILE_LOCALFILE_NOT_EXIST ) {
        printf("Local file %s doesn't exist!\n", filename);
    }
    else {
        printf("Put file %s successfully!\n", filename);
    }
}

/**
 * @brief 从分区根目录中获取文件，存放到本地，会存储成当前路径下的同名文件
 *
 * @param device 设备对象
 * @param filename 需要获取的文件名
 *
 * @return 无返回值
 */
PUBLIC void task_get(Device device, const char *filename){
    printf("Get file: %s\n", filename);
    u8 ret = lofsGetFile(device, filename);

    if ( ret == LOFS_GETFILE_FILE_NOT_EXIST ) {
        printf("File %s doesn't exist!\n", filename);
    }
    else {
        printf("Get file %s successfully\n", filename);
    }
}

/**
 * @brief 执行删除分区中文件的任务
 *
 * @param device 设备对象
 * @param filename 欲删除文件的文件名
 *
 * @return 无返回值
 */
PUBLIC void task_remove(Device device, const char *filename){
    printf("Remove file: %s\n", filename);
    u8 ret = lofsRemoveFile(device, filename);

    if ( ret == LOFS_REMOVEFILE_FILE_NOT_EXIST ) {
        printf("File %s doesn't exist!\n", filename);
    }
    else {
        printf("Remove file %s successfully\n", filename);
    }
}

/**
 * @brief 执行写入引导代码任务
 *
 * @param device 设备对象
 * @param filename 引导代码二进制文件路径
 *
 * @return 无返回值
 */
PUBLIC void task_boot(Device device, const char *filename){
    // 打开引导代码文件
    printf("Write boot code into mbr.\n");
    FILE *bootCodeFile = fopen(filename, "r");
    if ( !bootCodeFile ) {
        printf("Local file %s doesn't exist!\n", filename);
    }

    // 读入前512字节（MBR长度）
    char mbrBuffer[MBR_BYTES];
    size_t readBytes = fread(mbrBuffer, 1, MBR_BYTES, bootCodeFile);
    printf("Bytes in: %lu\n", readBytes);

    // 写入到设备MBR中
    seekDevice(device, DEV_SEEK_BEGIN, 0);
    writeDevice(device, mbrBuffer, MBR_BYTES);
    printf("Bytes out: %lu\n", readBytes);

    fclose(bootCodeFile);
}

/**
 * @brief 执行写入嵌入式扇区代码任务
 *
 * @param device 设备对象
 * @param filename 引导代码二进制文件路径
 *
 * @return 无返回值
 */
PUBLIC void task_embedded(Device device, const char *filename){
    // 打开引导代码文件
    printf("Write boot code into embedded sector.\n");
    FILE *bootCodeFile = fopen(filename, "r");
    if ( !bootCodeFile ) {
        printf("Local file %s doesn't exist!\n", filename);
    }

    // 读入前1024字节（EMBEDDED SECTOR长度）
    char embeddedBuffer[EMBEDDED_BYTES];
    size_t readBytes = fread(embeddedBuffer, 1, EMBEDDED_BYTES, bootCodeFile);
    printf("Bytes in: %lu\n", readBytes);

    // 写入到设备MBR中
    seekDevice(device, DEV_SEEK_BEGIN, 512);
    writeDevice(device, embeddedBuffer, EMBEDDED_BYTES);
    printf("Bytes out: %lu\n", readBytes);

    fclose(bootCodeFile);
}

/**
 * @brief 清除整个设备，全部填充0
 *
 * @param device 设备对象
 *
 * @return 无返回值
 */
PRIVATE void clearDevice(Device device){
    char zeroBuffer[SECTOR_BYTES];
    memset(zeroBuffer, 0, sizeof(zeroBuffer));
    Size writtenSize = 0;
    while ( writtenSize < DISK_SIZE ) {
        writeDevice(device, zeroBuffer, sizeof(zeroBuffer));
        writtenSize += sizeof(zeroBuffer);
    }
}
