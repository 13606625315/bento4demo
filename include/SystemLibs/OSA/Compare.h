#ifndef __IMOUOS_OSAPI_COMPARE_H__
#define __IMOUOS_OSAPI_COMPARE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"

/// 有符号整数比较返回key1 \key2比较结果
/// \param [in]  key1        第一个比较参数
/// \param [in]  key2        第二个比较参数
/// \return  -1: key1 > key2 1:key1 < key2 0:key1 == key2
IMOUOS_API Int32 imouos_compareI32(const long key1, const long key2);

/// 无符号整数比较返回key1 \key2比较结果
/// \param [in]  key1        第一个比较参数
/// \param [in]  key2        第二个比较参数
/// \return  -1: key1 > key2 1:key1 < key2 0:key1 == key2
IMOUOS_API Int32 imouos_compareU32(const unsigned long key1, const unsigned long key2);

#ifdef __cplusplus
}
#endif
#endif /* __IMOUOS_OSAPI_COMPARE_H__ */
