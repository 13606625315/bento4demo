//
//  "$Id$"
//
//  Copyright (c)1992-2016, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//  Description:
//  Revisions:      Year-Month-Day  SVN-Author  Modification
//
#ifndef __IMOUOS_OSAPI_UTILS_QUEUE_H__
#define __IMOUOS_OSAPI_UTILS_QUEUE_H__

#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************
 *
 *                            类型定义区
 *
 *******************************************************************/

typedef imouos_handle imouos_queue;

/********************************************************************
 *
 *                            函数声明
 *
 *******************************************************************/

/// 创建队列对象
/// \param [in]  size       队列大小
/// \return      队列句柄；0：无效句柄
IMOUOS_API Int32 imouos_queueCreate(imouos_queue *pqueue, Uint32 size);

/// 销毁队列对象
/// \param [in] que     队列句柄
/// \return             EQueueRet
IMOUOS_API Int32 imouos_queueDestory(imouos_queue que);

/// 重置队列
/// \param [in] que     队列句柄
/// \return             EQueueRet
IMOUOS_API Int32 imouos_queueReset(imouos_queue queue);

/// 将数据放到队列中
/// \param [in]  que     队列句柄
/// \param [in]  data    数据指针
/// \param [in]  timeout    队列满的超时阻塞时间
/// \return     EQueueRet
IMOUOS_API Int32 imouos_queuePush(imouos_queue queue, void* data, Uint32 timeout);

/// 从队列中获取数据  , 出队列
/// \param [in]  que     队列句柄
/// \param [in] void **ppdata 数据对象地址
/// \param [in]  timeout    队列满的超时阻塞时间
/// \return     EQueueRet
IMOUOS_API Int32 imouos_queuePop(imouos_queue queue, void **ppdata, Uint32 timeout);

/// 队列是否为空
/// \param [in]  que     队列句柄
/// \return   1:为空 0:非空
IMOUOS_API Int32 imouos_queueIsEmpty(imouos_queue queue);

/// 队列中的数据个数
/// \param [in]  que     队列句柄
/// \return     >=0 对象个数  其余 EQueueRet
IMOUOS_API Int32 imouos_queueSize(imouos_queue que);

/// 队列能力集
/// \param [in] que      队列句柄
/// \return     >=0 能力集 其余 错误
IMOUOS_API Int32 imouos_queueCapacity(imouos_queue que);

#ifdef __cplusplus
}
#endif

#endif //  __IMOUOS_UTILS_QUEUE_H__

