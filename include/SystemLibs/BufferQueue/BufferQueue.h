/*******************************************************************************
* MapQueue.h
*
* Copyright (C) 2021-2023 Hangzhou Huacheng Technology CO.,LTD.
*
* Author : longjie<long_jie@dahuatech.com>
* Version: V1.0.0  2023-03-06 Create
*
* Description: 地图内存管理模块接口
*
*       1. 硬件说明。
*          无。
*
*       2. 程序结构说明。
*          调用流程图:
* \Code
*         ===========================
*                   Public
*                     |
*             bufferQueue_ProducerCreateBuffer
*                     |
*                   Public
*             
*                  Producer
*                     |
*             bufferQueue_ProducerConnect
*                     |
*             bufferQueue_ProducerDequeueBuffer
*                     |
*             bufferQueue_ProducerQueueBuffer         
*                     |
*                  Producer
*                  
*                  Consumer
*                     |
*             bufferQueue_ConsumerConnect
*                     |
*             bufferQueue_ConsumerAcquireBuffer
*                     |
*                  DoYours
*                     |
*             bufferQueue_ConsumerReleaseBuffer
*                     |
*                 Consumer
*         ===========================
* \endcode
*
*       3. 使用说明。
*          无。
*
*       4. 局限性说明。
*          无。
*
*       5. 其他说明。
*          无。
*
* Modification:
*     Date    : 2023-03-06 Create
*     Revision:
*     Author  :
*     Contents: 地图内存管理模块接口
*               提供地图内存的申请释放关联等操作  ，减少拷贝和频繁申请释放
*******************************************************************************/

#ifndef __IMOU_MAPQUEUE_H__
#define __IMOU_MAPQUEUE_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


/// 缓存槽集合
typedef struct BufferQueueSlots
{
    int32_t mSlot;      // slot编号，生产者自定义[0, intMax]
    int32_t bufSize;    // 缓存单元大小，生产者自定义
    int32_t bufNum;     // 缓存单元数量
}BufferQueueSlots;

/// 基本单位
typedef struct BufferQueueElem
{
    uint8_t* bufAddr;   // buf地址
    int32_t  bufSize;   // buf长度
    int32_t  bufSlot;   // buf编号
}BufferQueueElem;

/// Producer：当某个slot对应的空闲buf数量改变时回调告知生产者。出参为编号对应空闲数量
/// Consumer：当某个slot对应的可取buf数量改变时回调通知消费者。出参为编号对应可取数量
typedef int32_t (*bufferQueueChangeCallback)(int32_t mSlot, int32_t bufNum);

/// Public：缓存池初始化
///
/// \param [in]  mSlots:内存槽属性
/// \param [in]  arraySize:内存槽组数
/// \param [out] 无
/// \return 见IMOUOS_ErrCode_e
int32_t bufferQueue_CreateBuffer(BufferQueueSlots *mSlots, int32_t arraySize);

/// Public：获取最大slot数量
///
/// \param [in]  无
/// \param [in]  无
/// \param [out] 无
/// \return arraySize
int32_t bufferQueue_GetMaxSlots();

/// Producer：链接bufferQueue
///
/// \param [in]  bufCallBack:bufferQueue回调，slot对应空闲buf数量，改变时回调告知生产者
/// \param [out] mSlots   :内存槽属性，mSlots不是必要的参数，如果不关心可以传空
/// \param [out] arraySize:内存槽组数
/// \return 见IMOUOS_ErrCode_e
int32_t bufferQueue_ProducerConnect(bufferQueueChangeCallback bufCallBack, BufferQueueSlots *mSlots, int32_t *arraySize);

/// Producer：指定内存槽编号获取内存缓冲区，空闲slot从回调获取
///
/// \param [in]  mSlot: 槽编号
/// \param [in]  timeOut :超时等待时间
/// \param [out] bufElem :缓存元素结构
/// \return 缓存元素指针 :如果为空则无效
int32_t  bufferQueue_ProducerDequeueBuffer(int32_t mSlot, uint64_t timeOut, BufferQueueElem* bufElem);

/// Producer：内存槽绑定数据
///
/// \param [in]  bufAddr :缓冲区地址
/// \param [out] 无
/// \return 见IMOUOS_ErrCode_e
int32_t bufferQueue_ProducerQueueBuffer(BufferQueueElem* bufAddr);

/// Consumer：链接bufferQueue
///
/// \param [in]  bufCallBack:bufferQueue回调，slot对应可取buf数量，改变时回调告知消费者
/// \param [out] mSlots   :内存槽属性，mSlots不是必要的参数，如果不关心可以传空
/// \param [out] arraySize:内存槽组数
/// \return 见IMOUOS_ErrCode_e
int32_t bufferQueue_ConsumerConnect(bufferQueueChangeCallback bufCallBack, BufferQueueSlots *mSlots, int32_t *arraySize);

/// Consumer：获取槽内存数据，可取slot从回调获取
///
/// \param [in]  mSlot   :内存槽编号
/// \param [in]  timeOut :超时等待时间
/// \param [out] bufElem :缓存元素结构
/// \return 缓存元素指针 :如果为空则无效
int32_t bufferQueue_ConsumerAcquireBuffer(int32_t mSlot, uint64_t timeOut, BufferQueueElem* bufElem);

/// Consumer：释放地图内存使用权
///
/// \param [in]  pQueueElem: 缓存元素指针
/// \param [out] 无
/// \return 见IMOUOS_ErrCode_e
int32_t bufferQueue_ConsumerReleaseBuffer(BufferQueueElem *bufElem);

/// Public：缓存池销毁
///
/// \param [in]  无
/// \param [in]  无
/// \param [out] 无
/// \return 见IMOUOS_ErrCode_e
int32_t bufferQueue_DestroyBuffer();

#ifdef __cplusplus
}
#endif //__cplusplus
#endif //__IMOU_MAPQUEUE_H__

