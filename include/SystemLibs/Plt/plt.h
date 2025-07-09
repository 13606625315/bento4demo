/*******************************************************************************
* plt.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  zhu_yuchun <zhu_yuchun@dahuatech.com>
* Version:  V1.0.0  2022-09-16 Create
* Description: plt相关接口封装
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
#ifndef __PLT_H__
#define __PLT_H__

#include "OSA/ImousTypes.h"
#include <stdint.h>

/// 产测分区获取记录接口
/// \param [in] param.
/// \param [out] value.
/// \param [in] len.
/// \return  e产测分区获取记录接口的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_pltestGetParam(char *param, char *value, int32_t length);


/// 产测流程记录接口
/// \param [in] module. 测试项
/// \param [in] plstatus. 测试结果
/// \return  产测流程记录接口的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_pltestRecord(char *module, char *plstatus);


#endif
