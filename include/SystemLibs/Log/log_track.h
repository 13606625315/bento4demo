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
 * Modification: 埋点日志模块,埋点描述信息依赖的结构体，必须使用1字节对齐
 *    Date     : 2023年7月14日
 *    Revision : 
 *    Author   :
 *    Contents :
 *******************************************************************************/

#ifndef _LIBLOG_TRACK_H__
#define _LIBLOG_TRACK_H__

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

///< 固定标签
#define IMOULOG_TRACK_TAG       "LOGTRACK"

///< 扩展描述信息最大长度
#define IMOULOG_TRACK_EXMAXLEN    256

///< 头部结构LOG_TrackLocalInfo转字符后最大长度
#define IMOULOG_TRACK_HEADMAXLEN  64 

///< 埋点等级：参考公共埋点协议定义
typedef enum
{
    TRACK_INFO_LV_FATAL,            // 致命级别
    TRACK_INFO_LV_SERIOUS,          // 严重级别
    TRACK_INFO_LV_IMPORTANT,        // 重要信息
    TRACK_INFO_LV_COMM,             // 一般级别
}TRACK_INFO_LV_T;

typedef struct
{
    TRACK_MODULE_T modulenum;       // 模块编号
    TRACK_INFO_LV_T  level;         // 错误等级
    int32_t err;                    // 错误码
    int32_t len;                    // 埋点描述信息长度
    void *data;                     // 埋点描述信息
}LOG_TrackLocalInfo;

/*******************************************************************************
* 函数名  : imou_log_track
* 描  述  : 该函数向应用上报埋点信息，由各个模块调用。
*         len不应该超过IMOULOG_TRACK_EXMAXLEN
* 输  入  : module：模块编号
*         level：错误等级
*         err：错误码
*         pFmt：埋点信息描述
*         len: 埋点描述信息的长度
*
* 输  出  : 无。
* 返回值  :   0: 成功
*         ~0: 失败
*******************************************************************************/
int32_t  imouos_log_track(TRACK_MODULE_T module, TRACK_INFO_LV_T level, int32_t err, void *pFmt, int32_t len);

/*******************************************************************************
* 函数名  : imouos_log_tracksring
* 描  述  : 该函数向应用上报格式化埋点信息，由各个模块调用。
*         使用该函数调用时，协议文档对应字节序0-1填string1, 下一行字节序填0，类型填string
* 输  入  : module：模块编号
*         level： 错误等级
*         err：    错误码
*         fmt：    格式化字符串（最长IMOULOG_TRACK_EXMAXLEN - 2字节）
*
* 输  出  : 无。
* 返回值  :   0: 成功
*         ~0: 失败
*******************************************************************************/
int32_t imouos_log_tracksring(TRACK_MODULE_T module, TRACK_INFO_LV_T level, int32_t err, const char *fmt, ...);

#ifndef TRACK_MODULE
#define TRACK_MODULE  0
#endif

///< 致命等级埋点
#define TRACKF(err)\
    do\
    {\
         imouos_log_track(TRACK_MODULE, TRACK_INFO_LV_FATAL, err, NULL, 0);\
    }\
    while(0);

///< 严重等级埋点
#define TRACKE(err)\
    do\
    {\
         imouos_log_track(TRACK_MODULE, TRACK_INFO_LV_SERIOUS, err, NULL, 0);\
    }\
    while(0);    

///< 重要等级埋点
#define TRACKW(err)\
    do\
    {\
         imouos_log_track(TRACK_MODULE, TRACK_INFO_LV_IMPORTANT, err, NULL, 0);\
    }\
    while(0);

///< 一般等级埋点
#define TRACKI(err)\
    do\
    {\
         imouos_log_track(TRACK_MODULE, TRACK_INFO_LV_COMM, err, NULL, 0);\
    }\
    while(0);

        
///< 扩展致命等级埋点
#define TRACKF_EX(err, pFmt, len)\
    do\
    {\
         imouos_log_track(TRACK_MODULE, TRACK_INFO_LV_FATAL, err, pFmt, len);\
    }\
    while(0);

///< 扩展严重等级埋点
#define TRACKE_EX(err, pFmt, len)\
    do\
    {\
         imouos_log_track(TRACK_MODULE, TRACK_INFO_LV_SERIOUS, err, pFmt, len);\
    }\
    while(0);

///< 扩展重要等级埋点  
#define TRACKW_EX(err, pFmt, len)\
    do\
    {\
         imouos_log_track(TRACK_MODULE, TRACK_INFO_LV_IMPORTANT, err, pFmt, len);\
    }\
    while(0);
    
///< 扩展一般等级埋点      
#define TRACKI_EX(err, pFmt, len)\
    do\
    {\
         imouos_log_track(TRACK_MODULE, TRACK_INFO_LV_COMM, err, pFmt, len);\
    }\
    while(0);

///< 格式化字符串埋点
#define imouos_track_printLog(module, level, error, fmt...) imouos_log_tracksring(module, level, error, fmt)
#define TRACKLOGI(error, ...) (imouos_track_printLog(TRACK_MODULE, TRACK_INFO_LV_COMM, error, __VA_ARGS__))
#define TRACKLOGW(error, ...) (imouos_track_printLog(TRACK_MODULE, TRACK_INFO_LV_IMPORTANT, error, __VA_ARGS__))
#define TRACKLOGE(error, ...) (imouos_track_printLog(TRACK_MODULE, TRACK_INFO_LV_SERIOUS, error, __VA_ARGS__))
#define TRACKLOGF(error, ...) (imouos_track_printLog(TRACK_MODULE, TRACK_INFO_LV_FATAL, error, __VA_ARGS__))

#ifdef __cplusplus
}
#endif

#endif // _LIBLOG_TRACK_H__

