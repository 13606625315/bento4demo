/*******************************************************************************
* mapSave.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  mapSave <chen_liheng1@dahuatech.com>
* Version:  V1.0.0  2023-06-14 Create
* Description: mapSave Interface
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
#ifndef _MAPSAVE_H_
#define _MAPSAVE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "Common/imou_alg_common_define.h"

/// 读取地图文件
///
/// \param [in]  data         :读取地图文件buffer
/// \param [in]  maxBufferLen :读取地图的buffer长度
/// \param [in]  path         :读取路径
/// \return 成功:E_IMOUOS_SUCCESS  失败:ERR
int32_t imouos_readMapData(imou_alg_OccupancyGridInfo_t* data, int32_t maxBufferLen, const char* path);

/// 写地图文件
///
/// \param [in]  data       :写地图文件buffer
/// \param [in]  maxMapSize :对应地图允许的最大size
/// \param [in]  path       :写路径
/// \return 成功:E_IMOUOS_SUCCESS  失败:ERR
int32_t imouos_saveMapData(imou_alg_OccupancyGridInfo_t* data, int32_t maxMapSize,const char* path);

#ifdef __cplusplus
}
#endif
#endif