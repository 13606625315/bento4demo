/*******************************************************************************
* imemory.c
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  panjianguang <pan_jianguang@dahuatech.com>
* Version:  V1.0.0  2022-01-12 Create
* Description: Binder Bnbinder Interface
*
*       1.  硬件说明。
*
*       2.  程序结构说明。
*
*       3.  使用说明。
*
*       4.  局限性说明。
*
*       5.  其他说明。
*           无
*
* Modification:
*    Date    :
*    Revision:
*    Author  :
*    Contents:
*******************************************************************************/

#ifndef _IMEMORY_H_
#define _IMEMORY_H_

#include "OSA/ImousTypes.h"

typedef struct _IMemory_
{
	uint8_t *base;
	int32_t fd;
	uint32_t size;
	uint32_t offset;
}IMemory;

/// 创建shm共享内存
/// \param [in] name，申请共享内存名字.
/// \param [in] size 申请共享内存大小
/// \param [out] memory 创建共享内存句柄.
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_create_shmMemory(char *name,uint32_t size, IMemory *memory);

/// 通过binder传输共享内存
/// \param [in] data 传输依赖的parcel.
/// \param [in] memory 传输的共享内存句柄
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_put_shmMemory(Parcel *data, IMemory *memory);

/// 通过binder得到共享内存
/// \param [in] data 传输依赖的parcel.
/// \param [in] memory 传输的共享内存句柄
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_get_shmMemory(Parcel *data, IMemory *memory);

/// 锁定申请共享内存中的某一段
/// \param [in] memory 传输的共享内存句柄
/// \param [in] offset 传输的共享内存偏移
/// \param [in] len 传输的共享内存大小
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_pin_shmMemory(IMemory *memory,size_t offset, size_t len);

/// 释放申请共享内存中的某一段
/// \param [in] memory 传输的共享内存句柄
/// \param [in] offset 传输的共享内存偏移
/// \param [in] len 传输的共享内存大小
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_unpin_shmMemory(IMemory *memory,size_t offset, size_t len);

#endif

