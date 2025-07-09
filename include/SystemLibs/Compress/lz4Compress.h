/*******************************************************************************
* lz4Compress.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  chenliheng <chen_liheng1@dahuatech.com>
* Version:  V1.0.0  2023-07-18 Create
* Description: lz4 compress Interface
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
#ifndef _LZ4_COMPRESS_H_
#define _LZ4_COMPRESS_H_

#ifdef __cplusplus
extern "C"
{
#endif

/**
  * @brief     lz4获取压缩后的最大可能大小
  * @param[IN] srcLen  待压缩数据长度
  * @return 最大压缩可能长度:获取成功  小于0:获取失败
  */
int32_t imouos_getLz4CompressMaxSize(int32_t srcLen);

/**
  * @brief     lz4压缩数据接口(NOTICE:srcData和dstData不允许是同一块buffer)
  * @param[IN] srcData  待压缩数据
  * @param[IN] srcLen  待压缩数据长度
  * @param[IN/OUT] dstData  压缩后数据
  * @param[IN] dstBufferLen dstData buffer的长度
  * @return E_IMOUOS_SUCCESS:压缩成功  其他:失败
  */
int32_t imouos_lz4Compress(void *srcData, int32_t srcLen, void *dstData, int32_t dstBufferLen);

/**
  * @brief     lz4解压数据接口(NOTICE:srcData和dstData不允许是同一块buffer)
  * @param[IN] srcData  待解压数据
  * @param[IN] srcLen  待解压数据长度
  * @param[IN/OUT] dstData  解压后数据
  * @param[IN] dstBufferLen dstData buffer的长度
  * @return E_IMOUOS_SUCCESS:压缩成功  其他:失败
  */
int32_t imouos_lz4Decompress(const void *srcData, int32_t srcLen, void *dstData, int32_t dstBufferLen);

#ifdef __cplusplus
}
#endif
#endif