/*******************************************************************************
* IPCThreadState.h
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

#ifndef _IPCTHREADSTATE_H_
#define _IPCTHREADSTATE_H_

#include "OSA/ImousTypes.h"

typedef struct _IPCThreadState
{
    ProcessState *mProcess;
 
    Parcel 		mIn;
    Parcel      mOut;
    int32_t     mLastError;
    uint32_t    mCallingPid;
   	uint32_t    mCallingUid;
    int32_t     mStrictModePolicy;
    int32_t     mLastTransactionBinderFlags;

}IPCThreadState;

/// IPCThreadState 创建函数，以线程为单元分配，每个线程都会对应不同的结构体
/// \return  IPCThreadState结构体
IMOUOS_API IPCThreadState *imouos_IPCThreadState_Self(void);

/// IPCThreadState 函数返回值，返回值，内部不创建
/// \return  IPCThreadState结构体
IMOUOS_API IPCThreadState* imouos_IPCThreadState_SelfOrNull(void);

/// parcel内存释放函数，imouos_bpbinder_transact函数调用完毕后，必须调用此函数来释放映设内存
/// \param [in] reply，imouos_bpbinder_transact函数中的reply参数
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_freeParcelBuffer(Parcel *reply);

/// 服务端获取远端进程PID函数
/// \param [in] IPCThreadState结构体
/// \return  函数远端调用PID值
IMOUOS_API pid_t imouos_getCallingPid(IPCThreadState* thiz);


/// 服务端获取远端线程UID函数
/// \param [in] IPCThreadState结构体
/// \return  函数远端线程UID值
IMOUOS_API uid_t imouos_getCallingUid(IPCThreadState* thiz);

/// IPCThreadState引用计数减一
/// \param [in] IPCThreadState结构体
/// \param [in] handle,bpBinder成员
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_decStrongHandle(IPCThreadState*thiz,int32_t handle);

/// IPCThreadState引用计数加一
/// \param [in] IPCThreadState结构体
/// \param [in] handle,bpBinder成员
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_incStrongHandle(IPCThreadState*thiz,int32_t handle);

/// IPCThreadState命令flush函数，当销毁或强制执行bpbinder命令时可调用
/// \param [in] IPCThreadState结构体
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_IPCThreadflushCommands(IPCThreadState* thiz);

#endif

