
#ifndef __IMOUOS_OSAPI_THRSPECIFIC_H__
#define __IMOUOS_OSAPI_THRSPECIFIC_H__

#include "OSA/IntTypes.h"
#include "OSA/Thread.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************
 *
 *                            类型定义区
 *
 *******************************************************************/
typedef imouos_handle imouos_thrPriVal;


/********************************************************************
 *
 *                            函数声明
 *
 *******************************************************************/

/// 创建线程私有变量(只需要创建一次，所有线程都有此私有数据)
/// \param [in] 析构变量函数指针
/// \param [in] ppriVal 私有变量句柄地址
/// \return    IMOUOS_ErrCode_e码
Int32 imouos_thrCreateKVal(imouos_thrPriVal *ppriVal);

/// 删除线程私有变量
/// \param [in] priVal 私有变量句柄
/// \return    IMOUOS_ErrCode_e码
Int32 imouos_thrDelKVal(imouos_thrPriVal priVal);

/// 设置线程私有变量
/// \param [in] priVal 私有变量句柄
/// \param [in] pval 私有数据
/// \return    IMOUOS_ErrCode_e码
Int32 imouos_thrSetKVal(imouos_thrPriVal priVal, void* pval);

/// 获取线程私有变量
/// \param [in] priVal 私有变量句柄
/// \return IMOUOS_ErrCode_e码
Int32 imouos_thrGetKVal(imouos_thrPriVal priVal, void **ppval);

#ifdef __cplusplus
}
#endif

#endif //  __IMOUOS_INFRA_THRSPECIFIC_INTERNAL_H__

