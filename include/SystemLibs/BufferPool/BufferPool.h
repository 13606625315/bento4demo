/*******************************************************************************
* BufferPool.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  longjie <long_jie@dahuatech.com>
* Version:  V1.0.0  2023-04-01 Create
* Description: map Interface
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
*    Contents:  内存池管理接口，管理内存申请和释放，实现内存分时复用
*               建议频繁申请释放的同size类型内存用该库，其它可用c库malloc
*******************************************************************************/

#ifndef __IMOU_BUFFRPOOL_H__
#define __IMOU_BUFFRPOOL_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/// 申请缓存单元
///
/// \param [in]  size:  缓存单元大小
/// \param [in]  module 模块名
/// \param [out] 无
/// \return ~NULL：缓存单元
/// \        NULL：申请失败
void* bufferPool_alloc(size_t size, const char* module);

/// 释放缓存单元
///
/// \param [in]  ptr:要释放的缓存单元
/// \param [in]  module 模块名
/// \param [out] 无
/// \return 无
void bufferPool_free(void *ptr, const char* module);

#ifdef __cplusplus
}
#endif //__cplusplus
#endif //__IMOU_BUFFRPOOL_H__

