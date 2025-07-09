/*******************************************************************************
* config.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  panjianguang <pan_jianguang@dahuatech.com>
* Version:  V1.0.0  2022-04-07 Create
* Description: Database interface
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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "OSA/ImousTypes.h"

/****   cfgName 命名规范,单个配置项目前设置最大为1024字节，总共配置区为128K
*****   配置一旦设定，其大小只能变小，不能变大
*****   persist.模块名.配置，表示会断电保存配置
*****   rw.模块名.配置，表示保存在内存中，断电不保存。
*****   ro.模块名.配置，表示为只读配置，第一次可写入，其后只读不可写。
*****   xx.模块名.配置，其他开头字段目前暂不支持，无法实现配置更新。
*/

typedef int (*cfgCallBack_t)(char*config, uint32_t size);

/// 获取某名称的配置数据，采用共享内存方式
/// \param [in] cfgName   配置名
/// \param [in] config  拿到的配置数据地址
/// \param [in] size    拿到的配置数据大小
/// \return          获取配置数据的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_getConfig(char* cfgName, uint8_t *config, uint32_t size);

/// 设置某名称的配置数据，采用进程间binder通信方式
/// \param [in] cfgName   配置名
/// \param [in] config  设置的配置数据地址
/// \param [in] size    设置的配置数据大小
/// \return          设置配置数据的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_setConfig(char* cfgName, uint8_t *config, uint32_t size);

/// 删除某名称的配置数据，采用进程间binder通信方式
/// \param [in] cfgName   配置名
/// \return          删除配置数据的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_delConfig(char* cfgName);

/// 设置某名称的配置数组数据，采用进程间binder通信方式
/// \param [in] cfgName   配置名
/// \param [in] config  设置的配置数据地址
/// \param [in] itemSize    设置的数组配置数据一项大小
/// \param [in] itemNum     设置的数组配置数据数据
/// \return          设置配置数组数据的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_setConfigArray(char* cfgName, uint8_t *config, uint32_t itemSize,uint32_t itemNum);

/// 获取某名称的配置数组数据，采用共享内存通信方式
/// \param [in] cfgName   配置名
/// \param [in] config  获取的配置数据地址
/// \param [in] itemSize    获取的数组配置数据一项大小
/// \param [in] itemNum     获取的数组配置数据数据
/// \param [in] offset     获取的数组配置数据启始项
/// \return          获取配置数组数据的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_getConfigArrayMembers(char* cfgName, uint8_t *config, uint32_t itemSize,uint32_t itemNum,uint32_t offset);

/// 设置某名称的配置数组数据，采用共享内存通信方式
/// \param [in] cfgName   配置名
/// \param [in] config  设置的配置数据地址
/// \param [in] itemSize    设置的数组配置数据一项大小
/// \param [in] itemNum     设置的数组配置数据数据
/// \param [in] offset     设置的数组配置数据启始项
/// \return          设置配置数组数据的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_setConfigArrayMembers(char* cfgName, uint8_t *config, uint32_t itemSize,uint32_t itemNum,uint32_t offset);

/// 订阅某名称的配置，采用broadcast进程回调实现，依赖broadcast进程
/// \param [in] cfgName   配置名
/// \param [in] method  订阅配置的回调函数
/// \return          订阅某名称的配置的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_cfgAttach(char* cfgName, cfgCallBack_t method);

/// 取消订阅某名称的配置，采用broadcast进程回调实现，依赖broadcast进程
/// \param [in] cfgName   配置名
/// \param [in] method  订阅配置的回调函数
/// \return          取消订阅某名称的配置的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_cfgDetach(char* cfgName, cfgCallBack_t method);

#endif

