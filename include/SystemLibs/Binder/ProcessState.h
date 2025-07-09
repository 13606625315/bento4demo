/*******************************************************************************
* ProcessState.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  panjianguang <pan_jianguang@dahuatech.com>
* Version:  V1.0.0  2022-01-10 Create
* Description: Binder Bpbinder Interface
*
*       1.  Ó²¼þËµÃ÷¡£
*
*       2.  ³ÌÐò½á¹¹ËµÃ÷¡£
*
*       3.  Ê¹ÓÃËµÃ÷¡£
*
*       4.  ¾ÖÏÞÐÔËµÃ÷¡£
*
*       5.  ÆäËûËµÃ÷¡£
*           ÎÞ
*
* Modification:
*    Date    :
*    Revision:
*    Author  :
*    Contents:
*******************************************************************************/

#ifndef _PROCESSSTATE_H_
#define _PROCESSSTATE_H_

#include <pthread.h>
#include "OSA/ImousTypes.h"

#define USER_MAX_BINDER_THREADS 4

typedef enum
{
    BINDER_THREADLOOP_NONBLOCK = 0,
    BINDER_THREADLOOP_BLOCK = 1,
}BINDER_THREADLOOP_TYPE;

typedef struct _ProcessState
{
    int32_t    mDriverFD;
    void*  mVMStart;
	int  mThreadPoolStarted;
	int  mThreadPoolSeq;
	char mThreadName[USER_MAX_BINDER_THREADS][20];
    pthread_mutex_t mLock;
    // Protects thread count variable below.
    pthread_mutex_t     mThreadCountLock;
    pthread_cond_t      mThreadCountDecrement;
    // Number of binder threads current executing a command.
    uint32_t    mExecutingThreadsCount;
   	// Maximum number for binder threads allowed for this process.
    uint32_t    mMaxThreads;

	// Time when thread pool was emptied
	int64_t   mStarvationStartTimeMs;
}ProcessState;

/// 设置ProcessState线程池最大值
/// \param [in] thiz，ProcessState句柄，每个进程对应唯一一个
/// \param [in] maxThreads 线程数最大值
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_setThreadPoolMaxThreadCount(ProcessState *thiz,int maxThreads) ;

/// 开启线程池函数
/// \param [in] thiz，ProcessState句柄，每个进程对应唯一一个
/// \param [in] block 是否阻塞，阻塞时此函数不返回，非阻塞时马上返回
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_startThreadPool(ProcessState *thiz,int32_t block);

/// 创建ProcessState句柄，每个进程对应唯一一个
/// \return  thiz，ProcessState句柄，每个进程对应唯一一个
IMOUOS_API ProcessState *imouos_ProcessState_Self(void);

/// 查询ProcessState句柄，每个进程对应唯一一个
/// \return  thiz，ProcessState句柄，每个进程对应唯一一个
IMOUOS_API ProcessState *imouos_ProcessState_selfOrNull(void);

/// 销毁ProcessState句柄，每个进程对应唯一一个
/// \param [in] thiz，ProcessState句柄，每个进程对应唯一一个
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_ProcessState_Del(ProcessState *thiz);

#endif

