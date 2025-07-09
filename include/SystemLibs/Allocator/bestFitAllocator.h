/*******************************************************************************
* bestFitAllocator.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  panjianguang <pan_jianguang@dahuatech.com>
* Version:  V1.0.0  2022-01-19 Create
* Description: bestFitAllocator Interface
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

#ifndef _BESTFITALLOCATOR_H_
#define _BESTFITALLOCATOR_H_

#include "OSA/ImousTypes.h"
#include "OSA/Mutex.h"

typedef struct chunk{
	void		*start;
	int32_t		size : 28;
	int32_t 	free : 4;
	struct chunk*	prev;
	struct chunk*	next;
}chunk_t;

struct LinkedList
{
	struct chunk *mFirst;
	struct chunk *mLast;
};

typedef struct _BestFitAllocator
{
	int32_t magicNum;
	int32_t mHeapSize;	
	struct LinkedList mList;
	imouos_mutex mLock;
}BestFitAllocator;

/// 常见bestfit函数分配器
/// \param [in] start，分配器的开始地址.
/// \param [in] size 分配器的大小.
/// \return  bestfit分配器指针
BestFitAllocator* imouos_bestFitAllocatorInit(void *start,uint32_t size);

/// 常见bestfit分配器Malloc函数
/// \param [in] allocator，bestfit分配器指针.
/// \param [in] size 分配器的大小.
/// \return 分配的指针
void* imouos_bestFitAllocatorMalloc(BestFitAllocator* allocator,uint32_t size);

/// 常见bestfit分配器Free函数
/// \param [in] allocator，bestfit分配器指针.
/// \param [in] start malloc分配的地址.
/// \return 释放的指针
void* imouos_bestFitAllocatorFree(BestFitAllocator* allocator,void* start);

/// 常见bestfit分配器打印信息
/// \param [in] allocator，bestfit分配器指针.
/// \return 无
void imouos_bestFitAllocatorInfo(BestFitAllocator* allocator);

#endif

