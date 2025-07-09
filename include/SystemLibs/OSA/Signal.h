#ifndef __IMOUOS_OSAPI_SIGNAL_H__
#define __IMOUOS_OSAPI_SIGNAL_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"

/********************************************************************
 *
 *                            宏定义区
 *
 *******************************************************************/


/********************************************************************
 *
 *                            类型定义区
 *
 *******************************************************************/

/// 线程处理函数
typedef void (*imouos_sighandler)(Int32);

/********************************************************************
 *
 *                            函数声明区
 *
 *******************************************************************/

/// 注册信号
/// \param [in] sigId 信号1-64
/// \param [in] handler 信号处理函数
/// \return           IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_signal(Int32 sigId, imouos_sighandler handler);

/// 注册信号,只执行一次
/// \param [in] sigId 信号1-64
/// \param [in] handler 信号处理函数
/// \return           IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_signalOnce(Int32 sigId, imouos_sighandler handler);

#ifdef __cplusplus
}
#endif
#endif //  __IMOUOS_INFRA_SIGNAL_H__
