#ifndef __IMOUOS_OSAPI_HASH_H__
#define __IMOUOS_OSAPI_HASH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"

/// 计算32位哈希值
/// \param [in]  data	数据
/// \param [in]  len	长度
/// \param [out] hval	结果
/// \return				0：计算成功；<0：计算失败
IMOUOS_API Int32 imouos_hash32(const void* data, Sizet len, Uint32* hval);

/// 计算64位哈希值
/// \param [in]  data	数据
/// \param [in]  len	长度
/// \param [out] hval	结果
/// \return				0：计算成功；<0：计算失败
IMOUOS_API Int32 imouos_hash64(const void* data, Sizet len, Uint64* hval);

/// 计算128位哈希值
/// \param [in]  data	数据
/// \param [in]  len	长度
/// \param [out] hval	高64位结果
/// \param [out] lval	低64位结果
/// \return				0：计算成功；<0：计算失败
IMOUOS_API Int32 imouos_hash128(const void* data, Sizet len, Uint64* hval, Uint64* lval);

#ifdef __cplusplus
}
#endif

#endif
