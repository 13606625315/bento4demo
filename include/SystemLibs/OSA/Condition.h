#ifndef __IMOUOS_OSAPI_CONDITION_H__
#define __IMOUOS_OSAPI_CONDITION_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"
#include "OSA/Mutex.h"

typedef imouos_handle imouos_cond;

/**
* 创建条件变量
* @return       句柄
*/
IMOUOS_API Int32 imouos_condCreate(imouos_cond *pcond);

/**
* 销毁条件变量
* @param cond   句柄
* @return       0：成功；<0：失败
*/
IMOUOS_API Int32 imouos_condDestory(imouos_cond cond);

/**
* 等待条件变量;一个线程等待"条件变量的条件成立"而挂起
* 另一个线程使"条件成立"（给出条件成立信号, imouos_condSignal）
* @param cond   句柄
* @param usec   微秒
* @return       0：成功；<0：失败
*/
IMOUOS_API Int32 imouos_condWait(imouos_cond cond, imouos_mutex mutex, unsigned timeout);

/**
* 激活条件变量,发送一个信号给另外一个正在处于阻塞等待状态的线程,使其脱离阻塞状态,
* 继续执行.如果没有线程处在阻塞等待状态,pthread_cond_signal也会成功返回.如果没有阻塞等待
* 的线程,信号将丢失
* @param cond   句柄
* @return       0：成功；<0：失败
*/
IMOUOS_API Int32 imouos_condSignal(imouos_cond cond);

#ifdef __cplusplus
}
#endif
#endif
