/*******************************************************************************
* Bnbinder.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  panjianguang <pan_jianguang@dahuatech.com>
* Version:  V1.0.0  2022-01-10 Create
* Description: Binder Bnbinder Interface
*
*       1.  ó2?t?μ?÷?￡
*
*       2.  3ìDò?á11?μ?÷?￡
*
*       3.  ê1ó??μ?÷?￡
*
*       4.  ???TD??μ?÷?￡
*
*       5.  ?????μ?÷?￡
*           ?T
*
* Modification:
*    Date    :
*    Revision:
*    Author  :
*    Contents:
*******************************************************************************/

#ifndef _BNBINDER_H_
#define _BNBINDER_H_

#include "OSA/ImousTypes.h"

typedef struct _BnBinder_ BnBinder;

typedef int32_t (*BnBinder_handle_t)(uint32_t code, Parcel *data, Parcel* reply, uint32_t flags);
typedef int32_t (*BnBinder_dump_t)(int32_t argc,char* argv[]);
typedef int32_t (*Binder_innerCallback_t)(BnBinder* bnBinder,uint32_t code,Parcel *data, Parcel* reply, uint32_t flags);

typedef struct _BnBinder_
{
	char Token[32];
	BnBinder_handle_t onTransact;
	Binder_innerCallback_t onInnerCallback;
	Binder_Callback_t onCallback;
	BpBinder *bpCallback;
	BnBinder_dump_t dump;
}BnBinder;

/// 创建binder服务端函数
/// \param [in] name  binder服务名，必须是全局唯一的。
/// \param [in] onTransact binder服务端回调函数,该服务所有接受到的消息都会回调此函数，通过回调函数code区分消息类型。
/// \return  BnBinder 创建完毕的bnbinder结构体，供addService使用
IMOUOS_API BnBinder* imouos_bnbinder_create(char* name, BnBinder_handle_t onTransact);

/// 创建binder服务端dump调式注册函数
/// \param [in] bnbinder： imouos_bnbinder_create函数创建的binder结构体
/// \param [in] BnBinder_dump_t 注册的dump回调函数，通过dumpsys工具可实时回调到此函数
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_bnbinder_registerDump(BnBinder* bnbinder, BnBinder_dump_t onDump);

/// 创建binder服务端回调函数，目前不建议使用
/// \param [in] bnbinder： imouos_bnbinder_create函数创建的binder结构体
/// \param [in] Msg 回调函数msg事件
/// \param [in] wParam 回调函数wParam参数
/// \param [in] lParam 回调函数lParam参数
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_bnBinder_doCallback(BnBinder *binder,uint32_t Msg, uint32_t wParam, uint32_t lParam);

#endif
