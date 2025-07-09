/*******************************************************************************
* info.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  huangxionglve <huang_xionglve@dahuatech.com>
* Version:  V1.0.0  2022-07-04 Create
* Description: info相关接口封装
*
*       1.  硬件说明。
*
*       2.  程序结构说明。
*
*       3.  使用说明。
*
*       4.  局限性说明。
*
*       5.  其他说明。
*           无
*
* Modification:
*    Date    :
*    Revision:
*    Author  :
*    Contents:
*******************************************************************************/
#ifndef __INFO_H__
#define __INFO_H__

#include "OSA/ImousTypes.h"
#include <stdint.h>

#define UCI_COMMIT 1
#define UCI_UNCOMMIT 0

/// uci封装写配置函数
/// \param [in] key.  
/// option在uci语法中表示三级配置下，当前key仅支持option三级配置的设置，即a1.a2.a3样式，必须要有两个点，否则会死机
/// \param [in] value.
/// \param [in] commit 0:不提交 1：提交  
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_uci_set_option(char *key,char *value,uint32_t commit);

/// 注册bpbinder死忙通知函数
/// \param [in] key.
/// option在uci语法中表示三级配置下，当前key仅支持option三级配置的设置，即a1.a2.a3样式，必须要有两个点
/// \param [out] value.
/// \param [in] len.
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_uci_get_option(char *key,char *value,uint32_t len);


/// env环境变量获取接口
/// \param [in] param.
/// \param [out] value.
/// \param [in] len.
/// \return  env环境变量获取接口的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_envGetBootParam(char *param, char *value, int32_t length);

/// env环境变量设置接口
/// \param [in] param.
/// \param [in] value.
/// \return  env环境变量设置接口的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_envSetBootParam(char *param, const char *value);

/// env环境变量显示接口
/// \param [in] param.
/// \param [in] value.
/// \return  env环境变量显示接口的返回状态IMOUOS_ErrCode_e
IMOUOS_API void imouos_envShowAllPara(void);


#endif
