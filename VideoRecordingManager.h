#ifndef VIDEO_RECORDING_MANAGER_H
#define VIDEO_RECORDING_MANAGER_H

#include "MP4Converter.h"
#include <string>
#include <vector>
#include <queue>
#include <memory>
#include <chrono>
#include <mutex>
#include <thread>
#include <atomic>
#include <map>
#include <fstream>

// 录像文件信息结构
struct VideoFileInfo {
    std::string filename;
    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point end_time;
    size_t file_size;
    bool is_complete;
    uint32_t checksum;
    
    VideoFileInfo() : file_size(0), is_complete(false), checksum(0) {}
};

// 录像状态信息结构
struct VideoRecordingStatus {
    bool is_recording;
    unsigned long long total_size_bytes;
    unsigned int file_count;
    unsigned long long available_space_bytes;
    unsigned int current_segment_duration_sec;
    std::string current_filename;
    
    VideoRecordingStatus() : is_recording(false), 
                           total_size_bytes(0), file_count(0), 
                           available_space_bytes(0), current_segment_duration_sec(0) {}
};

// 录像配置参数
struct VideoRecordingConfig {
    size_t max_storage_size = 6LL * 1024 * 1024 * 1024; // 6GB
    int segment_duration_minutes = 5;                    // 5分钟分割
    size_t estimated_file_size = 5 * 1024 * 1024;      // 预估5MB每个文件
    std::string storage_path = "./recordings/";
    std::string codec_type = "h264";
    int video_width = 1920;
    int video_height = 1080;
    double video_fps = 25.0;
    bool enable_compression = true;
    double storage_cleanup_threshold = 0.9;              // 90%时开始清理
};

// 内存池管理
class MemoryPool {
public:
    MemoryPool(size_t pool_size, size_t block_size);
    ~MemoryPool();
    
    void* allocate();
    void deallocate(void* ptr);
    size_t getAvailableBlocks() const;
    
private:
    std::vector<uint8_t> m_pool;
    std::queue<void*> m_free_blocks;
    size_t m_block_size;
    mutable std::mutex m_mutex;
};

// 录像存储管理器
class VideoRecordingManager {
public:
    VideoRecordingManager(const VideoRecordingConfig& config = VideoRecordingConfig());
    ~VideoRecordingManager();
    
    // 录像控制接口
    bool startRecording();
    bool pauseRecording();
    bool stopRecording();
    bool isRecording() const { return m_is_recording; }
    
    // 添加视频帧
    bool addVideoFrame(const unsigned char* data, size_t size, bool is_keyframe);
    
    // 配置管理
    void updateConfig(const VideoRecordingConfig& config);
    VideoRecordingConfig getConfig() const { return m_config; }
    
    // 存储状态查询
    size_t getUsedStorage() const;
    size_t getAvailableStorage() const;
    std::vector<VideoFileInfo> getFileList() const;
    VideoRecordingStatus GetStatus() const;
    
    // 文件管理
    bool deleteFile(const std::string& filename);
    void cleanupOldFiles();
    void CleanupOldFiles(unsigned int keep_duration_seconds);
    bool SplitCurrentFile();
    std::vector<std::string> GetRecordingFiles() const;
    
private:
    // 内部状态
    VideoRecordingConfig m_config;
    std::atomic<bool> m_is_recording;
    std::atomic<bool> m_should_stop;
    
    // 当前录像状态
    std::unique_ptr<MP4Converter> m_current_converter;
    std::chrono::system_clock::time_point m_current_segment_start;
    std::string m_current_filename;
    
    // 文件管理
    std::vector<VideoFileInfo> m_file_list;
    mutable std::mutex m_file_list_mutex;
    
    // 内存管理
    std::unique_ptr<MemoryPool> m_memory_pool;
    
    // 工作线程
    std::thread m_cleanup_thread;
    std::mutex m_recording_mutex;
    
    // 内部方法
    bool initializeStorage();
    std::string generateFilename(const std::chrono::system_clock::time_point& start_time,
                                const std::chrono::system_clock::time_point& end_time);
    bool startNewSegment();
    bool finishCurrentSegment();
    void cleanupThread();
    bool checkStorageSpace();
    void deleteOldestFile();
    uint32_t calculateChecksum(const std::string& filepath);
    void updateFileInfo(const std::string& filename, bool is_complete);
    bool createDirectoryIfNotExists(const std::string& path);
    
    // 时间工具
    std::string timePointToString(const std::chrono::system_clock::time_point& tp);
    std::chrono::system_clock::time_point stringToTimePoint(const std::string& str);
    
    // 异常处理
    void handleRecordingError(const std::string& error_msg);
    bool recoverFromError();
};

// 全局录像管理器实例
class GlobalRecordingManager {
public:
    static VideoRecordingManager& getInstance();
    static void cleanup();
    
private:
    static std::unique_ptr<VideoRecordingManager> s_instance;
    static std::mutex s_mutex;
};

#endif // VIDEO_RECORDING_MANAGER_H