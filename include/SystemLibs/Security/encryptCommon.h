/*******************************************************************************
* encryptCommon.h
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
#ifndef _ENCRYPT_COMMON_H_
#define _ENCRYPT_COMMON_H_
#ifdef __cplusplus
extern "C"
{
#endif
/// 获取字节填充至block_len对齐所需要填充的数量
///例如block_padding( dataLen, 16 ); 代表填充至16字节的倍数
/// \param [in] len :填充前的数据长度
/// \param [in] block_len :需填充多少字节对齐
/// \return 返回需要填充的数量
static inline uint32_t imouos_block_padding( uint32_t len, uint32_t block_len )
{
	return ( block_len - len % block_len ) % block_len;
}

/// 根据block_len字节对齐函数
///
/// \param [in] len :填充前的数据长度
/// \param [in] block_len :需填充多少字节对齐
/// \return 返回block_len字节对齐后总字节数
static inline uint32_t imouos_block_algin_len( uint32_t len, uint32_t block_len )
{
	return ( len % block_len == 0 ) ? len : len + imouos_block_padding( len, block_len );
}

#ifdef __cplusplus
}
#endif
#endif