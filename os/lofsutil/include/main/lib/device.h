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
 *  @brief      平台无关化设备操作接口实现
 *  @date       2014年11月07日 15时57分28秒
 *  @author     kinuxroot, kinuxroot@163.com
 *  @group      LessOS Development Group
 *  @version    1.0
 *  @note       
 *              基本只是包含其他特定平台的实现头文件
 *
 */

#ifndef _DEVICE_H
#define _DEVICE_H


#include "type.h"

#ifdef __PLATFORM_WIN32

#include "lib/platform/win32/device.h"

#endif //__PLATFORM_WIN32

#ifdef __PLATFORM_STD

#include "lib/platform/std/device.h"

#endif	//__PLATFORM_STD

#ifdef __PLATFORM_LESSOS

#include "lib/platform/LessOS/device.h"

#endif	//__PLATFORM_STD


#endif //_DEVICE_H
