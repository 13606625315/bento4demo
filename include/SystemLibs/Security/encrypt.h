/*******************************************************************************
* encrypt.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  chenliheng <chen_liheng1@dahuatech.com>
* Version:  V1.0.0  2023-06-14 Create
* Description: aes encrypt Interface
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
#ifndef _ENCRYPT_H_
#define _ENCRYPT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "mbedtls/aes.h"
#include "mbedtls/cipher.h"
#include "mbedtls/platform.h"
#include "mbedtls/md5.h"

/// 对flash数据读写进行aes128加密
///
/// \param [in]  data    :待加密数据
/// \param [in]  dataLen :待加密数据长度(需保证尾部预留16字节整数倍的内存空间大小，否则有死机风险)
/// \param [out] encData :加密后的数据
/// \param [in]  bufferLen :加密后数据buffer长度(需保证尾部预留16字节整数倍的内存空间大小，否则有死机风险)
/// \return 成功:返回加密后的长度(16字节填充)  失败:返回 < 0的值
int32_t imouos_flashDataEncode(char *data,int32_t dataLen,char *encData,int32_t bufferLen);

/// 对flash数据读写进行aes128解密
///
/// \param [in]  data     待解密数据
/// \param [in]  dataLen  带解密数据长度(必须16字节整数倍)
/// \param [out] decData  解密后的数据
/// \param [in]  bufferLen :解密后数据buffer长度(需保证尾部预留16字节整数倍的内存空间大小，否则有死机风险)
/// \return 其他 err      0 success
int32_t imouos_flashDataDecode(char *data,int32_t dataLen,char *decData,int32_t bufferLen);

#ifdef __cplusplus
}
#endif
#endif