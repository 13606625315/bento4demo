/*******************************************************************************
 * logprint.h
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
 *    Contents : 格式化打印模块
 *******************************************************************************/

#ifndef _LOGPRINT_H
#define _LOGPRINT_H

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/// IMOUOS log priority values, in ascending priority order.
/// IMOUOS日志优先级变量，按照降序排列
typedef enum imouos_LogPriority
{
    IMOUOS_LOG_SILENT = 0,
    IMOUOS_LOG_FATAL  = 1,
    IMOUOS_LOG_CRIT   = 2,
    IMOUOS_LOG_ERROR  = 3,
    IMOUOS_LOG_WARN   = 4,
    IMOUOS_LOG_NOTICE = 5,
    IMOUOS_LOG_INFO   = 6,
    IMOUOS_LOG_DEBUG  = 7,
    IMOUOS_LOG_VERBOSE,
    IMOUOS_LOG_DEFAULT,
    IMOUOS_LOG_UNKNOWN,
} imouos_LogPriority;

/// IMOUOS日志打印格式
typedef enum
{
    /* Verbs */
    FORMAT_OFF = 0,
    FORMAT_BRIEF,
    FORMAT_PROCESS,
    FORMAT_TAG,
    FORMAT_THREAD,
    FORMAT_RAW,
    FORMAT_TIME,
    FORMAT_THREADTIME,
    FORMAT_LONG,
    /* Adverbs. The following are modifiers to above format verbs */
    FORMAT_MODIFIER_COLOR,     /* converts priority to color */
    FORMAT_MODIFIER_TIME_USEC, /* switches from msec to usec time precision */
    FORMAT_MODIFIER_PRINTABLE, /* converts non-printable to printable escapes */
    FORMAT_MODIFIER_YEAR,      /* Adds year to date */
    FORMAT_MODIFIER_ZONE,      /* Adds zone to date, + UTC */
    FORMAT_MODIFIER_EPOCH,     /* Print time as seconds since Jan 1 1970 */
    FORMAT_MODIFIER_MONOTONIC, /* Print cpu time as seconds since start */
    FORMAT_MODIFIER_UID,       /* Adds uid */
    FORMAT_MODIFIER_DESCRIPT,  /* Adds descriptive */
    /* private, undocumented */
    FORMAT_MODIFIER_TIME_NSEC, /* switches from msec to nsec time precision */
} ImouOsLogPrintFormat;

typedef struct ImouOsLogFormat_t ImouOsLogFormat;

/// IMOUOS日志结构
typedef struct ImouOsLogEntry
{
    time_t tv_sec;
    long tv_nsec;
    imouos_LogPriority priority;
    int32_t uid;
    int32_t pid;
    int32_t tid;
    const char* tag;
    size_t tagLen;
    size_t messageLen;
    const char* message;
} ImouOsLogEntry;

/// 申请Formate对象
///
/// \param  [in]  无
/// \param  [out] 无
/// \retval ~NULL 成功
/// \retval  NULL 失败
ImouOsLogFormat* imouos_log_format_new();

/// 释放Formate对象
///
/// \param  [in ] p_format：需要释放的Formate对象
/// \param  [out] 无
/// \retval 无
void imouos_log_format_free(ImouOsLogFormat* p_format);

/// 设置Formate对象对应的打印格式
///
/// \param  [in ] p_format：需设置格式的Formate对象
/// \param  [in ] format：需设置的格式
/// \param  [out] 无
/// \retval currently returns 0 if format is a modifier, 1 if not
int imouos_log_setPrintFormat(ImouOsLogFormat* p_format,
                                         ImouOsLogPrintFormat format);

/// 转换字符串对应的格式
///
/// \param  [in ] s：需转换的字符串，如brief、process...
/// \param  [out] 无
/// \retval Returns FORMAT_OFF on invalid string
ImouOsLogPrintFormat imouos_log_formatFromString(const char* s);

/// 增加过滤规则：Assumes single threaded execution
///
/// \param  [in ] p_format：需设置规则的对象
/// \param  [in ] filterExpression: a single filter expression，eg "AT:d"
/// \param  [out] 无
/// \retval returns 0 on success and -1 on invalid expression
int imouos_log_addFilterRule(ImouOsLogFormat* p_format,
                                        const char* filterExpression);

/// 增加过滤规则：Assumes single threaded execution
///
/// \param  [in ] p_format：需设置规则的对象
/// \param  [in ] filterString: a whitespace-separated set of filter expressions
///                             eg "AT:d *:i"
/// \param  [out] 无
/// \retval returns 0 on success and -1 on invalid expression
int imouos_log_addFilterString(ImouOsLogFormat* p_format,
                                          const char* filterString);

/// 判断日志是否应该根据优先级和标签打印
///
/// \param  [in ] p_format：需判断的对象
/// \param  [in ] tag: a whitespace-separated set of filter expressions
/// \param  [in ] pri: 
/// \param  [out] 无
/// \retval returns 1 if this log line should be printed based on its priority
///         and tag, and 0 if it should not
int imouos_log_shouldPrintLine(ImouOsLogFormat* p_format,
                                          const char* tag,
                                          imouos_LogPriority pri);

/// 处理日志buf：Splits a wire-format buffer into an ImouOsLogEntry 
///              entry allocated by caller. Pointers will point directly into buf
///
/// \param  [in ] buf:   需要转换的日志buf 
/// \param  [out] entry：转换结果
/// \retval Returns 0 on success and -1 on invalid wire format (entry will be
///         in unspecified state)
int imouos_log_processLogBuffer(struct logger_entry* buf,
                                            ImouOsLogEntry* entry);


/// 处理日志buf：Like imouos_log_processLogBuffer, but for binary logs.
/// \param  [in ] buf:  需要转换的日志buf 
/// \param  [in ] map:  If "map" is non-NULL, 
///                     it will be used to convert the log tag number
/// \param  [out] entry：转换结果
/// \retval Returns 0 on success and -1 on invalid wire format (entry will be
///         in unspecified state)
//int imouos_log_processBinaryLogBuffer(struct logger_entry* buf,
//                                      ImouOsLogEntry* entry,
//                                      const EventTagMap* map,
//                                      char* messageBuf,
//                                      int messageBufLen);


/// Formats a log message into a buffer
/// 日志消息格式化到缓冲区
///
/// \param  [in ] p_format：需处理的对象
/// \param  [in ] defaultBuffer: 默认buf，如果不可用，会重新申请
/// \param  [in ] defaultBufferSize：默认buf大小
/// \param  [in ] p_line：日志信息
/// \param  [out] p_outLength：输出缓存长度
/// \retval If return value != defaultBuffer, caller must call free
///         NOTE: must free(), not imouos_free()
///         Returns NULL on malloc error
char* imouos_log_formatLogLine(ImouOsLogFormat* p_format,
                                         char* defaultBuffer,
                                         size_t defaultBufferSize,
                                         const ImouOsLogEntry* p_line,
                                         size_t* p_outLength);

/// Either print or do not print log line, based on filter
/// 根据过滤器判断是否打印或不打印
/// Assumes single threaded execution
///
/// \param  [in ] p_format：需处理的对象 
/// \param  [in ] fd：输出到文本或者终端
/// \param  [in ] entry：日志消息
/// \param  [out] 无
/// \retval Returns count bytes written
int imouos_log_printLogLine(ImouOsLogFormat* p_format,
                                      int fd,
                                      const ImouOsLogEntry* entry);

#ifdef __cplusplus
}
#endif

#endif /*_LOGPRINT_H*/

