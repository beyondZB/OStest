#include "lib/platform/LessOS/device.h"
#include "lib/list.h"
#include "hd.h"


// 设备信息结构体
typedef struct {
    // C标准库文件对象指针，代表设备文件
    Device device;
    // 链表节点，用于连接各个节点
    ListNode listNode;
} DeviceInfo;

// 设备列表
List deviceList;
// 设备列表是否初始化
PRIVATE int isDeviceListInited = 0;
PRIVATE DeviceInfo *findDeviceInfo(Device device);
#define NR_DEVICE_INFO_MALLOC	5
DeviceInfo deviceInfoM[NR_DEVICE_INFO_MALLOC] = {{NULL, NULL}};


/**
 * @brief 相当于malloc，获取全局DeviceInfoM数组的空间，若没有则返回NULL
 *
 * @return DeviceInfo*;
 */
PUBLIC DeviceInfo * deviceInfoMalloc()
{
	int i=0;
	for(i = 0; i < NR_DEVICE_INFO_MALLOC; i++)
	{
		if(deviceInfoM[i]->device == NULL && ListNode == NULL)
			return deviceInfo[i];
	}
	return NULL;
}

/**
* @brief 相当于free，释放全局DeviceInfoM数组的空间
*/
PUBLIC DeviceInfo * deviceInfoFree(DeviceInfo* deviceInfo)
{
	deviceInfo.device = NULL;
	deviceInfo.listNode = NULL;
}

/**
 * @brief 使用给定的文件名和打开模式打开设备
 *
 * @param fileName 设备文件名
 * @param openMode 文件打开模式，参见DeviceOpenMode列表
 *
 * @return 打开的设备对象
 */
PUBLIC Device lessosOpenDevice(const char* fileName, DeviceOpenMode openMode){
//    Device device = fopen(fileName, openMode);
//
//    // 创建设备信息
//    DeviceInfo *deviceInfo = (DeviceInfo*)malloc(sizeof(DeviceInfo));
//    deviceInfo->file = device;
//
//    // 如果设备列表尚未初始化，首先初始化设备列表
//    if ( !isDeviceListInited ) {
//        isDeviceListInited = 1;
//        listInit(&deviceList);
//    }
//    // 将设备信息添加到设备列表中
//    listAdd(&deviceList, deviceInfo);
//
//    return device;

	char str[4] = "sda";
	for(int i = 0; i < 4; i++){
		if(str[i] != fileName[i])
			return NULL;
	}

	struct hd_info* hdi = hdOpen(0);
	Device device;
	device.deviceNo = 0;
	device.position = 0;
	device.size = hdi->hd_size;

	// 创建设备信息
	DeviceInfo *deviceInfo = deviceInfoMalloc();
	if(deviceInfo != NULL)
		deviceInfo.device = device;
	else
		return NULL;

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
PUBLIC void lessosCloseDevice(Device device){
//    DeviceInfo *deviceInfo = findDeviceInfo(device);
//    if ( deviceInfo ) {
//        // 从设备列表中移除设备信息节点，并释放节点，关闭文件
//        listRemove(deviceInfo);
//        free(deviceInfo);
//        fclose(device);
//    }
	
	DeviceInfo *deviceInfo = findDeviceInfo(device);
	if(deviceInfo){
		//从设备列表列表中删除设备信息节点，并释放节点，关闭设备
		listRemove(deviceInfo);
		hdClose(deviceInfo->device->deviceNo);
		deviceInfoFree(deviceInfo);
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
//	Size readSize = fread(buffer, size, 1, device);
//	
//	return readSize;
	
	hdRead(device->deviceNo, device->position, size, buffer);
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
//    Size writtenSize = fwrite(buffer, size, 1, device);
	hdWrite(device->deviceNo, device->position, size, buffer);
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
}

/**
 * @brief 返回当前设备指针的偏移
 *
 * @param device 设备对象
 *
 * @return 当前设备指针偏移
 */
PUBLIC Size stdDeviceOffset(Device device){
//    return ftell(device);

	return device->position;
}

/**
 * @brief 根据设备对象查找设备信息节点
 *
 * @param device 设备对象
 *
 * @return 设备信息节点指针
 */
PRIVATE DeviceInfo *findDeviceInfo(Device device){
//    DeviceInfo *deviceInfo = NULL;
//    listForEach(DeviceInfo, deviceInfo, &deviceList) {
//        if ( deviceInfo->file == device ) {
//            break;
//        }
//    }
//
//    return deviceInfo;

	DeviceInfo *deviceInfo = NULL;
	listForEach(DeviceInfo, deviceInfo, &deviceList){
		if( deviceInfo->device == device ){
			break;
		}
	}

	return deviceInfo;
}
