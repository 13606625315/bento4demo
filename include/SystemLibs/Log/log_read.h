/*******************************************************************************
 * log_read.h
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
 *    Contents : liblog读日志接口
 *******************************************************************************/
#ifndef _LIBS_LOG_LOG_READ_H
#define _LIBS_LOG_LOG_READ_H

#include "OSA/Atomic.h"
#include "Log/log_id.h"

#ifndef __IMOUOS_USE_LIBLOG_READER_INTERFACE
#ifndef __IMOUOS_API__
#define __IMOUOS_USE_LIBLOG_READER_INTERFACE 3
#elif __IMOUOS_API__ > 23 /* > Marshmallow */
#define __IMOUOS_USE_LIBLOG_READER_INTERFACE 3
#elif __IMOUOS_API__ > 22 /* > Lollipop */
#define __IMOUOS_USE_LIBLOG_READER_INTERFACE 2
#elif __IMOUOS_API__ > 19 /* > KitKat */
#define __IMOUOS_USE_LIBLOG_READER_INTERFACE 1
#else
#define __IMOUOS_USE_LIBLOG_READER_INTERFACE 0
#endif
#endif

#define IMOUOS_LOG_RDONLY  O_RDONLY
#define IMOUOS_LOG_WRONLY  O_WRONLY
#define IMOUOS_LOG_RDWR    O_RDWR
#define IMOUOS_LOG_ACCMODE O_ACCMODE
#ifndef O_NONBLOCK
#define IMOUOS_LOG_NONBLOCK 0x00000800
#else
#define IMOUOS_LOG_NONBLOCK O_NONBLOCK
#endif
#if __IMOUOS_USE_LIBLOG_READER_INTERFACE > 2
#define IMOUOS_LOG_WRAP 0x40000000  /* Block until buffer about to wrap */
#define IMOUOS_LOG_WRAP_DEFAULT_TIMEOUT 7200 /* 2 hour default */
#endif
#if __IMOUOS_USE_LIBLOG_READER_INTERFACE > 1
#define IMOUOS_LOG_PSTORE 0x80000000
#endif

/*
 * Native log reading interface section. See logcat for sample code.
 *
 * The preferred API is an exec of logcat. Likely uses of this interface
 * are if native code sufg192fers from exec or filtration being too costly,
 * access to raw information, or parsing is an issue.
 */

/*
 * The userspace structure for version 1 of the logger_entry ABI.
 */
#ifndef __struct_logger_entry_defined
#define __struct_logger_entry_defined
struct logger_entry {
  uint16_t len;   /* length of the payload */
  uint16_t __pad; /* no matter what, we get 2 bytes of padding */
  int32_t pid;    /* generating process's pid */
  int32_t tid;    /* generating process's tid */
  int32_t sec;    /* seconds since Epoch */
  int32_t nsec;   /* nanoseconds */
#ifndef __cplusplus
  char msg[0]; /* the entry's payload */
#endif
};
#endif

/*
 * The userspace structure for version 2 of the logger_entry ABI.
 */
#ifndef __struct_logger_entry_v2_defined
#define __struct_logger_entry_v2_defined
struct logger_entry_v2 {
  uint16_t len;      /* length of the payload */
  uint16_t hdr_size; /* sizeof(struct logger_entry_v2) */
  int32_t pid;       /* generating process's pid */
  int32_t tid;       /* generating process's tid */
  int32_t sec;       /* seconds since Epoch */
  int32_t nsec;      /* nanoseconds */
  uint32_t euid;     /* effective UID of logger */
#ifndef __cplusplus
  char msg[0]; /* the entry's payload */
#endif
} __attribute__((__packed__));
#endif

/*
 * The userspace structure for version 3 of the logger_entry ABI.
 */
#ifndef __struct_logger_entry_v3_defined
#define __struct_logger_entry_v3_defined
struct logger_entry_v3 {
  uint16_t len;      /* length of the payload */
  uint16_t hdr_size; /* sizeof(struct logger_entry_v3) */
  int32_t pid;       /* generating process's pid */
  int32_t tid;       /* generating process's tid */
  int32_t sec;       /* seconds since Epoch */
  int32_t nsec;      /* nanoseconds */
  uint32_t lid;      /* log id of the payload */
#ifndef __cplusplus
  char msg[0]; /* the entry's payload */
#endif
} __attribute__((__packed__));
#endif

/*
 * The userspace structure for version 4 of the logger_entry ABI.
 */
#ifndef __struct_logger_entry_v4_defined
#define __struct_logger_entry_v4_defined
struct logger_entry_v4 {
  uint16_t len;      /* length of the payload */
  uint16_t hdr_size; /* sizeof(struct logger_entry_v4) */
  int32_t pid;       /* generating process's pid */
  uint32_t tid;      /* generating process's tid */
  uint32_t sec;      /* seconds since Epoch */
  uint32_t nsec;     /* nanoseconds */
  uint32_t lid;      /* log id of the payload, bottom 4 bits currently */
  uint32_t uid;      /* generating process's uid */
#ifndef __cplusplus
  char msg[0]; /* the entry's payload */
#endif
};
#endif

/*
 * The maximum size of the log entry payload that can be
 * written to the logger. An attempt to write more than
 * this amount will result in a truncated log entry.
 */
#define LOGGER_ENTRY_MAX_PAYLOAD 4068

/*
 * The maximum size of a log entry which can be read.
 * An attempt to read less than this amount may result
 * in read() returning EINVAL.
 */
#define LOGGER_ENTRY_MAX_LEN (5 * 1024)

#ifndef __struct_log_msg_defined
#define __struct_log_msg_defined
struct log_msg {
    union {
        unsigned char buf[LOGGER_ENTRY_MAX_LEN + 1];
        struct logger_entry_v4 entry;
        struct logger_entry_v4 entry_v4;
        struct logger_entry_v3 entry_v3;
        struct logger_entry_v2 entry_v2;
        struct logger_entry entry_v1;
    } __attribute__((aligned(4)));
};
#endif

struct logger;
/// logger系列接口对象可由imouos_logger_open获取

/// 获取logger对应的ID
///
/// \param  [in]  无
/// \param  [out] 无
/// \retval return positive success, or negative errno
int  imouos_logger_get_id(struct logger* logger);

/// 清除logger
///
/// \param  [in]  无
/// \param  [out] 无
/// \retval return positive success, or negative errno
int  imouos_logger_clear(struct logger* logger);

/// 获取日志大小
///
/// \param  [in]  无
/// \param  [out] 无
/// \retval return positive success, or negative errno
long imouos_logger_get_log_size(struct logger* logger);

/// 设置日志大小
///
/// \param  [in]  size
/// \param  [out] 无
/// \retval return positive success, or negative errno
int  imouos_logger_set_log_size(struct logger* logger, unsigned long size);

/// 获取可读的日志大小
///
/// \param  [in]  size
/// \param  [out] 无
/// \retval return positive success, or negative errno
long imouos_logger_get_log_readable_size(struct logger* logger);

/// 获取log版本
///
/// \param  [in]  size
/// \param  [out] 无
/// \retval return positive success, or negative errno
int  imouos_logger_get_log_version(struct logger* logger);

struct logger_list;
#if __IMOUOS_USE_LIBLOG_READER_INTERFACE > 1

/// 获取log日志量
///
/// \param  [in]  size
/// \param  [out] 无
/// \retval return positive success, or negative errno
ssize_t imouos_logger_get_statistics(struct logger_list* logger_list,
                                               char* buf,
                                               size_t len);

/// 获取删除的日志
///
/// \param  [in]  size
/// \param  [out] 无
/// \retval return positive success, or negative errno
ssize_t imouos_logger_get_prune_list(struct logger_list* logger_list,
                                               char* buf,
                                               size_t len);

/// 指定删除的日志
///
/// \param  [in]  size
/// \param  [out] 无
/// \retval return positive success, or negative errno
int32_t imouos_logger_set_prune_list(struct logger_list* logger_list,
                                               char* buf,
                                               size_t len);
#endif

/// 按照给定条目数申请一个读日志对象
///
/// \param  [in ] mode: 日志读取模式：阻塞或非阻塞，eg：IMOUOS_LOG_NONBLOCK
/// \param  [in ] tail: 需要读取的条数
/// \param  [in ] pid:  需要读取日志的pid
/// \param  [out] 无
/// \retval return logger_list
struct logger_list* imouos_logger_list_alloc(int mode, 
                                                     unsigned int tail, 
                                                     pid_t pid);

/// 按照给定start时间申请读日志对象
///
/// \param  [in ] mode:  日志读取模式：阻塞或非阻塞，eg：IMOUOS_LOG_NONBLOCK
/// \param  [in ] start: 需要读取的起始时间
/// \param  [in ] pid:   需要读取日志的pid
/// \param  [out] 无
/// \retval return logger_list
struct logger_list* imouos_logger_list_alloc_time(int mode,
                                                            struct timespec start,
                                                            pid_t pid);
/// 释放读日志对象
///
/// \param  [in ] logger_list: 需要释放的日志操作对象
/// \param  [out] 无
/// \retval return logger_list
void imouos_logger_list_free(struct logger_list* logger_list);

/// In the purest sense, the following two are orthogonal interfaces */
/// Read from the selected logs
/// 读取指定对象日志
///
/// \param  [in ] logger_list: 需要释放的日志操作对象
/// \param  [out] 无
/// \retval return logger_list
int  imouos_logger_list_read(struct logger_list* logger_list,
                                     struct log_msg* log_msg);

/// Open the named log and add it to the logger list
/// 打开指定ID对应的logger对象
///
/// \param  [in ] logger_list: 需要释放的日志操作对象
/// \param  [out] 无
/// \retval 返回一个可操作的logger对象
struct logger* imouos_logger_open(struct logger_list* logger_list,
                                         imouos_logId id);

#endif /* _LIBS_LOG_LOG_H */


