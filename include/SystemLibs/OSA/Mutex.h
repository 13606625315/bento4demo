//
//  Copyright (c)1992-2016, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//  Description:
//  Revisions:      Year-Month-Day  SVN-Author  Modification
//

#ifndef __IMOUOS_OSAPI_MUTEX_H__
#define __IMOUOS_OSAPI_MUTEX_H__

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

#define INVALID_MUTEX_HANDLE 0  /// 无效句柄

/********************************************************************
 *
 *                            类型定义区
 *
 *******************************************************************/

/// 锁句柄
typedef imouos_handle imouos_mutex;

/// 定义锁的类型
typedef enum EMutexType
{
    EMutexType_Normal = 0,       ///<普通锁
    EMutexType_Recursive         ///<递归锁
} EMutexType;

/********************************************************************
 *
 *                            函数声明区
 *
 *******************************************************************/

/// 创建锁对象
/// \param [in] imouos_mutex *pmutex 互斥锁句柄地址
/// \param [in] EMutexType type 互斥锁类型
/// \return             IMOUOS_ErrCode_e 返回码
IMOUOS_API Int32 imouos_mutexCreate(imouos_mutex *pmutex, EMutexType type);

/// 销毁锁对象
/// \param [in] mutex  句柄
/// \return           IMOUOS_ErrCode_e 返回码
IMOUOS_API Int32 imouos_mutexDestroy(imouos_mutex mutex);

/// 锁进入临界区
/// \param [in] mutex  句柄
/// \return           IMOUOS_ErrCode_e 返回码
IMOUOS_API Int32 imouos_mutexLock(imouos_mutex mutex);

/// 锁尝试进入临界区
/// \param [in] mutex  句柄
/// \return           IMOUOS_ErrCode_e 返回码
IMOUOS_API Int32 imouos_mutexTryLock(imouos_mutex mutex);

/// 锁离开临界区
/// \param [in] mutex  句柄
/// \return           IMOUOS_ErrCode_e 返回码
IMOUOS_API Int32 imouos_mutexUnlock(imouos_mutex mutex);

#ifdef __cplusplus
}
#endif

#endif //  __IMOUOS_INFRA_MUTEX_H__
