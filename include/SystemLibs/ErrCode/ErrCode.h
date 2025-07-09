/*******************************************************************************
* ErrCode.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  zhuyuchun <zhu_yuchun@dahuatech.com>
* Version:  V1.0.0  2022-07-18 Create
* Description: error coder Interface
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

#ifndef __IMOUOS_ERRCODE_H__
#define __IMOUOS_ERRCODE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "OSA/IntTypes.h"
#include "OSA/Error.h"

/********************************************************************
 *
 *                            宏定义区
 *
 *******************************************************************/

typedef enum
{
   E_IMOUOS_ERRMODULE_SYSTEMSERVER = 1,
   E_IMOUOS_ERRMODULE_SERVICEMANAGER,
   E_IMOUOS_ERRMODULE_BROADCAST,
   E_IMOUOS_ERRMODULE_CLOUDACCESS,
   E_IMOUOS_ERRMODULE_DATABASE,
   E_IMOUOS_ERRMODULE_DUMPSYS,
   E_IMOUOS_ERRMODULE_MEDIACODEC,
   E_IMOUOS_ERRMODULE_NETMANAGER,
   E_IMOUOS_ERRMODULE_SWPROBOT,
   E_IMOUOS_ERRMODULE_WIFIMANAGER,
   E_IMOUOS_ERRMODULE_NR,
   
}IMOUOS_ErrModule_e;

#if defined(SYSTEMSERVER)
#define ERRCODE_MODULE E_IMOUOS_ERRMODULE_SYSTEMSERVER
#elif defined(SERVICEMANAGER) 
#define ERRCODE_MODULE E_IMOUOS_ERRMODULE_SERVICEMANAGER
#elif defined(BROADCAST) 
#define ERRCODE_MODULE E_IMOUOS_ERRMODULE_BROADCAST
#elif defined(CLOUDACCESS) 
#define ERRCODE_MODULE E_IMOUOS_ERRMODULE_CLOUDACCESS
#elif defined(DATABASE) 
#define ERRCODE_MODULE E_IMOUOS_ERRMODULE_DATABASE
#elif defined(DUMPSYS) 
#define ERRCODE_MODULE E_IMOUOS_ERRMODULE_DUMPSYS
#elif defined(MEDIACODEC) 
#define ERRCODE_MODULE E_IMOUOS_ERRMODULE_MEDIACODEC
#elif defined(NETMANAGER) 
#define ERRCODE_MODULE E_IMOUOS_ERRMODULE_NETMANAGER
#elif defined(SWPROBOT) 
#define ERRCODE_MODULE E_IMOUOS_ERRMODULE_SWPROBOT
#elif defined(WIFIMANAGER) 
#define ERRCODE_MODULE E_IMOUOS_ERRMODULE_WIFIMANAGER
#else
#define ERRCODE_MODULE E_IMOUOS_ERRMODULE_NR
#endif

#define IMOUOS_ERRCODE_RECORD(errCode) \
imouos_errCode_record(errCode,ERRCODE_MODULE,__FILE__,__LINE__)

/********************************************************************
 *
 *                            类型定义区
 *
 *******************************************************************/


/********************************************************************
 *
 *                            函数声明区
 *
 *******************************************************************/

/// 记录错误码信息接口函数
/// \param [in]    errCode    错误码
/// \param [in]    pFile      文件名
/// \param [in]    line       行号
/// \return  设置错误码信息返回状态IMOUOS_ErrCode_e
int32_t imouos_errCode_record(IMOUOS_ErrCode_e errCode, IMOUOS_ErrModule_e eModule, char *pFile, int32_t line);


#ifdef __cplusplus
} // extern "C"
#endif

#endif //__IMOUOS_ERRCODE_H__