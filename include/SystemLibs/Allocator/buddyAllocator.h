/*******************************************************************************
* buddyAllocator.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  panjianguang <pan_jianguang@dahuatech.com>
* Version:  V1.0.0  2022-02-21 Create
* Description: buddyallocator Interface
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

#ifndef _BUDDYALLOCATOR_H_
#define _BUDDYALLOCATOR_H_

#include <pthread.h>
#include "OSA/ImousTypes.h"
#include "OSA/Mutex.h"

typedef struct _ChunkNode {
    uint32_t idx;
    uint32_t order;
    uint32_t bufOffset;
    uint32_t preOffset;
	uint32_t nextOffset;
} ChunkNode;

typedef struct _BufferList {
    ChunkNode head;
} BufferList;

typedef struct MemoryPoolArg
{
    uint32_t  totalSize;  ///< 内存池总字节数，由于对齐的原因，实际申请的量会更大一点。
    uint32_t  chunkSize;  ///< 内存池区块字节数，必须是2^nK 字节，也是能够申请到的内存块的最小单位。
    uint32_t  alignSize;  ///< CPacket数据区可用容量对齐字节数，必须为2^n 字节。
} MemoryPoolArg;

typedef struct _BuddyAllocator
{
	int32_t 			magicNum;
    MemoryPoolArg       arg;
    int32_t             pOriginBufferOffset;
    int32_t             pBufferOffset;
    int32_t             pChunkHeadOffset;
    int32_t             chunkMask;
    int32_t             chunkNodeLog2;
    int32_t             pBufferListOffset;
    int32_t             bufferListCount;
    int32_t             chunkCount;
    int32_t             alignSize;
	int32_t				shared;
    imouos_mutex        privMutex;
	pthread_mutex_t		shareMutex;	
} BuddyAllocator;

/// buddyAlloator分配器计算大小
/// \param [in] chunkSize,，chunk大小.
/// \param [in] chunkCount chunk的数量.
/// \return  所需申请大小
IMOUOS_API uint32_t imouos_buddyAllocatorGetBufTotalSize(uint32_t chunkSize,uint32_t chunkCount);

/// buddyAlloator分配器初始化
/// \param [in] start,分配器开始地址.
/// \param [in] chunkSize,，chunk大小.
/// \param [in] chunkCount chunk的数量.
/// \param [in] totalSize 分配器所需大小，由imouos_buddyAllocatorGetBufTotalSize计算所得.
/// \param [in] shared 是否需要进程间共享.
/// \return  buddyAlloator分配器指针
IMOUOS_API BuddyAllocator* imouos_buddyAllocatorInit(void *start,uint32_t chunkSize,uint32_t chunkCount,uint32_t totalSize,int32_t shared);

/// buddyAlloator分配器Malloc函数
/// \param [in] poolImpl，buddyAlloator分配器指针.
/// \param [in] size 分配器的大小.
/// \return 分配的指针
IMOUOS_API void* imouos_buddyAllocatorMalloc(BuddyAllocator* poolImpl,uint32_t* size);

/// buddyAlloator分配器Free函数
/// \param [in] poolImpl，buddyAlloator分配器指针.
/// \param [in] ptr 申请分配器的地址.
/// \return 释放的指针
IMOUOS_API void imouos_buddyAllocatorFree(BuddyAllocator* poolImpl,void* ptr);

/// buddyAlloator分配器打印信息函数
/// \param [in] poolImpl，buddyAlloator分配器指针.
/// \return 无
IMOUOS_API void imouos_buddyAllocatorInfo(BuddyAllocator* poolImpl);

#endif

