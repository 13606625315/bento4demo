/*******************************************************************************
* Parcel.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  panjianguang <pan_jianguang@dahuatech.com>
* Version:  V1.0.0  2022-01-10 Create
* Description: Binder Parcel Interface
*
*       1.  ó2?t?μ?÷?￡
*
*       2.  3ìDò?á11?μ?÷?￡
*
*       3.  ê1ó??μ?÷?￡
*
*       4.  ???TD??μ?÷?￡
*
*       5.  ?????μ?÷?￡
*           ?T
*
* Modification:
*    Date    :
*    Revision:
*    Author  :
*    Contents:
*******************************************************************************/

#ifndef _PARCEL_H_
#define _PARCEL_H_

#include <stdbool.h>
#include "OSA/ImousTypes.h"

#define PARCEL_MAX_CAPITAL 512

typedef struct _parcel_
{
    uint8_t     *data;
    uint32_t    mDataPos; 
	uint32_t    mDataSize;
    uint32_t    mDataCapital;
    uint32_t    *mObjects;
    uint32_t    mObjectSize;
    uint32_t    mObjectCapital;	
    int32_t     mError;
	uint32_t	mOwner;
    uint8_t  	mIPCData[PARCEL_MAX_CAPITAL];
}Parcel;

/// 设置线Parcel初始化函数,默认内部创建512字节缓存
/// \param [in] parcel 结构体，一般申明后首先需初始化
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_buf_init(Parcel *parcel);

/// 设置线Parcel初始化函数2,当需要缓存的数据大于400字节左右时，调用此函数初始化
/// \param [in] parcel 结构体
/// \param [in] buf,输入buf地址
/// \param [in] len,输入buf 长度
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_buf_init2(Parcel *parcel,uint8_t *buf,uint32_t len);

/// 往parcel写入uint32数据
/// \param [in] parcel 结构体
/// \param [in] val,写入的uint32值
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_writeUint32(Parcel *parcel,uint32_t val);

/// 往parcel读取uint32数据
/// \param [in] parcel 结构体
/// \param [in] val,读取的uint32地址
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_readUint32(Parcel *parcel,uint32_t *val);

/// 往parcel写入uint32数据数组
/// \param [in] parcel 结构体
/// \param [in] val,写入的uint32数组地址
/// \param [in] len,写入的uint32数组地址的长度
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_writeUint32Arrays(Parcel *parcel,uint32_t *val,uint32_t len);

/// 往parcel读取uint32数据数组
/// \param [in] parcel 结构体
/// \param [in] val,读取的uint32数组地址
/// \param [in] len,读取的uint32数组地址的长度
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_readUint32Arrays(Parcel *parcel,int32_t* val,uint32_t len);

/// 往parcel写入int32数据
/// \param [in] parcel 结构体
/// \param [in] val,写入的int32值
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_writeInt32(Parcel *parcel,int32_t val);

/// 往parcel读取uint32数据
/// \param [in] parcel 结构体
/// \param [in] val,读取的uint32地址
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_readInt32(Parcel *parcel,int32_t *val);

/// 往parcel写入int32数据数组
/// \param [in] parcel 结构体
/// \param [in] val,写入的int32数组地址
/// \param [in] len,写入的int32数组地址的长度
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_writeInt32Arrays(Parcel *parcel,int32_t *val,int32_t len);

/// 往parcel读取int32数据数组
/// \param [in] parcel 结构体
/// \param [in] val,读取的int32数组地址
/// \param [in] len,读取的int32数组地址的长度
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_readInt32Arrays(Parcel *parcel,int32_t *val,int32_t len);

/// 往parcel写入bool数据
/// \param [in] parcel 结构体
/// \param [in] val,写入的bool值
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_writeBool(Parcel *parcel,bool val);

/// 往parcel读取bool数据
/// \param [in] parcel 结构体
/// \param [in] val,读取的bool地址
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_readBool(Parcel *parcel,bool *val);

/// 往parcel写入char数据
/// \param [in] parcel 结构体
/// \param [in] val,写入的char值
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_writeByte(Parcel *parcel,char val);

/// 往parcel读取char数据
/// \param [in] parcel 结构体
/// \param [in] val,读取的char地址
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_readByte(Parcel *parcel,char *val);

/// 往parcel写入char数据数组
/// \param [in] parcel 结构体
/// \param [in] val,写入的char数组地址
/// \param [in] len,写入的char数组地址的长度
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_writeByteArrays(Parcel *parcel,char *val,uint32_t len);

/// 往parcel读取char数据数组
/// \param [in] parcel 结构体
/// \param [in] val,读取的char数组地址
/// \param [in] len,读取的char数组地址的长度
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_readByteArrays(Parcel *parcel,char *val,uint32_t len);

/// 往parcel写入float数据
/// \param [in] parcel 结构体
/// \param [in] val,写入的float值
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_writeFloat(Parcel *parcel,float val);

/// 往parcel读取float数据
/// \param [in] parcel 结构体
/// \param [in] val,读取的float地址
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_readFloat(Parcel *parcel,float *val);

/// 往parcel写入double数据
/// \param [in] parcel 结构体
/// \param [in] val,写入的double值
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_writeDouble(Parcel *parcel,double val);

/// 往parcel读取double数据
/// \param [in] parcel 结构体
/// \param [in] val,读取的double地址
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_readDouble(Parcel *parcel,double *val);

/// 往parcel写入pointer地址数据
/// \param [in] parcel 结构体
/// \param [in] val,写入的pointer地址
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_writePointer(Parcel *parcel,size_t val);

/// 往parcel读取dpointer地址数据
/// \param [in] parcel 结构体
/// \param [in] val,读取d的pointer地址
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_readPointer(Parcel *parcel,size_t *val);

/// 往parcel写入string数据
/// \param [in] parcel 结构体
/// \param [in] val,写入的string地址
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_writeString(Parcel *parcel,char *val);

/// 往parcel读取string数据
/// \param [in] parcel 结构体
/// \param [out] 返回string长度
/// \return  读取的string地址
IMOUOS_API char* imouos_parcel_readString(Parcel *parcel,uint32_t *len);

/// 往parcel写入string16数据
/// \param [in] parcel 结构体
/// \param [in] val,写入的string16地址
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_writeString16(Parcel *parcel,char *val);

/// 往parcel读取string16数据
/// \param [in] parcel 结构体
/// \param [out] 返回string16长度
/// \return  读取的string16地址
IMOUOS_API uint16_t* imouos_parcel_readString16(Parcel *parcel,uint32_t *len);

/// 往parcel写入void数据
/// \param [in] parcel 结构体
/// \param [in] val,写入的void数据地址
/// \param [in] len,写入的void数据长度
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_writeUserObject(Parcel *parcel,void *val,uint32_t len);

/// 往parcel读取void数据
/// \param [in] parcel 结构体
/// \param [in] val,读取的void数据地址
/// \param [in] len,读取的void数据长度
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_readUserObject(Parcel *parcel,void *val,uint32_t len);

/// 往parcel写入bnbinder数据
/// \param [in] parcel 结构体
/// \param [in] ptr,写入的bnbinder数据地址
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_writeObject(Parcel *parcel,void* ptr);

/// 往parcel读取bnbinder数据
/// \param [in] parcel 结构体
/// \param [in] ptr,读取的bnbinder数据地址
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_readObject(Parcel *parcel,void* ptr);

/// 往parcel写入fd句柄，fd可实现进程间传输
/// \param [in] parcel 结构体
/// \param [in] fd,写入fd句柄
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_writeFileDescriptor(Parcel *parcel,int fd);

/// 往parcel读取fd句柄，fd可实现进程间传输
/// \param [in] parcel 结构体
/// \param [in] fd,读取fd句柄
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_parcel_readFileDescriptor(Parcel *parcel,int *fd);


#endif

