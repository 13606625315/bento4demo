/*******************************************************************************
 * log_id.h
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
 *    Contents : log模块id转换接口
 *******************************************************************************/

#ifndef _LIBS_LOG_LOG_ID_H
#define _LIBS_LOG_LOG_ID_H

#ifdef __cplusplus
extern "C" {
#endif

/// 日志ID
#ifndef log_id_t_defined
#define log_id_t_defined
typedef enum imouos_logId
{
    E_LOG_ID_MIN    = 0,
    E_LOG_ID_MAIN   = 0,
    E_LOG_ID_SYSTEM = 1,
    E_LOG_ID_CRASH  = 2,
    E_LOG_ID_KERNEL = 3, 
    E_LOG_ID_MAX
} imouos_logId;
#endif
#define sizeof_log_id_t sizeof(typeof_log_id_t)
#define typeof_log_id_t unsigned char

/// Send a simple string to the log.
/// 打印一条字符串到log模块
///
/// \param  [in]  bufID:日志ID
/// \param  [in]  prio：优先级
/// \param  [in]  tag： 标签
/// \param  [in]  text：内容
/// \param  [out] 无
/// \retval >=0 成功：写入字节数
/// \retval <0  失败 
int __imouos_log_buf_write(int bufID, int prio, const char* tag,
                                    const char* text);

/// Send a simple string to the log.
/// 打印一条日志到log模块
///
/// \param  [in]  bufID:日志ID
/// \param  [in]  prio：优先级
/// \param  [in]  tag： 标签
/// \param  [in]  fmt： 内容
/// \param  [out] 无
/// \retval >=0 成功：写入字节数
/// \retval <0  失败 
int __imouos_log_buf_print(int bufID, int prio, const char* tag,
                            const char* fmt, ...)
#if defined(__GNUC__)
    __attribute__((__format__(printf, 4, 5)))
#endif
    ;

/// 转换日志名为ID号
///
/// \param  [in]  logName:日志名字，每个ID对应一个名字
/// \param  [out] 无
/// \retval ID：日志ID
/// \retval <0  失败 
imouos_logId imouos_name_to_log_id(const char* logName);

/// 转换ID号为日志名
///
/// \param  [in]  log_id:日志ID，每个ID对应一个名字
/// \param  [out] 无
/// \retval 日志名字
/// \retval NULL  失败 
const char*  imouos_log_id_to_name(imouos_logId log_id);

#ifdef __cplusplus
}
#endif

#endif /* _LIBS_LOG_LOG_ID_H */

