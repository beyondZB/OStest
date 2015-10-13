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

/** @file       device.c
 *  @brief      基于C标准库的设备读写实现
 *  @date       2014年11月07日 15时57分28秒
 *  @author     kinuxroot, kinuxroot@163.com
 *  @group      LessOS Development Group
 *  @version    1.0
 *  @note       
 *              在Unix和类Unix上将设备模拟为文件的系统中可以使用
 *
 */

#include "lib/platform/std/device.h"
#include "lib/list.h"

#include <stdlib.h>

// 设备信息结构体
typedef struct {
    // C标准库文件对象指针，代表设备文件
    FILE* file;
    // 链表节点，用于连接各个节点
    ListNode listNode;
} DeviceInfo;

// 设备列表
List deviceList;
// 设备列表是否初始化
PRIVATE int isDeviceListInited = 0;
PRIVATE DeviceInfo *findDeviceInfo(Device device);

/**
 * @brief 使用给定的文件名和打开模式打开设备
 *
 * @param fileName 设备文件名
 * @param openMode 文件打开模式，参见DeviceOpenMode列表
 *
 * @return 打开的设备对象
 */
PUBLIC Device stdOpenDevice(const char* fileName, DeviceOpenMode openMode){
    Device device = fopen(fileName, openMode);

    // 创建设备信息
    DeviceInfo *deviceInfo = (DeviceInfo*)malloc(sizeof(DeviceInfo));
    deviceInfo->file = device;

    // 如果设备列表尚未初始化，首先初始化设备列表
    if ( !isDeviceListInited ) {
        isDeviceListInited = 1;
        listInit(&deviceList);
    }
    // 将设备信息添加到设备列表中
    listAdd(&deviceList, deviceInfo);

    return device;
}

/**
 * @brief 关闭指定设备对象
 *
 * @param device 设备对象
 *
 * @return 无返回值
 */
PUBLIC void stdCloseDevice(Device device){
    DeviceInfo *deviceInfo = findDeviceInfo(device);
    if ( deviceInfo ) {
        // 从设备列表中移除设备信息节点，并释放节点，关闭文件
        listRemove(deviceInfo);
        free(deviceInfo);
        fclose(device);
    }
}

/**
 * @brief 从设备的当前位置读取固定字节数据到指定缓冲区中
 *
 * @param device 设备对象
 * @param buffer 目标缓冲区
 * @param size 读取字节数
 *
 * @return 返回已读取的字节数
 */
PUBLIC Size stdReadDevice(Device device, void *buffer, Size size){
    Size readSize = fread(buffer, size, 1, device);

    return readSize;
}

/**
 * @brief 将缓冲区中固定字节数的数据写入设备的当前位置
 *
 * @param device 设备对象
 * @param buffer 来源缓冲区
 * @param size 写入字节数
 *
 * @return 返回写入的字节数
 */
PUBLIC Size stdWriteDevice(Device device, void *buffer, Size size){
    Size writtenSize = fwrite(buffer, size, 1, device);
}

/**
 * @brief 根据特定模式移动设备指针
 *
 * @param device 设备对象
 * @param seekMode 指针移动模式，三种模式见设备指针移动模式列表
 * @param size 需要移动的字节数
 *
 * @return 无返回值
 */
PUBLIC void stdSeekDevice(Device device, DeviceSeekMode seekMode, Size size){
    fseek(device, size, seekMode);
}

/**
 * @brief 返回当前设备指针的偏移
 *
 * @param device 设备对象
 *
 * @return 当前设备指针偏移
 */
PUBLIC Size stdDeviceOffset(Device device){
    return ftell(device);
}

/**
 * @brief 根据设备对象查找设备信息节点
 *
 * @param device 设备对象
 *
 * @return 设备信息节点指针
 */
PRIVATE DeviceInfo *findDeviceInfo(Device device){
    DeviceInfo *deviceInfo = NULL;
    listForEach(DeviceInfo, deviceInfo, &deviceList) {
        if ( deviceInfo->file == device ) {
            break;
        }
    }

    return deviceInfo;
}
