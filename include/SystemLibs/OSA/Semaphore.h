//
//  Copyright (c)1992-2016, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//  Description:
//  Revisions:      Year-Month-Day  SVN-Author  Modification
//

#ifndef __IMOUOS_OSAPI_SEMPHORE_H__
#define __IMOUOS_OSAPI_SEMPHORE_H__

#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************
 *
 *                            宏定义区
 *
 *******************************************************************/

#define INVALID_SEM_HANDLE (0)  /// 无效句柄

/********************************************************************
 *
 *                            类型定义区
 *
 *******************************************************************/

/// 互斥锁句柄
typedef imouos_handle imouos_sem;

/********************************************************************
 *
 *                            函数声明区
 *
 *******************************************************************/

/// 创建信号量对象
/// \param [in] psem     信号量句柄地址
/// \param [in] count 信号量计数
/// \return         IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_semCreate(imouos_sem *psem, Uint32 count);

/// 销毁信号量对象
/// \param [in] sem  句柄
/// \return         IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_semDestory(imouos_sem sem);

/// 尝试减少信号量，如果信号量已经为0，则马上返回
/// \param [in] sem  句柄
/// \return         IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_semTryPend(imouos_sem sem);

/// 减少信号量计数，如果已经减少到0，会阻塞调用的线程
/// \param [in] sem  句柄
/// \return         IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_semPend(imouos_sem sem);

/// 尝试减少信号量，如果信号量已经为0，则马上返回
/// \param [in] sem  句柄
/// \param [in] msec	最多等待事件，单位毫秒
/// \return         IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_semPendTimeout(imouos_sem sem, Uint32 msec);

/// 增加信号量计数，如果是从0累加，会唤醒其等待队列的第一个线程;
/// \param [in] sem  句柄
/// \return         IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_semPost(imouos_sem sem);

#ifdef __cplusplus
}
#endif

#endif //  __IMOUOS_INFRA_SEMPHORE_H__
