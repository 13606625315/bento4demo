/*******************************************************************************
 * log_transport.h
 *
 * Copyright (C) 2022-2024 imou CO.,LTD.
 *
 * Author : long_jie <long_jie@dahuatech.com>
 * Version: V1.0.0  2022年9月19日 Create
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
 * Modification: 
 *    Date     : 2022年9月20日
 *    Revision : 
 *    Author   :
 *    Contents : 日志传输模式控制接口
 *******************************************************************************/

#ifndef _LIBS_LOG_TRANSPORT_H
#define _LIBS_LOG_TRANSPORT_H

#ifdef __cplusplus
extern "C" {
#endif

/// Logging transports, bit mask to select features. Function returns selection.
/// 日志传输的位掩码可选择值，设置和获取函数返回选择的值
#define LOGGER_DEFAULT 0x00
#define LOGGER_LOGD    0x01
#define LOGGER_KERNEL  0x02 /* Reserved/Deprecated */
#define LOGGER_NULL    0x04 /* Does not release resources of other selections */
#define LOGGER_LOCAL   0x08 /* logs sent to local memory */
#define LOGGER_STDERR  0x10 /* logs sent to stderr */


/// 设置日志传输方式：当前仅支持LOGGER_LOGD、LOGGER_DEFAULT
///
/// \param  [in]  transport_flag:日志传输标志
/// \param  [out] 无
/// \retval return the selected transport flag mask, or negative errno
int imouos_set_log_transport(int transport_flag);

/// 获取日志传输方式
///
/// \param  [in]  无
/// \param  [out] 无
/// \retval return the selected transport flag mask, or negative errno
int imouos_get_log_transport();

#ifdef __cplusplus
}
#endif

#endif /* _LIBS_LOG_TRANSPORT_H */


