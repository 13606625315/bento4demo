/*******************************************************************************
* tinyDebugger.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  longjie <long_jie@dahuatech.com>
* Version:  V1.0.0  2022-08-19 Create
* Description: tinyDebugger headfile
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
*    Date    :  2022-08-19
*    Revision:
*    Author  :  
*    Contents:  轻量debugger工具，记录进程现场信息（如崩溃时信息等）
*******************************************************************************/

#ifndef _IMOUOS_TINYDEBUGGER_H_
#define _IMOUOS_TINYDEBUGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "OSA/IntTypes.h"
#include "OSA/Error.h"

/// 注册debugger调试器，进程出问题时回溯栈
/// \param  [in]  无
/// \param  [out] 无
/// \return 见IMOUOS_ErrCode_e
int32_t imouos_tinyDebuggerRegister();

#ifdef __cplusplus
} // extern "C"
#endif

#endif //_IMOUOS_TINYDEBUGGER_H_

