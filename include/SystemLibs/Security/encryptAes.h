/*******************************************************************************
* encryptAes.h
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
//支持的aes加密方式
//支持aes-ecb aes-cbc两种加密模式
//支持pkcs5 pkcs7 pkcsuser三种填充方式
//支持128 192 256三种密钥长度

//aes编码注意
//加解密的数据必须16字节对齐，因为aes加密是按照16字节分组加密的
//加/解密后的数据长度和原始数据保持一致，是不变的

//tip:如何选择aes模式?
//加密速度 1 aes-ecb > aes-cbc 2 key-128 > key-192 > key-256 3 padding无影响
//加密强度 1 aes-cbc > aes-ecb 2 key-256 > key-192 > key-128 3 pkcsuser > pkcs5 > pkcs7

#ifndef _ENCRYPT_AES_H_
#define _ENCRYPT_AES_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "mbedtls/aes.h"
#include "mbedtls/cipher.h"
#include "mbedtls/platform.h"
#include "mbedtls/md5.h"

#define AES_BLOCK_LEN       16    //aes分组大小:固定16字节(128bit)
#define AES_IV_LEN          16    //aes iv向量大小:固定16字节(128bit)

#define AES128_KEY_LEN      16    //aes 128秘钥大小:16字节(128bit)
#define AES192_KEY_LEN      24    //aes 192秘钥大小:24字节(192bit)
#define AES256_KEY_LEN      32    //aes 256秘钥大小:16字节(256bit)

//aes key type
typedef enum
{
    AES128 = 0,
    AES192,
    AES256,
} AesKeySize;

//aes padding type
typedef enum
{
    PKCS7 = 0,  //zero padding
    PKCS5,      //number padding
    PKCS_USER,  //user define padding
} AesPaddingMode;

/// aes待加密数据padding
///
/// \param [in]     mod          :padding模式
/// \param [in out] data         :待padding数据
/// \param [in]     dataLen      :待padding数据长度
/// \param [in]     maxBufferLen :最大buffer长度
/// \param [out]    bufElem      :缓存元素结构
/// \return IMOUOS_ErrCode_e
int32_t imouos_aesPaddingMod(AesPaddingMode mod, char *data, int32_t dataLen, int32_t maxBufferLen);

/// aes-ecb128模式加密
///
/// \param [in]     aesKey  :aes加密密钥
/// \param [in]     data    :待加密数据(必须16字节对齐)
/// \param [in]     dataLen :待加密数据长度
/// \param [out]    encData :加密后的数据(长度不变)
/// \return IMOUOS_ErrCode_e
int32_t imouos_aes128EcbEncrypt(const unsigned char *aesKey, const unsigned char *data, int dataLen, unsigned char *encData);

/// aes-ecb128模式解密
///
/// \param [in]     aesKey  :aes加密密钥
/// \param [in]     data    :待解密数据(必须16字节对齐)
/// \param [in]     dataLen :待解密数据长度
/// \param [out]    encData :解密后的数据(长度不变)
/// \return IMOUOS_ErrCode_e
int32_t imouos_aes128EcbDecrypt(const unsigned char *aesKey, const unsigned char *data, int dataLen, unsigned char *decData);

/// aes-ecb192模式加密
///
/// \param [in]     aesKey  :aes加密密钥
/// \param [in]     data    :待加密数据(必须16字节对齐)
/// \param [in]     dataLen :待加密数据长度
/// \param [out]    encData :加密后的数据(长度不变)
/// \return IMOUOS_ErrCode_e
int32_t imouos_aes192EcbEncrypt(const unsigned char *aesKey, const unsigned char *data, int dataLen, unsigned char *encData);

/// aes-ecb192模式解密
///
/// \param [in]     aesKey  :aes加密密钥
/// \param [in]     data    :待解密数据(必须16字节对齐)
/// \param [in]     dataLen :待解密数据长度
/// \param [out]    encData :解密后的数据(长度不变)
/// \return IMOUOS_ErrCode_e
int32_t imouos_aes192EcbDecrypt(const unsigned char *aesKey, const unsigned char *data, int dataLen, unsigned char *decData);

/// aes-ecb256模式加密
///
/// \param [in]     aesKey  :aes加密密钥
/// \param [in]     data    :待加密数据(必须16字节对齐)
/// \param [in]     dataLen :待加密数据长度
/// \param [out]    encData :加密后的数据(长度不变)
/// \return IMOUOS_ErrCode_e
int32_t imouos_aes256EcbEncrypt(const unsigned char *aesKey, const unsigned char *data, int dataLen, unsigned char *encData);

/// aes-ecb256模式解密
///
/// \param [in]     aesKey  :aes加密密钥
/// \param [in]     data    :待解密数据(必须16字节对齐)
/// \param [in]     dataLen :待解密数据长度
/// \param [out]    encData :解密后的数据(长度不变)
/// \return IMOUOS_ErrCode_e
int32_t imouos_aes256EcbDecrypt(const unsigned char *aesKey, const unsigned char *data, int dataLen, unsigned char *decData);

/// aes-cbc128模式加密
///
/// \param [in]     aesKey  :aes加密密钥
/// \param [in]     iv      :aes cbc模式的初始向量(16字节)
/// \param [in]     data    :待加密数据(必须16字节对齐)
/// \param [in]     dataLen :待加密数据长度
/// \param [out]    encData :加密后的数据(长度不变)
/// \return IMOUOS_ErrCode_e
int32_t imouos_aes128CbcEncrypt(const unsigned char *aesKey, unsigned char *iv, const unsigned char *data, int dataLen, unsigned char *encData);

/// aes-cbc128模式解密
///
/// \param [in]     aesKey  :aes解密密钥
/// \param [in]     iv      :aes cbc模式的初始向量(16字节)
/// \param [in]     data    :待解密数据(必须16字节对齐)
/// \param [in]     dataLen :待解密数据长度
/// \param [out]    encData :解密后的数据(长度不变)
/// \return IMOUOS_ErrCode_e
int32_t imouos_aes128CbcDecrypt(const unsigned char *aesKey, const unsigned char *iv, const unsigned char *data, int dataLen, unsigned char *decData);

/// aes-cbc192模式加密
///
/// \param [in]     aesKey  :aes加密密钥
/// \param [in]     iv      :aes cbc模式的初始向量(16字节)
/// \param [in]     data    :待加密数据(必须16字节对齐)
/// \param [in]     dataLen :待加密数据长度
/// \param [out]    encData :加密后的数据(长度不变)
/// \return IMOUOS_ErrCode_e
int32_t imouos_aes192CbcEncrypt(const unsigned char *aesKey, unsigned char *iv, const unsigned char *data, int dataLen, unsigned char *encData);

/// aes-cbc192模式解密
///
/// \param [in]     aesKey  :aes解密密钥
/// \param [in]     iv      :aes cbc模式的初始向量(16字节)
/// \param [in]     data    :待解密数据(必须16字节对齐)
/// \param [in]     dataLen :待解密数据长度
/// \param [out]    encData :解密后的数据(长度不变)
/// \return IMOUOS_ErrCode_e
int32_t imouos_aes192CbcDecrypt(const unsigned char *aesKey, const unsigned char *iv, const unsigned char *data, int dataLen, unsigned char *decData);

/// aes-cbc256模式加密
///
/// \param [in]     aesKey  :aes加密密钥
/// \param [in]     iv      :aes cbc模式的初始向量(16字节)
/// \param [in]     data    :待加密数据(必须16字节对齐)
/// \param [in]     dataLen :待加密数据长度
/// \param [out]    encData :加密后的数据(长度不变)
/// \return IMOUOS_ErrCode_e
int32_t imouos_aes256CbcEncrypt(const unsigned char *aesKey, unsigned char *iv, const unsigned char *data, int dataLen, unsigned char *encData);

/// aes-cbc256模式解密
///
/// \param [in]     aesKey  :aes解密密钥
/// \param [in]     iv      :aes cbc模式的初始向量(16字节)
/// \param [in]     data    :待解密数据(必须16字节对齐)
/// \param [in]     dataLen :待解密数据长度
/// \param [out]    encData :解密后的数据(长度不变)
/// \return IMOUOS_ErrCode_e
int32_t imouos_aes256CbcDecrypt(const unsigned char *aesKey, const unsigned char *iv, const unsigned char *data, int dataLen, unsigned char *decData);
#ifdef __cplusplus
}
#endif
#endif