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

/** @file       main.c
 *  @brief      lofsutil主程序
 *  @date       2014年11月07日 15时57分28秒
 *  @author     kinuxroot, kinuxroot@163.com
 *  @group      LessOS Development Group
 *  @version    1.0
 *  @note       
 *
 */

#include "lib/device.h"
#include "task.h"

#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Device disk = NULL;

int main(int argc, char** argv){
    int option = 0;
    // 如果选项中带参数，那么使用flag_filename记录该参数
    char* flag_filename = 0;

    // 根据选项设置任务
    enum {
        // 帮助
        TASK_HELP,
        // 清除磁盘
        TASK_CLEAR,
        // 格式化分区
        TASK_FORMAT,
        // 获取元数据和统计信息
        TASK_METADATA,
        // 列出根目录文件
        TASK_LIST,
        // 将本地文件放入分区
        TASK_PUT,
        // 从分区中获取文件
        TASK_GET,
        // 删除文件
        TASK_REMOVE,
        // 写入引导代码
        TASK_BOOT,
        //写入嵌入式扇区代码
        TASK_EMBEDDED
    } task = TASK_HELP;

    // 使用getopt解析命令行
    while ( (option = getopt(argc, argv, "cfhmlp:g:r:b:e:")) != -1 ) {
        switch (option) {
        case 'c':
            task = TASK_CLEAR;
            break;
        case 'h':
            task_help();
            exit(EXIT_SUCCESS);
        case 'f':
            task = TASK_FORMAT;
            break;
        case 'm':
            task = TASK_METADATA;
            break;
        case 'l':
            task = TASK_LIST;
            break;
        case 'p':
            task = TASK_PUT;
            flag_filename = optarg;
            break;
        case 'g':
            task = TASK_GET;
            flag_filename = optarg;
            break;
        case 'r':
            task = TASK_REMOVE;
            flag_filename = optarg;
            break;
        case 'b':
            task = TASK_BOOT;
            flag_filename = optarg;
            break;
        case 'e':
            task = TASK_EMBEDDED;
            flag_filename =  optarg;
        }
    }

    if ( task == TASK_HELP ) {
        task_help();
        exit(EXIT_SUCCESS);
    }

    // 如果有多余的参数，代表需要操作的设备文件，则尝试打开该设备
    if ( argc > optind ) {
        disk = openDevice(argv[optind], DEV_OM_WRITE);
        if ( disk == NULL ) {
            printf("File not found: %s!\n", argv[optind]);
            exit(EXIT_FAILURE);
        }
    }
    else {
        printf("Please select a device!\n");
        exit(EXIT_FAILURE);
    }

    char buffer[512];

    // 根据任务选项调用特定的任务函数
    switch ( task ) {
    case TASK_CLEAR:
        task_clear(disk);
        break;
    case TASK_FORMAT:
        task_format(disk);
        break;
    case TASK_METADATA:
        task_metadata(disk);
        break;
    case TASK_LIST:
        task_list(disk);
        break;
    case TASK_PUT:
        task_put(disk, flag_filename);
        break;
    case TASK_GET:
        task_get(disk, flag_filename);
        break;
    case TASK_REMOVE:
        task_remove(disk, flag_filename);
        break;
    case TASK_BOOT:
        task_boot(disk, flag_filename);
        break;
    case TASK_EMBEDDED:
        task_embedded(disk, flag_filename);
        break;
    }

    closeDevice(disk);

    return EXIT_SUCCESS;
}