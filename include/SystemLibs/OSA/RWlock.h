//
//  Copyright (c)1992-2016, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//  Description:
//  Revisions:      Year-Month-Day  SVN-Author  Modification
//

#ifndef __IMOUOS_OSAPI_RWLOCK_H__
#define __IMOUOS_OSAPI_RWLOCK_H__

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

typedef imouos_handle imouos_rwlock;

typedef enum compat_rwlock_prefer
{
    compat_rwlock_prefer_read,  /** 读优先 */
    compat_rwlock_prefer_write, /** 写优先 */
}compat_rwlock_prefer_t;


/**
* 创建读写锁
* @param imouos_rwlock *prwlock
* @param prefer     优先类型
* @return           IMOUOS_ErrCode_e
*/
IMOUOS_API Int32 imouos_rwlockCreate(imouos_rwlock* prwlock, compat_rwlock_prefer_t prefer);

/**
* 销毁读写锁
* @param rwlock     读写锁句柄
* @return           IMOUOS_ErrCode_e
*/
IMOUOS_API Int32 imouos_rwlockDestory(imouos_rwlock rwlock);

/**
* 获取读锁
* @param rwlock     读写锁句柄
* @return          IMOUOS_ErrCode_e
*/
IMOUOS_API Int32 imouos_rwlockEnterR(imouos_rwlock rwlock);

/**
* 释放读锁
* @param rwlock     读写锁句柄
* @return           IMOUOS_ErrCode_e
*/
IMOUOS_API Int32 imouos_rwlockLeaveR(imouos_rwlock rwlock);

/**
* 获取写锁
* @param rwlock     读写锁句柄
* @return           IMOUOS_ErrCode_e
*/
IMOUOS_API Int32 imouos_rwlockEnterW(imouos_rwlock rwlock);

/**
* 释放写锁
* @param rwlock     读写锁句柄
* @return           IMOUOS_ErrCode_e
*/
IMOUOS_API Int32 imouos_rwlockLeaveW(imouos_rwlock rwlock);

#ifdef __cplusplus
}
#endif

#endif //  __IMOUOS_INFRA_RWLOCK_H__