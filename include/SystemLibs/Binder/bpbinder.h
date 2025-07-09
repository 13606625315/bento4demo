/*******************************************************************************
* Bpbinder.h
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

#ifndef _BPBINDER_H_
#define _BPBINDER_H_

#include "OSA/ImousTypes.h"

#define BINDER_FLAG_SYNC	0
#define BINDER_FLAG_ASYNC	1

typedef struct _BpBinder_ BpBinder;

typedef int32_t (*BpBinder_Death_handle)(BpBinder *bpBinder);
typedef int32_t (*BpBinder_tranact_t)(uint32_t code, Parcel *data, Parcel* reply, uint32_t flags);
typedef int32_t (*Binder_Callback_t)(uint32_t Msg, uint32_t wParam, uint32_t lParam);

typedef struct _BpBinder_
{
	uint32_t mHandle;
	int32_t mAlive;
	int32_t mCallback;
	BpBinder_Death_handle mUserDeathHandle;
	BpBinder_Death_handle mInnerDeathHandle;
	BpBinder_tranact_t bpUserTranact;
}BpBinder;

/// 注册bpbinder死忙通知函数
/// \param [in] bpBinder，远程代理句柄.
/// \param [in] deathHandle 死忙通知函数.
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_bpbinder_linkToDeath(BpBinder *bpBinder,BpBinder_Death_handle deathHandle);

/// 销毁bpbinder死忙通知函数
/// \param [in] bpBinder，远程代理句柄.
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_bpbinder_unLinkToDeath(BpBinder *bpBinder);

/// bpbinder远程调用函数
/// \param [in] bpBinder，远程代理句柄.
/// \param [in] code,远程调用code码，可标识不同的功能区分
/// \param [in] data,输入parcel参数
/// \param [out] reply ,输出parcel参数
/// \param [in] flags ,同步异步参数
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_bpbinder_transact(BpBinder* bpBinder,uint32_t code, Parcel* data, Parcel* reply, uint32_t flags);

/// bpbinder回调函数，此函数暂时不建议使用，只支持点对点回调
/// \param [in] bpBinder，远程代理句柄.
/// \param [in] onCallback，注册回调函数.
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_bpbinder_setCallback(BpBinder* bpBinder,Binder_Callback_t onCallback);

#endif

