#include "lib/platform/LessOS/device.h"
#include "hd.h"
#include "type.h"
#include "proto.h"


/**
 * @brief 相当于malloc，获取全局DeviceM数组的空间，若没有则返回NULL
 *
 * @return Device;
 */
PUBLIC Device deviceMalloc()
{
	int i=0;
	for(i = 0; i < NR_DEVICE_MALLOC; i++)
	{
		if(deviceM[i].size == 0)
			return &deviceM[i];
	}
	return NULL;
}

/**
* @brief 相当于free，释放全局DeviceM数组的空间
*/
PUBLIC void deviceFree(Device device)
{
	memset(device, 0, sizeof(Device));
}

/**
 * @brief 使用给定的文件名和打开模式打开设备
 *
 * @param fileName 设备文件名
 * @param openMode 文件打开模式，参见DeviceOpenMode列表
 *
 * @return 打开的设备对象
 */
PUBLIC Device lessosOpenDevice(const char* deviceName, DeviceOpenMode openMode){

	char str[4] = "sda";
	if(strcmp(str, deviceName))
		return NULL;

	struct hd_info* hdi = kernelHdOpen(0);
	Device device = deviceMalloc();
	if(device == NULL)
		return NULL;
	device->deviceNo = 0;
	device->position = 0;
	device->size = hdi->hd_size;

	return device;
}

/**
 * @brief 关闭指定设备对象
 *
 * @param device 设备对象
 *
 * @return 无返回值
 */
PUBLIC void lessosCloseDevice(Device device){
	
	if(device->size != 0){
		//从设备列表列表中删除设备信息节点，并释放节点，关闭设备
		kernelHdClose(device->deviceNo);
		deviceFree(device);
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
//	PUBLIC void hdRead(u32 device, u64 position, u32 count, char* buf)
PUBLIC Size lessosReadDevice(Device device, void *buffer, Size size){
	
	kernelHdRead(device->deviceNo, device->position, size, buffer);
	device->position += size;
	return size;	
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
PUBLIC Size lessosWriteDevice(Device device, void *buffer, Size size){
	kernelHdWrite(device->deviceNo, device->position, size, buffer);
	device->position += size;
	return 0;
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
PUBLIC void lessosSeekDevice(Device device, DeviceSeekMode seekMode, Size size){
	switch(seekMode)
	{
	case DEV_SEEK_BEGIN:
		device->position = size;
		break;

	case DEV_SEEK_END:
		device->position = device->size - size;
		break;
	
	case DEV_SEEK_CURRENT:
		device->position += size;
		break;
	}
//	disp_str("seek position = ");
//	disp_int(device->position);
}

/**
 * @brief 返回当前设备指针的偏移
 *
 * @param device 设备对象
 *
 * @return 当前设备指针偏移
 */
PUBLIC Size lessosDeviceOffset(Device device){

	return device->position;
}

