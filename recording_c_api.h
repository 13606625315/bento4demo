/*******************************************************************************
* recording_c_api.h
*
* Copyright (C) 2024 Recording System
*
* Author: AI Assistant
* Version: V1.0.0 2024-01-01 Create
* Description: 清扫机器人录像系统C接口
*
* 功能说明:
*   1. 提供录像开始、暂停、停止等控制接口
*   2. 集成imouos视频流订阅
*   3. 支持录像配置和状态查询
*   4. 线程安全的C接口封装
*
*******************************************************************************/

#ifndef __RECORDING_C_API_H__
#define __RECORDING_C_API_H__

#include "include/Framework/MediaCodec/mediaRpcApi.h"
#include "include/Framework/MediaCodec/mediaTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

// 录像系统错误码
typedef enum {
    RECORDING_SUCCESS = 0,              ///< 成功
    RECORDING_ERROR_INVALID_PARAM = -1, ///< 无效参数
    RECORDING_ERROR_NOT_INITIALIZED = -2, ///< 未初始化
    RECORDING_ERROR_ALREADY_STARTED = -3, ///< 已经开始录像
    RECORDING_ERROR_NOT_STARTED = -4,    ///< 录像未开始
    RECORDING_ERROR_STORAGE_FULL = -5,   ///< 存储空间不足
    RECORDING_ERROR_FILE_OPERATION = -6, ///< 文件操作失败
    RECORDING_ERROR_MEMORY_ALLOC = -7,   ///< 内存分配失败
    RECORDING_ERROR_INTERNAL = -8        ///< 内部错误
} RecordingErrorCode;

// 录像状态
typedef enum {
    RECORDING_STATE_STOPPED = 0,    ///< 停止状态
    RECORDING_STATE_RECORDING = 1,  ///< 录像中
} RecordingState;

// 录像配置结构体
typedef struct {
    const char* storage_path;           ///< 存储路径
    unsigned int segment_duration_sec;  ///< 分段时长(秒)
    unsigned int max_storage_gb;        ///< 最大存储空间(GB)
    unsigned int memory_pool_size_mb;   ///< 内存池大小(MB)
    unsigned int min_file_duration_sec; ///< 最小文件时长(秒)
    int enable_audio;                   ///< 是否启用音频录制
    int video_stream_type;              ///< 视频流类型(VideoStreamType)
} RecordingConfig;

// 录像状态信息
typedef struct {
    RecordingState state;               ///< 当前状态
    unsigned long long total_size_bytes; ///< 总录像大小(字节)
    unsigned int file_count;            ///< 录像文件数量
    unsigned long long available_space_bytes; ///< 可用存储空间(字节)
    unsigned int current_segment_duration_sec; ///< 当前段录像时长(秒)
    const char* current_filename;       ///< 当前录像文件名
    const char* last_error;             ///< 最后错误信息
} RecordingStatus;

/**
 * @brief 初始化录像系统
 * @param config 录像配置参数
 * @return RecordingErrorCode 错误码
 */
RecordingErrorCode recording_init(const RecordingConfig* config);

/**
 * @brief 反初始化录像系统
 * @return RecordingErrorCode 错误码
 */
RecordingErrorCode recording_deinit(void);

/**
 * @brief 开始录像
 * @param channel_id 通道ID(通常为0)
 * @return RecordingErrorCode 错误码
 */
RecordingErrorCode recording_start(int channel_id);


/**
 * @brief 停止录像
 * @return RecordingErrorCode 错误码
 */
RecordingErrorCode recording_stop(void);

/**
 * @brief 获取录像状态
 * @param status 状态信息输出
 * @return RecordingErrorCode 错误码
 */
RecordingErrorCode recording_get_status(RecordingStatus* status);

/**
 * @brief 更新录像配置
 * @param config 新的配置参数
 * @return RecordingErrorCode 错误码
 */
RecordingErrorCode recording_update_config(const RecordingConfig* config);

/**
 * @brief 手动触发文件分割
 * @return RecordingErrorCode 错误码
 */
RecordingErrorCode recording_split_file(void);

/**
 * @brief 清理旧录像文件
 * @param keep_duration_hours 保留时长(小时)
 * @return RecordingErrorCode 错误码
 */
RecordingErrorCode recording_cleanup_old_files(unsigned int keep_duration_hours);

/**
 * @brief 获取录像文件列表
 * @param file_list 文件列表输出(调用者负责释放内存)
 * @param count 文件数量输出
 * @return RecordingErrorCode 错误码
 */
RecordingErrorCode recording_get_file_list(char*** file_list, unsigned int* count);

/**
 * @brief 释放文件列表内存
 * @param file_list 文件列表
 * @param count 文件数量
 */
void recording_free_file_list(char** file_list, unsigned int count);

/**
 * @brief 设置错误回调函数
 * @param callback 错误回调函数
 */
typedef void (*RecordingErrorCallback)(RecordingErrorCode error_code, const char* error_msg);
void recording_set_error_callback(RecordingErrorCallback callback);

/**
 * @brief 获取版本信息
 * @return 版本字符串
 */
const char* recording_get_version(void);

#ifdef __cplusplus
}
#endif

#endif /* __RECORDING_C_API_H__ */