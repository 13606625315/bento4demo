/*******************************************************************************
* packet.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  panjianguang <pan_jianguang@dahuatech.com>
* Version:  V1.0.0  2022-02-21 Create
* Description: packet Interface for AV Buffer
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

#ifndef _PACKET_H_
#define _PACKET_H_

#include "OSA/ImousTypes.h"

typedef uintptr_t imouos_packet;
#define IMOUOS_PACKET_INVALID	0

/*Packet Service Init*/
/// packet内存池服务端初始化，此函数在服务端调用，可实现进程间传输
/// \param [in] chunkSize,，chunk大小.
/// \param [in] chunkCount chunk的数量.
/// \return  IMemory 创建的共享packet指针
IMOUOS_API IMemory* imouos_packet_service_init(uint32_t chunkSize, uint32_t chunkCount);

/*Packet Client Init*/
/// packet内存池客户端初始化，此函数在客户端调用，可实现进程间传输
/// \param [in] chunkSize,，chunk大小.
/// \param [in] chunkCount chunk的数量.
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_packet_client_init(IMemory* memory);

/*Packet Common Init*/
/// packet申请
/// \param [in] bufferSize,申请主内存大小.
/// \param [in] extraSize，申请辅助内存大小.
/// \return  imouos_packet 返回申请packet句柄
IMOUOS_API imouos_packet imouos_packetCreate(uint32_t bufferSize, uint32_t extraSize);

/// 往packet写入主内存数据
/// \param [in] imouos_packet,packet句柄.
/// \param [in] buffer，往packet写入数据的buffer地址.
/// \param [in] buffer，往packet写入数据的length大小.
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_packetPut(imouos_packet pkt, uint8_t* buffer, uint32_t length);

/// packet得到对应的地址
/// \param [in] imouos_packet,packet句柄.
/// \return  packet对应的内存地址
IMOUOS_API uint8_t* imouos_packetGet(imouos_packet pkt);

/// packet得到对应的大小
/// \param [in] imouos_packet,packet句柄.
/// \return  packet对应的大小
IMOUOS_API int32_t imouos_packetSize(imouos_packet pkt);

/// packet调整大小
/// \param [in] imouos_packet,packet句柄.
/// \param [in] size,申请调整的大小.
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_packetResize(imouos_packet pkt, uint32_t size);

/// packet调整大小
/// \param [in] imouos_packet,packet句柄.
/// \return  packet对应的能力大小
IMOUOS_API int32_t imouos_packetCapacity(imouos_packet pkt);

/// packet得到辅助内存地址
/// \param [in] imouos_packet,packet句柄.
/// \return  packet对应的辅助内存地址
IMOUOS_API uint8_t* imouos_packetGetExtraBuffer(imouos_packet pkt);

/// packet得到辅助内存的大小
/// \param [in] imouos_packet,packet句柄.
/// \return  packet对应的辅助内存大小
IMOUOS_API int32_t imouos_packetGetExtraSize(imouos_packet pkt);

/// packet增加引用计数
/// \param [in] imouos_packet,packet句柄.
/// \return  packet增加引用计数
IMOUOS_API int32_t imouos_packetAddRef(imouos_packet pkt);

/// packet减少引用计数
/// \param [in] imouos_packet,packet句柄.
/// \return  packet减少引用计数
IMOUOS_API int32_t imouos_packetDecRef(imouos_packet pkt);

/// packet是否有效
/// \param [in] imouos_packet,packet句柄.
/// \return  是否有效值
IMOUOS_API int32_t imouos_packetIsVaild(imouos_packet pkt);

/// packet内存池信息打印
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_packetInfo(void);

/// packet对应的内存地址转成packet
/// \param [in] virtAddr,packet对应的内存地址
/// \return  imouos_packet 返回packet句柄
IMOUOS_API imouos_packet imouos_virtAddrToPacket(void* virtAddr);
#endif

