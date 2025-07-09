//
//  Copyright (c)1992-2016, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//  Description:
//  Revisions:      Year-Month-Day  SVN-Author  Modification
//

#ifndef __IMOUOS_OSAPI_ATOMIC_H__
#define __IMOUOS_OSAPI_ATOMIC_H__

#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************
 *
 *                            宏定义区
 *
 *******************************************************************/

#define INVALID_ATOMIC_HANDLE 0  /// 无效句柄

/********************************************************************
 *
 *                            类型定义区
 *
 *******************************************************************/

/// 原子计数器句柄
typedef Int32 *imouos_atomic;

typedef Int32 imouos_atomic_t;

/********************************************************************
 *
 *                            函数声明区
 *
 *******************************************************************/
/// 设置某一newval值
/// \param [in]  atomic   原子计数器句柄
/// \return     null
IMOUOS_API void imouos_atomicSet(imouos_atomic atomic, imouos_atomic_t newval);

/// 计数减一
/// \param [in]  atomic   原子计数器句柄
/// \return     操作后计数值
IMOUOS_API imouos_atomic_t imouos_atomicDec(imouos_atomic atomic);

/// 计数加一
/// \param [in]  atomic   原子计数器句柄
/// \return     操作后计数值
IMOUOS_API imouos_atomic_t imouos_atomicInc(imouos_atomic atomic);

/// 获取计数
/// \param [in]  atomic   原子计数器句柄
/// \return     计数值
IMOUOS_API imouos_atomic_t imouos_atomicRead(imouos_atomic atomic);

/// 计数加v
/// \param [in]  atomic   原子计数器句柄
/// \return     操作后计数值
IMOUOS_API imouos_atomic_t imouos_atomicAddV(imouos_atomic atomic, imouos_atomic_t v);

/// 计数减v
/// \param [in]  atomic   原子计数器句柄
/// \return     操作后计数值
IMOUOS_API imouos_atomic_t imouos_atomicDecV(imouos_atomic atomic, imouos_atomic_t v);

/// *atomic==oldval则，*atomic=newval, return 1; else return 0;
/// \param [in]  atomic   原子计数器句柄
/// \return     比较*ptr与oldval的值，如果两者相等，则将newval更新到*ptr并返回true
IMOUOS_API Int32 imouos_atomic_bool_compare_exchage(imouos_atomic atomic, imouos_atomic_t oldval, imouos_atomic_t newval);

/// *atomic==oldval则，*atomic=newval else nothing
/// \param [in]  atomic   原子计数器句柄
/// \return     比较*ptr与oldval的值，如果两者相等，则将newval更新到*ptr并返回操作之前*ptr的值
IMOUOS_API imouos_atomic_t imouos_atomic_val_compare_exchage(imouos_atomic atomic, imouos_atomic_t oldval, imouos_atomic_t newval);


/// return *atomic=*atomic&val
/// \param [in]  atomic   原子计数器句柄
/// \return     将*ptr与value相与，结果更新到*ptr，并返回操作之后新*ptr的值
IMOUOS_API Int32 imouos_atomic_and(imouos_atomic atomic, imouos_atomic_t val);

/// return *atomic = *atomic|val
/// \param [in]  atomic   原子计数器句柄
/// \return     将*ptr与value相或， 结果更新到*ptr，并返回操作之后新*ptr的值
IMOUOS_API Int32 imouos_atomic_or(imouos_atomic atomic, imouos_atomic_t val);

/// return *atomic= *atomic^val
/// \param [in]  atomic   原子计数器句柄
/// \return     将*ptr与value异或，结果更新到*ptr，并返回操作之后新*ptr的值
IMOUOS_API Int32 imouos_atomic_xor(imouos_atomic atomic, imouos_atomic_t val);

#ifdef __cplusplus
}
#endif

#endif //  __IMOUOS_INFRA_ATOMIC_H__

