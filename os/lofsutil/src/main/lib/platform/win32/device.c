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
 *  @brief      Windows平台设备读写实现
 *  @date       2014年11月07日 15时57分28秒
 *  @author     kinuxroot, kinuxroot@163.com
 *  @group      LessOS Development Group
 *  @version    1.0
 *  @note
 *
 */

#ifdef __PLATFORM_WIN32

#include "lib/platform/win32/device.h"
#include "lib/list.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

typedef struct {
    HANDLE handle;
    Size offset;
    ListNode listNode;

    struct {
        char data[512];
        u8 dirty;
        u8 available;
    } buffer;
} DeviceInfo;

List deviceList;
PRIVATE int isDeviceListInited = 0;
PRIVATE DeviceInfo *findDeviceInfo(Device device);
PRIVATE Size setFilePointer(HANDLE file, Size size, DeviceSeekMode seekMode);
PRIVATE void flushBuffer(DeviceInfo *deviceInfo);
PRIVATE void fillBuffer(DeviceInfo *deviceInfo);
PRIVATE void writeBackBuffer(DeviceInfo *deviceInfo);
PRIVATE Size win32ReadFile(HANDLE file, void *buffer, Size size);
PRIVATE Size win32WriteFile(HANDLE file, void *buffer, Size size);

PUBLIC Device win32OpenDevice(const char* fileName, DeviceOpenMode openMode){
    DWORD desireAccess = 0;

    if ( openMode & DEV_OM_READ ) {
        printf("read\n");
        desireAccess |= GENERIC_READ;
    }

    if ( openMode & DEV_OM_WRITE ) {
        printf("write\n");
        desireAccess |= GENERIC_WRITE;
    }

    Device device = CreateFile(
            TEXT(fileName),
            desireAccess,
            FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

    DeviceInfo *deviceInfo = (DeviceInfo*)malloc(sizeof(DeviceInfo));
    deviceInfo->handle = device;
    deviceInfo->offset = 0;
    deviceInfo->buffer.dirty = 0;
    deviceInfo->buffer.available = 0;

    if ( !isDeviceListInited ) {
        isDeviceListInited = 1;
        listInit(&deviceList);
    }
    listAdd(&deviceList, deviceInfo);

    return device;
}

PUBLIC void win32CloseDevice(Device device){
    DeviceInfo *deviceInfo = findDeviceInfo(device);
    if ( deviceInfo ) {
        listRemove(deviceInfo);
        free(deviceInfo);
        CloseHandle(device);
    }
}

PUBLIC Size win32ReadDevice(Device device, void *buffer, Size size){
    DeviceInfo *deviceInfo = findDeviceInfo(device);

    if ( deviceInfo ) {
        flushBuffer(deviceInfo);

        Size totalReadBytes = 0;
        Size currentSectorOffset = deviceInfo->offset % 512;
        Size currentSectorRemainBytes = 512 - currentSectorOffset;
        Size firstReadBytes = MIN(currentSectorRemainBytes, size);
        if ( firstReadBytes > 0 ) {
            if ( !deviceInfo->buffer.available ) {
                printf("Read first sector\n");
                fillBuffer(deviceInfo);
            }
            memcpy(buffer, deviceInfo->buffer.data + currentSectorOffset, firstReadBytes);
            totalReadBytes += firstReadBytes;
            deviceInfo->offset += firstReadBytes;
            buffer += firstReadBytes;
            printf("win32ReadDevice, deviceInfo: %p\n", deviceInfo);
            printf("win32ReadDevice, offset: %d\n", deviceInfo->offset);
        }

        while ( totalReadBytes < size ) {
            Size onceReadBytes = MIN(512, size - totalReadBytes);
            fillBuffer(deviceInfo);
            memcpy(buffer, deviceInfo->buffer.data, onceReadBytes);
            totalReadBytes += onceReadBytes;
            deviceInfo->offset += onceReadBytes;
            buffer += onceReadBytes;
        }

        if ( deviceInfo->offset % 512 == 0 ) {
            fillBuffer(deviceInfo);
        }

        return totalReadBytes;
    }

    return -1;
}

PUBLIC void win32SeekDevice(Device device, DeviceSeekMode seekMode, Size size){
    DeviceInfo *deviceInfo = findDeviceInfo(device);
    if ( deviceInfo ) {
        Size newOffset = setFilePointer(device, size, seekMode);
        if ( deviceInfo->buffer.available &&
                (BYTE2SECTOR(newOffset) != 
                 BYTE2SECTOR(deviceInfo->offset)) ) {
            flushBuffer(deviceInfo);
            deviceInfo->buffer.available = 0;
        }
        deviceInfo->offset = newOffset;
    }
}

PUBLIC Size win32DeviceOffset(Device device){
    DeviceInfo *deviceInfo = findDeviceInfo(device);

    if ( deviceInfo ) {
        printf("win32DeviceOffset, deviceInfo: %p\n", deviceInfo);
        printf("win32DeviceOffset, offset: %d\n", deviceInfo->offset);
        return deviceInfo->offset;
    }
    return -1;
}

PRIVATE DeviceInfo *findDeviceInfo(Device device){
    DeviceInfo *deviceInfo = NULL;
    listForEach(DeviceInfo, deviceInfo, &deviceList) {
        if ( deviceInfo->handle == device ) {
            break;
        }
    }

    return deviceInfo;
}

PRIVATE Size setFilePointer(HANDLE file, Size size, DeviceSeekMode seekMode){
    DWORD moveMethod = -1;
    switch ( seekMode ) {
    case DEV_SEEK_BEGIN:
        moveMethod = FILE_BEGIN;
        break;
    case DEV_SEEK_CURRENT:
        moveMethod = FILE_CURRENT;
        break;
    case DEV_SEEK_END:
        moveMethod = FILE_END;
    }

    if ( moveMethod != -1 ) {
        DWORD lowPart = ((i64_parts*)&size)->low_part;
        DWORD highPart = ((i64_parts*)&size)->high_part;
        DWORD newOffsetLowPart = SetFilePointer(
                file, lowPart, &highPart, moveMethod);
        ((i64_parts*)&size)->high_part = highPart;

        return size;
    }

    return -1;
}

PRIVATE void flushBuffer(DeviceInfo *deviceInfo){
    if ( deviceInfo->buffer.dirty ) {
        writeBackBuffer(deviceInfo);
    }
}

PRIVATE void fillBuffer(DeviceInfo *deviceInfo){
    printf("fillBuffer, offset: %d\n", deviceInfo->offset);
    Size newOffset = setFilePointer(deviceInfo->handle, BYTE2SECTOR(deviceInfo->offset) * 512, DEV_SEEK_BEGIN);
    printf("fillBuffer, newoffset: %d\n", newOffset);
    Size actualSize = win32ReadFile(deviceInfo->handle, deviceInfo->buffer.data, 512);
    printf("fillBuffer, actualSize: %d\n", actualSize);
    deviceInfo->buffer.dirty = 0;
    deviceInfo->buffer.available = 1;
}

PRIVATE void writeBackBuffer(DeviceInfo *deviceInfo){
    Size beginOffset = BYTE2SECTOR(deviceInfo->offset) * 512;
    setFilePointer(deviceInfo->handle, beginOffset, DEV_SEEK_BEGIN);
    win32WriteFile(deviceInfo->handle, deviceInfo->buffer.data, 512);
    deviceInfo->buffer.dirty = 0;
}

PRIVATE Size win32ReadFile(HANDLE file, void *buffer, Size size){
    DWORD actualSize = 0;
    printf("win32ReadFile, size: %d\n", size);
    BOOL isSuc = ReadFile(file, buffer, size, &actualSize, NULL);
    printf("win32ReadFile, isSuc: %d\n", isSuc);

    return actualSize;
}

PRIVATE Size win32WriteFile(HANDLE file, void *buffer, Size size){
    DWORD actualSize = 0;
    WriteFile(file, buffer, size, &actualSize, NULL);

    return actualSize;
}

#endif // __PLATFORM_WIN32
