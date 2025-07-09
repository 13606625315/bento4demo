//
//  Copyright (c)1992-2016, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//  Description:
//  Revisions:      Year-Month-Day  SVN-Author  Modification
//

#ifndef __IMOUOS_OSAPI_MEMORY_H__
#define __IMOUOS_OSAPI_MEMORY_H__

#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"

/********************************************************************
 *
 *                            宏定义区
 *
 *******************************************************************/

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

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
/// 代理内存申请函数
/// \param [in]    module    模块名
/// \param [in]    size      需要申请的内存大小
/// \return                  指向内存区域的指针
void* IMOUOS_API imouos_malloc(Sizet size, const char* module);

/// 代理内存申请函数
/// \param [in]    module    模块名
/// \param [in]    n         需要申请的对象的数目
/// \param [i]    elem_size  需要申请的对象的大小
/// \return  Returns a pointer to n_elements * element_size bytes, with all locations set to zero.
void* IMOUOS_API imouos_calloc(Sizet n, Sizet size, const char* module);

/// 代理内存申请函数
/// \param [in]    module    模块名
/// \param [in]    ptr       旧地址
/// \param [in]    size      需要申请的内存大小
/// \return                  指向内存区域的指针
void* IMOUOS_API imouos_realloc(void* ptr, Sizet size, const char* module);

/// 代理内存释放函数
/// \param [in]    module    模块名
/// \param [in]    ptr       需要释放的内存区域
/// \return 无
void IMOUOS_API imouos_free(void* ptr, const char* module);

/// 代理内存拷贝函数,优化大数据拷贝
/// \param [in]    dstpp    目标地址
/// \param [in]    srcpp    原地址
/// \param [in]    len    拷贝长度
/// \return 无
void* IMOUOS_API imouos_memcpy(void* dstpp, void* srcpp, Sizet len);

/// 内存清0
/// \param [in]    p    地址
/// \param [in]    len  清除长度
/// \return 无
void* IMOUOS_API imouos_memset(void *p, Int32 v, Sizet len);

/// 锁物理内存，不被交换
/// \param [in]    name    进程名字
/// \return
int IMOUOS_API imouos_mlock(char *name);

/// 代理内存打印内存信息
/// \return 无
void IMOUOS_API imouos_mallocinfo(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __IMOUOS_ASYNCNETMUX_MALLOC_H__
