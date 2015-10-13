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

/** @file       device.h
 *  @brief      基于C标准库的设备读写实现
 *  @date       2014年11月07日 15时57分28秒
 *  @author     kinuxroot, kinuxroot@163.com
 *  @group      LessOS Development Group
 *  @version    1.0
 *  @note       
 *              在Unix和类Unix等将设备模拟为文件的系统中可以使用
 *              符合平台无关设备操作接口
 *
 */

#ifndef _STD_DEVICE_H
#define _STD_DEVICE_H

#include <stdio.h>

// 设备打开模式
typedef const char* DeviceOpenMode;
// 只读
#define DEV_OM_READ     "rb"
// 可读写
#define DEV_OM_WRITE    "rb+"

// 设备指针移动模式
#define DeviceSeekMode      int
// 从开始向前移动
#define DEV_SEEK_BEGIN      SEEK_SET
// 从结尾向后移动
#define DEV_SEEK_END        SEEK_END
// 从当前位置处移动
#define DEV_SEEK_CURRENT    SEEK_CUR

#define BYTE2SECTOR(byte)   ((byte) / 512)

#include "type.h"

typedef FILE* Device;

#define openDevice stdOpenDevice
#define closeDevice stdCloseDevice
#define readDevice stdReadDevice
#define writeDevice stdWriteDevice
#define seekDevice stdSeekDevice
#define deviceOffset stdDeviceOffset
#define INVALID_DEVICE NULL

PUBLIC Device stdOpenDevice(const char* fileName, DeviceOpenMode openMode);
PUBLIC void stdCloseDevice(Device device);
PUBLIC Size stdReadDevice(Device device, void *buffer, Size size);
PUBLIC Size stdWriteDevice(Device device, void *buffer, Size size);
PUBLIC void stdSeekDevice(Device device, DeviceSeekMode seekMode, Size size);
PUBLIC Size stdDeviceOffset(Device device);

#endif // _STD_DEVICE_H
