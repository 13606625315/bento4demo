/*******************************************************************************
 * log_track.h
 *
 * Copyright (C) 2021-2023 ZheJiang Dahua Technology CO.,LTD.
 *
 * Author : long_jie <long_jie@dahuatech.com>
 * Version: V1.0.0  2023年7月14日 Create
 *
 * Description:
 *
 *       1. 硬件说明
 *          无。
 *
 *       2. 程序结构说明。
 *          无。
 *
 *       3. 使用说明。
 *          无。
 *
 *       4. 局限性说明。
 *          无。
 *
 *       5. 其他说明。
 *          无。
 *
 * Modification: 埋点模块定义
 *    Date     : 2023年7月25日
 *    Revision : 
 *    Author   :
 *    Contents :
 *******************************************************************************/

#ifndef _LIBLOG_TRACKMODULE_H__
#define _LIBLOG_TRACKMODULE_H__

#ifdef __cplusplus
extern "C" {
#endif

///< 埋点模块：参考公共埋点协议定义
typedef enum
{
    LOG_TRACK_SERVER_CLOUD        = 38,   // ClodAccess
    LOG_TRACK_SERVER_NETMANAGER   = 39,   // NetManager
    LOG_TRACK_SERVER_WIFIMANAGER  = 40,   // WifiManager
    LOG_TRACK_SERVER_SWPROBOT     = 41,   // SwpRobot
    LOG_TRACK_SERVER_BROADCAST    = 42,   // BroadCast
    LOG_TRACK_SERVER_DATABSE      = 43,   // DataBase
    LOG_TRACK_SERVER_SYSTEMSERVER = 44,   // SysTemServer
    LOG_TRACK_SERVER_MEDIACODEC   = 45,   // MediaCodec
    LOG_TRACK_SERVER_TINYDEBUGGER = 46,   // TinyDebugger
    LOG_TRACK_SERVER_SERVERMANAGER= 47,   // ServiceManager

    LOG_TRACK_MAX = 68,
}TRACK_MODULE_T;

#ifdef __cplusplus
}
#endif

#endif // _LIBLOG_TRACK_H__

