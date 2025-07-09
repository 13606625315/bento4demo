#ifndef __IMOUOS_OSAPI_SPINLOCK_H__
#define __IMOUOS_OSAPI_SPINLOCK_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "OSA/ImousTypes.h"
#include "OSA/IntTypes.h"

typedef imouos_handle imouos_spinlock;

/**
* 创建自旋锁
* @return           句柄
*/
IMOUOS_API Int32 imouos_spinlockCreate(imouos_spinlock *plock);

/**
* 销毁自旋锁
* @param handler    句柄
* @return           0：成功；<0：失败
*/
IMOUOS_API Int32 imouos_spinlockDestroy(imouos_spinlock lock);

/**
* 进入临界区
* @param handler    句柄
* @return           0：成功；<0：失败
*/
IMOUOS_API Int32 imouos_spinLock(imouos_spinlock lock);

/**
* 离开临界区
* @param handler    句柄
* @return           0：成功；<0：失败
*/
IMOUOS_API Int32 imouos_spinUnlock(imouos_spinlock lock);

#ifdef __cplusplus
}
#endif
#endif
