#ifndef _LESSOS_DEVICE_H
#define _LESSOS_DEVICE_H

#include "type.h"
#include "const.h"

// 设备打开模式
typedef const unsigned char DeviceOpenMode;
// 只读
#define DEV_OM_READ     0
// 可读写
#define DEV_OM_WRITE    1

// 设备指针移动模式
#define DeviceSeekMode      int
// 从开始向前移动
#define DEV_SEEK_BEGIN      0
// 从结尾向后移动
#define DEV_SEEK_END        1
// 从当前位置处移动
#define DEV_SEEK_CURRENT    2

#define BYTE2SECTOR(byte)   ((byte) / 512)


typedef struct {
	u32	deviceNo;
	u64	position;
	u64	size;
	u16	mode;
} *Device, DeviceObject;

#define NR_DEVICE_MALLOC	5
DeviceObject deviceM[NR_DEVICE_MALLOC];

#define openDevice lessosOpenDevice
#define closeDevice lessosCloseDevice
#define readDevice lessosReadDevice
#define writeDevice lessosWriteDevice
#define seekDevice lessosSeekDevice
#define deviceOffset lessosDeviceOffset
#define INVALID_DEVICE NULL

PUBLIC Device lessosOpenDevice(const char* fileName, DeviceOpenMode openMode);
PUBLIC void lessosCloseDevice(Device device);
PUBLIC Size lessosReadDevice(Device device, void *buffer, Size size);
PUBLIC Size lessosWriteDevice(Device device, void *buffer, Size size);
PUBLIC void lessosSeekDevice(Device device, DeviceSeekMode seekMode, Size size);
PUBLIC Size lessosDeviceOffset(Device device);

#endif // _LESSOS_DEVICE_H
