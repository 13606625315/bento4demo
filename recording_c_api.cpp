/*******************************************************************************
* recording_c_api.cpp
*
* Copyright (C) 2024 Recording System
*
* Author: AI Assistant
* Version: V1.0.0 2024-01-01 Create
* Description: 清扫机器人录像系统C接口实现
*
*******************************************************************************/

#include "recording_c_api.h"
#include "VideoRecordingManager.h"
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <cstring>
#include <thread>

// 全局变量
static std::unique_ptr<VideoRecordingManager> g_recording_manager = nullptr;
static std::mutex g_recording_mutex;
static RecordingErrorCallback g_error_callback = nullptr;
static RecordingConfig g_current_config = {
    nullptr,  // storage_path
    0,        // segment_duration_sec
    0,        // max_storage_gb
    0,        // memory_pool_size_mb
    0,        // min_file_duration_sec
    0,        // enable_audio
    0         // video_stream_type
};
static std::string g_last_error_msg;
static bool g_is_initialized = false;
static int g_current_channel_id = -1;

// 内部函数声明
static void internal_error_handler(const std::string& error_msg, RecordingErrorCode error_code);
static Int32 video_stream_callback(Int32 chId, Int32 streamId, media_pkt mPkt);

// 版本信息
static const char* RECORDING_VERSION = "1.0.0";

/**
 * @brief 视频流回调函数
 */
static Int32 video_stream_callback(Int32 /* chId */, Int32 /* streamId */, media_pkt /* mPkt */) {
    std::lock_guard<std::mutex> lock(g_recording_mutex);
    
    if (!g_recording_manager || !g_is_initialized) {
        return -1;
    }
    
    try {
        // 这里需要根据实际的media_pkt格式解析数据
        // 由于media_pkt是imouos_handle类型，需要使用相应的API解析
        // 暂时使用模拟数据，实际使用时需要替换为真实的数据解析
        
        // 模拟H.264数据包
        std::vector<uint8_t> frame_data;
        
        // TODO: 实际实现中需要:
        // 1. 使用imouos相关API解析media_pkt获取实际数据
        // 2. 解析DHAV格式: STP_Head(128) + Extra + data + STP_Tail(8)
        // 3. 提取H.264/H.265数据
        // 4. 判断帧类型(I帧/P帧/B帧)
        
        // 示例代码(需要根据实际API替换):
        /*
        // 获取数据包大小
        size_t packet_size = imouos_get_packet_size(mPkt);
        if (packet_size > 0) {
            frame_data.resize(packet_size);
            // 获取数据包内容
            if (imouos_get_packet_data(mPkt, frame_data.data(), packet_size) == 0) {
                // 解析DHAV格式，提取视频数据
                // ...
            }
        }
        */
        
        // 临时使用模拟数据
        if (frame_data.empty()) {
            // 生成模拟的H.264 I帧数据
            frame_data = {
                0x00, 0x00, 0x00, 0x01, 0x67, 0x42, 0x00, 0x1E, // SPS
                0x00, 0x00, 0x00, 0x01, 0x68, 0xCE, 0x3C, 0x80, // PPS
                0x00, 0x00, 0x00, 0x01, 0x65, 0x88, 0x84, 0x00  // I帧开始
            };
        }
        
        // 添加视频帧到录像管理器
        bool is_keyframe = true; // 需要根据实际数据判断
        // 移除未使用的timestamp变量
        if (!g_recording_manager->addVideoFrame(frame_data.data(), frame_data.size(), 
                                               is_keyframe)) {
            internal_error_handler("Failed to add video frame", RECORDING_ERROR_INTERNAL);
            return -1;
        }
        
        return 0;
    } catch (const std::exception& e) {
        internal_error_handler(std::string("Video callback error: ") + e.what(), 
                             RECORDING_ERROR_INTERNAL);
        return -1;
    }
}

// 移除未使用的convert_error_code函数

/**
 * @brief 内部错误处理
 */
static void internal_error_handler(const std::string& error_msg, RecordingErrorCode error_code) {
    g_last_error_msg = error_msg;
    if (g_error_callback) {
        g_error_callback(error_code, error_msg.c_str());
    }
}

// C接口实现

RecordingErrorCode recording_init(const RecordingConfig* config) {
    if (!config) {
        return RECORDING_ERROR_INVALID_PARAM;
    }
    
    std::lock_guard<std::mutex> lock(g_recording_mutex);
    
    if (g_is_initialized) {
        return RECORDING_ERROR_ALREADY_STARTED;
    }
    
    try {
        // 保存配置
        g_current_config = *config;
        
        // 创建录像管理器配置
        VideoRecordingConfig manager_config;
        manager_config.storage_path = config->storage_path ? config->storage_path : "./recordings";
        manager_config.segment_duration_minutes = config->segment_duration_sec > 0 ? 
                                                  config->segment_duration_sec / 60 : 5; // 转换为分钟，默认5分钟
        manager_config.max_storage_size = config->max_storage_gb > 0 ? 
                                          config->max_storage_gb * 1024LL * 1024 * 1024 : 6LL * 1024 * 1024 * 1024; // 转换为字节，默认6GB
        // 注意：VideoRecordingConfig中没有memory_pool_size_mb、min_file_duration_sec和enable_audio成员
        // 这些配置项在当前版本的VideoRecordingConfig中不可用
        
        // 创建录像管理器
        g_recording_manager = std::make_unique<VideoRecordingManager>(manager_config);
        
        // VideoRecordingManager 构造函数已经使用配置参数初始化
        
        g_is_initialized = true;
        return RECORDING_SUCCESS;
        
    } catch (const std::exception& e) {
        g_recording_manager.reset();
        internal_error_handler(std::string("Initialization error: ") + e.what(), 
                             RECORDING_ERROR_INTERNAL);
        return RECORDING_ERROR_INTERNAL;
    }
}

RecordingErrorCode recording_deinit(void) {
    std::lock_guard<std::mutex> lock(g_recording_mutex);
    
    if (!g_is_initialized) {
        return RECORDING_ERROR_NOT_INITIALIZED;
    }
    
    try {
        // 停止录像
        if (g_recording_manager) {
            g_recording_manager->stopRecording();
        }
        
        // 取消视频流订阅
        if (g_current_channel_id >= 0) {
            // 注意: imouos可能没有取消订阅的API，需要根据实际情况处理
            g_current_channel_id = -1;
        }
        
        // 销毁录像管理器
        g_recording_manager.reset();
        g_is_initialized = false;
        
        return RECORDING_SUCCESS;
        
    } catch (const std::exception& e) {
        internal_error_handler(std::string("Deinitialization error: ") + e.what(), 
                             RECORDING_ERROR_INTERNAL);
        return RECORDING_ERROR_INTERNAL;
    }
}

IMOUOS_API Int32 imouos_mediaVideoSubStream(Int32 chId, Int32 streamId, videoStreamEncProc streamProc)
{
    return 0;
}

RecordingErrorCode recording_start(int channel_id) {
    std::lock_guard<std::mutex> lock(g_recording_mutex);
    
    if (!g_is_initialized || !g_recording_manager) {
        return RECORDING_ERROR_NOT_INITIALIZED;
    }
    
    try {
        // 开始录像
        if (!g_recording_manager->startRecording()) {
            internal_error_handler("Failed to start recording", RECORDING_ERROR_INTERNAL);
            return RECORDING_ERROR_INTERNAL;
        }
        
        // 订阅视频流
        Int32 stream_type = g_current_config.video_stream_type;
        if (stream_type < videoEncTypeMain || stream_type >= videoEncTypeExNumber) {
            stream_type = videoEncTypeMain; // 默认主码流
        }
        
        Int32 result = imouos_mediaVideoSubStream(channel_id, stream_type, video_stream_callback);
        if (result != 0) {
            g_recording_manager->stopRecording();
            internal_error_handler("Failed to subscribe video stream", RECORDING_ERROR_INTERNAL);
            return RECORDING_ERROR_INTERNAL;
        }
        
        g_current_channel_id = channel_id;
        return RECORDING_SUCCESS;
        
    } catch (const std::exception& e) {
        internal_error_handler(std::string("Start recording error: ") + e.what(), 
                             RECORDING_ERROR_INTERNAL);
        return RECORDING_ERROR_INTERNAL;
    }
}

RecordingErrorCode recording_stop(void) {
    std::lock_guard<std::mutex> lock(g_recording_mutex);
    
    if (!g_is_initialized || !g_recording_manager) {
        return RECORDING_ERROR_NOT_INITIALIZED;
    }
    
    try {
        // 停止录像
        if (!g_recording_manager->stopRecording()) {
            internal_error_handler("Failed to stop recording", RECORDING_ERROR_INTERNAL);
            return RECORDING_ERROR_INTERNAL;
        }
        
        // 重置通道ID
        g_current_channel_id = -1;
        
        return RECORDING_SUCCESS;
    } catch (const std::exception& e) {
        internal_error_handler(std::string("Stop recording error: ") + e.what(), 
                             RECORDING_ERROR_INTERNAL);
        return RECORDING_ERROR_INTERNAL;
    }
}

RecordingErrorCode recording_get_status(RecordingStatus* status) {
    if (!status) {
        return RECORDING_ERROR_INVALID_PARAM;
    }
    
    std::lock_guard<std::mutex> lock(g_recording_mutex);
    
    if (!g_is_initialized || !g_recording_manager) {
        return RECORDING_ERROR_NOT_INITIALIZED;
    }
    
    try {
        VideoRecordingStatus manager_status = g_recording_manager->GetStatus();
        
        // 转换状态
        if (manager_status.is_recording) {
            status->state = RECORDING_STATE_RECORDING;
        } else {
            status->state = RECORDING_STATE_STOPPED;
        }
        
        status->total_size_bytes = manager_status.total_size_bytes;
        status->file_count = manager_status.file_count;
        status->available_space_bytes = manager_status.available_space_bytes;
        status->current_segment_duration_sec = manager_status.current_segment_duration_sec;
        status->current_filename = manager_status.current_filename.c_str();
        status->last_error = g_last_error_msg.c_str();
        
        return RECORDING_SUCCESS;
    } catch (const std::exception& e) {
        internal_error_handler(std::string("Get status error: ") + e.what(), 
                             RECORDING_ERROR_INTERNAL);
        return RECORDING_ERROR_INTERNAL;
    }
}

RecordingErrorCode recording_update_config(const RecordingConfig* config) {
    if (!config) {
        return RECORDING_ERROR_INVALID_PARAM;
    }
    
    std::lock_guard<std::mutex> lock(g_recording_mutex);
    
    if (!g_is_initialized || !g_recording_manager) {
        return RECORDING_ERROR_NOT_INITIALIZED;
    }
    
    try {
        // 更新配置
        VideoRecordingConfig manager_config;
        manager_config.storage_path = config->storage_path ? config->storage_path : g_current_config.storage_path;
        manager_config.segment_duration_minutes = config->segment_duration_sec / 60;
        manager_config.max_storage_size = config->max_storage_gb * 1024LL * 1024 * 1024;
        // 注意：VideoRecordingConfig中没有memory_pool_size_mb、min_file_duration_sec和enable_audio成员
        
        g_recording_manager->updateConfig(manager_config);
        // updateConfig方法返回void，假设总是成功
        
        g_current_config = *config;
        return RECORDING_SUCCESS;
    } catch (const std::exception& e) {
        internal_error_handler(std::string("Update config error: ") + e.what(), 
                             RECORDING_ERROR_INTERNAL);
        return RECORDING_ERROR_INTERNAL;
    }
}

RecordingErrorCode recording_split_file(void) {
    std::lock_guard<std::mutex> lock(g_recording_mutex);
    
    if (!g_is_initialized || !g_recording_manager) {
        return RECORDING_ERROR_NOT_INITIALIZED;
    }
    
    try {
        if (!g_recording_manager->SplitCurrentFile()) {
            internal_error_handler("Failed to split file", RECORDING_ERROR_INTERNAL);
            return RECORDING_ERROR_INTERNAL;
        }
        return RECORDING_SUCCESS;
    } catch (const std::exception& e) {
        internal_error_handler(std::string("Split file error: ") + e.what(), 
                             RECORDING_ERROR_INTERNAL);
        return RECORDING_ERROR_INTERNAL;
    }
}

RecordingErrorCode recording_cleanup_old_files(unsigned int keep_duration_hours) {
    std::lock_guard<std::mutex> lock(g_recording_mutex);
    
    if (!g_is_initialized || !g_recording_manager) {
        return RECORDING_ERROR_NOT_INITIALIZED;
    }
    
    try {
        g_recording_manager->CleanupOldFiles(keep_duration_hours * 3600); // 转换为秒
        return RECORDING_SUCCESS;
    } catch (const std::exception& e) {
        internal_error_handler(std::string("Cleanup files error: ") + e.what(), 
                             RECORDING_ERROR_INTERNAL);
        return RECORDING_ERROR_INTERNAL;
    }
}

RecordingErrorCode recording_get_file_list(char*** file_list, unsigned int* count) {
    if (!file_list || !count) {
        return RECORDING_ERROR_INVALID_PARAM;
    }
    
    std::lock_guard<std::mutex> lock(g_recording_mutex);
    
    if (!g_is_initialized || !g_recording_manager) {
        return RECORDING_ERROR_NOT_INITIALIZED;
    }
    
    try {
        std::vector<std::string> files = g_recording_manager->GetRecordingFiles();
        
        *count = static_cast<unsigned int>(files.size());
        if (*count == 0) {
            *file_list = nullptr;
            return RECORDING_SUCCESS;
        }
        
        // 分配内存
        *file_list = static_cast<char**>(malloc(*count * sizeof(char*)));
        if (!*file_list) {
            return RECORDING_ERROR_MEMORY_ALLOC;
        }
        
        for (unsigned int i = 0; i < *count; i++) {
            (*file_list)[i] = static_cast<char*>(malloc(files[i].length() + 1));
            if (!(*file_list)[i]) {
                // 清理已分配的内存
                for (unsigned int j = 0; j < i; j++) {
                    free((*file_list)[j]);
                }
                free(*file_list);
                *file_list = nullptr;
                *count = 0;
                return RECORDING_ERROR_MEMORY_ALLOC;
            }
            strcpy((*file_list)[i], files[i].c_str());
        }
        
        return RECORDING_SUCCESS;
    } catch (const std::exception& e) {
        internal_error_handler(std::string("Get file list error: ") + e.what(), 
                             RECORDING_ERROR_INTERNAL);
        return RECORDING_ERROR_INTERNAL;
    }
}

void recording_free_file_list(char** file_list, unsigned int count) {
    if (!file_list) {
        return;
    }
    
    for (unsigned int i = 0; i < count; i++) {
        if (file_list[i]) {
            free(file_list[i]);
        }
    }
    free(file_list);
}

void recording_set_error_callback(RecordingErrorCallback callback) {
    std::lock_guard<std::mutex> lock(g_recording_mutex);
    g_error_callback = callback;
}

const char* recording_get_version(void) {
    return RECORDING_VERSION;
}