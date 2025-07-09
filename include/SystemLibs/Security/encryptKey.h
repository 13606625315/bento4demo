/*******************************************************************************
* encryptKey.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  chenliheng <chen_liheng1@dahuatech.com>
* Version:  V1.0.0  2023-06-14 Create
* Description: encrypt key general Interface
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
//本模块用于获取aes-128/192/256的密钥和cbc模式的iv向量值

#ifndef _ENCRYPT_KEY_H_
#define _ENCRYPT_KEY_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "mbedtls/cipher.h"
#include "mbedtls/platform.h"
#include "mbedtls/md5.h"

/// 获取aes128的密钥key1(NOTICI:密钥系统初始化后才能获取成功)
///
/// \param [in out] aesKey :aes128的密钥key1
/// \return IMOUOS_ErrCode_e
int32_t imouos_aes128Key1Get(unsigned char *aesKey);

/// 获取aes192的密钥key1(NOTICI:密钥系统初始化后才能获取成功)
///
/// \param [in out] aesKey :aes192的密钥key1
/// \return IMOUOS_ErrCode_e
int32_t imouos_aes192Key1Get(unsigned char *aesKey);

/// 获取aes256的密钥key1(NOTICI:密钥系统初始化后才能获取成功)
///
/// \param [in out] aesKey :aes256的密钥key1
/// \return IMOUOS_ErrCode_e
int32_t imouos_aes256Key1Get(unsigned char *aesKey);

/// 获取aes cbc的初始向量iv1(NOTICI:密钥系统初始化后才能获取成功)
///
/// \param [in out] aesKey :aes cbc的初始向量iv1
/// \return IMOUOS_ErrCode_e
int32_t imouos_aesIv1Get(unsigned char *aesIv);

/// 初始化密钥系统
///
/// \param [in] void
/// \return IMOUOS_ErrCode_e
int32_t imouos_encryptKeySystemInit(void);

#ifdef __cplusplus
}
#endif
#endif