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
 *  @brief      Windows平台设备读写实现
 *  @date       2014年11月07日 15时57分28秒
 *  @author     kinuxroot, kinuxroot@163.com
 *  @group      LessOS Development Group
 *  @version    1.0
 *  @note       
 *              符合平台无关设备操作接口
 *
 */

#ifndef _WIN32_DEVICE_H
#define _WIN32_DEVICE_H

#define DeviceOpenMode  u8
#define DEV_OM_READ     0x01
#define DEV_OM_WRITE    0x02

#define DeviceSeekMode      u8
#define DEV_SEEK_BEGIN      0x01
#define DEV_SEEK_END        0x02
#define DEV_SEEK_CURRENT    0x03

#define BYTE2SECTOR(byte)   ((byte) / 512)

#include "type.h"
#include <windows.h>

#define Device HANDLE

#define SEEK_BEG SEEK_SET
#define openDevice win32OpenDevice
#define closeDevice win32CloseDevice
#define readDevice win32ReadDevice
#define writeDevice win32WriteDevice
#define seekDevice win32SeekDevice
#define deviceOffset win32DeviceOffset
#define INVALID_DEVICE INVALID_HANDLE_VALUE

PUBLIC Device win32OpenDevice(const char* fileName, DeviceOpenMode openMode);
PUBLIC void win32CloseDevice(Device device);
PUBLIC Size win32ReadDevice(Device device, void *buffer, Size size);
PUBLIC void win32WriteDevice(Device device, char *buffer, Size size);
PUBLIC void win32SeekDevice(Device device, DeviceSeekMode seekMode, Size size);
PUBLIC Size win32DeviceOffset(Device device);

#endif //_WIN32_DEVICE_H
