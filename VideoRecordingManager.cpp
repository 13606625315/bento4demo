#include "VideoRecordingManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>
// 兼容性头文件：支持不同编译器和 C++ 标准
#if __cplusplus >= 201703L && __has_include(<filesystem>)
    #include <filesystem>
    namespace fs = std::filesystem;
    #define HAS_STD_FILESYSTEM 1
#elif __has_include(<experimental/filesystem>)
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
    #define HAS_STD_FILESYSTEM 1
#else
    #define HAS_STD_FILESYSTEM 0
    // 使用传统方法的兼容性实现
    #ifdef _WIN32
        #include <io.h>
        #include <windows.h>
    #else
        #include <dirent.h>
        #include <sys/stat.h>
    #endif
#endif
#include <fstream>
#include <algorithm>
#include <ctime>
#include <signal.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

// MemoryPool 实现
MemoryPool::MemoryPool(size_t pool_size, size_t block_size) 
    : m_block_size(block_size) {
    size_t num_blocks = pool_size / block_size;
    m_pool.resize(pool_size);
    
    // 初始化空闲块队列
    for (size_t i = 0; i < num_blocks; ++i) {
        m_free_blocks.push(&m_pool[i * block_size]);
    }
}

MemoryPool::~MemoryPool() {
    // 析构函数，内存会自动释放
}

void* MemoryPool::allocate() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_free_blocks.empty()) {
        return nullptr;
    }
    
    void* ptr = m_free_blocks.front();
    m_free_blocks.pop();
    return ptr;
}

void MemoryPool::deallocate(void* ptr) {
    if (!ptr) return;
    
    std::lock_guard<std::mutex> lock(m_mutex);
    m_free_blocks.push(ptr);
}

size_t MemoryPool::getAvailableBlocks() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_free_blocks.size();
}

// VideoRecordingManager 实现
VideoRecordingManager::VideoRecordingManager(const VideoRecordingConfig& config)
    : m_config(config)
    , m_is_recording(false)
    , m_should_stop(false) {
    
    // 初始化存储
    if (!initializeStorage()) {
        throw std::runtime_error("Failed to initialize storage");
    }
    
    // 创建内存池
    size_t pool_size = m_config.max_storage_size / 10; // 使用10%作为内存池
    size_t block_size = m_config.estimated_file_size;
    m_memory_pool = std::make_unique<MemoryPool>(pool_size, block_size);
    
    // 启动清理线程
    m_cleanup_thread = std::thread(&VideoRecordingManager::cleanupThread, this);
    
    // 注册信号处理器
#ifndef _WIN32
    signal(SIGTERM, [](int) {
        GlobalRecordingManager::getInstance().stopRecording();
    });
    signal(SIGINT, [](int) {
        GlobalRecordingManager::getInstance().stopRecording();
    });
#endif
}

VideoRecordingManager::~VideoRecordingManager() {
    stopRecording();
    m_should_stop = true;
    
    if (m_cleanup_thread.joinable()) {
        m_cleanup_thread.join();
    }
}

bool VideoRecordingManager::startRecording() {
    std::lock_guard<std::mutex> lock(m_recording_mutex);
    
    if (m_is_recording) {
        std::cout << "录像已经在进行中" << std::endl;
        return true;
    }
    
    // 检查存储空间
    if (!checkStorageSpace()) {
        std::cout << "存储空间不足，无法开始录像" << std::endl;
        return false;
    }
    
    // 开始新的录像段
    if (!startNewSegment()) {
        std::cout << "无法开始新的录像段" << std::endl;
        return false;
    }
    
    m_is_recording = true;
    std::cout << "录像开始: " << m_current_filename << std::endl;
    return true;
}

bool VideoRecordingManager::pauseRecording() {
    std::lock_guard<std::mutex> lock(m_recording_mutex);
    
    if (!m_is_recording) {
        return true;
    }
    
    // 完成当前录像段
    finishCurrentSegment();
    m_is_recording = false;
    
    std::cout << "录像暂停" << std::endl;
    return true;
}

bool VideoRecordingManager::stopRecording() {
    std::lock_guard<std::mutex> lock(m_recording_mutex);
    
    if (!m_is_recording) {
        return true;
    }
    
    // 完成当前录像段
    finishCurrentSegment();
    m_is_recording = false;
    
    std::cout << "录像停止" << std::endl;
    return true;
}

bool VideoRecordingManager::addVideoFrame(const unsigned char* data, size_t size, bool is_keyframe) {
    if (!m_is_recording || !m_current_converter) {
        return false;
    }
    
    // 检查是否需要分割文件
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(now - m_current_segment_start);
    
    if (duration.count() >= m_config.segment_duration_minutes) {
        std::lock_guard<std::mutex> lock(m_recording_mutex);
        
        // 完成当前段并开始新段
        finishCurrentSegment();
        if (!startNewSegment()) {
            handleRecordingError("无法开始新的录像段");
            return false;
        }
    }
    
    // 添加帧到当前转换器
    AP4_UI64 dts = 0, cts = 0;
    AP4_Result result = m_current_converter->AddSample(data, size, is_keyframe, dts, cts);
    
    if (result != AP4_SUCCESS) {
        handleRecordingError("添加视频帧失败");
        return false;
    }
    
    return true;
}

void VideoRecordingManager::updateConfig(const VideoRecordingConfig& config) {
    std::lock_guard<std::mutex> lock(m_recording_mutex);
    m_config = config;
}

size_t VideoRecordingManager::getUsedStorage() const {
    std::lock_guard<std::mutex> lock(m_file_list_mutex);
    size_t total_size = 0;
    for (const auto& file_info : m_file_list) {
        total_size += file_info.file_size;
    }
    return total_size;
}

size_t VideoRecordingManager::getAvailableStorage() const {
    return m_config.max_storage_size - getUsedStorage();
}

std::vector<VideoFileInfo> VideoRecordingManager::getFileList() const {
    std::lock_guard<std::mutex> lock(m_file_list_mutex);
    return m_file_list;
}

VideoRecordingStatus VideoRecordingManager::GetStatus() const {
    VideoRecordingStatus status;
    
    // 获取录像状态
    status.is_recording = m_is_recording.load();
    
    // 获取存储信息
    status.total_size_bytes = getUsedStorage();
    status.available_space_bytes = getAvailableStorage();
    
    // 获取文件数量
    {
        std::lock_guard<std::mutex> lock(m_file_list_mutex);
        status.file_count = static_cast<unsigned int>(m_file_list.size());
    }
    
    // 获取当前录像段信息
    if (status.is_recording && !m_current_filename.empty()) {
        status.current_filename = m_current_filename;
        
        // 计算当前录像段时长（秒）
        auto now = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - m_current_segment_start);
        status.current_segment_duration_sec = static_cast<unsigned int>(duration.count());
    } else {
        status.current_filename = "";
        status.current_segment_duration_sec = 0;
    }
    
    return status;
}

bool VideoRecordingManager::deleteFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_file_list_mutex);
    
    // 从文件列表中移除
    auto it = std::find_if(m_file_list.begin(), m_file_list.end(),
        [&filename](const VideoFileInfo& info) {
            return info.filename == filename;
        });
    
    if (it != m_file_list.end()) {
        std::string full_path = m_config.storage_path + filename;
        if (std::remove(full_path.c_str()) == 0) {
            m_file_list.erase(it);
            std::cout << "删除文件: " << filename << std::endl;
            return true;
        }
    }
    
    return false;
}

void VideoRecordingManager::cleanupOldFiles() {
    while (getUsedStorage() > m_config.max_storage_size * m_config.storage_cleanup_threshold) {
        deleteOldestFile();
    }
}

void VideoRecordingManager::CleanupOldFiles(unsigned int keep_duration_seconds) {
    std::lock_guard<std::mutex> lock(m_file_list_mutex);
    
    auto now = std::chrono::system_clock::now();
    auto cutoff_time = now - std::chrono::seconds(keep_duration_seconds);
    
    // 删除超过保留时间的文件
    auto it = m_file_list.begin();
    while (it != m_file_list.end()) {
        if (it->end_time < cutoff_time && it->is_complete) {
            std::string full_path = m_config.storage_path + it->filename;
            if (std::remove(full_path.c_str()) == 0) {
                std::cout << "清理旧文件: " << it->filename << std::endl;
                it = m_file_list.erase(it);
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }
}

bool VideoRecordingManager::SplitCurrentFile() {
    std::lock_guard<std::mutex> lock(m_recording_mutex);
    
    if (!m_is_recording || !m_current_converter) {
        return false;
    }
    
    try {
        // 完成当前录像段
        if (!finishCurrentSegment()) {
            return false;
        }
        
        // 开始新的录像段
        if (!startNewSegment()) {
            return false;
        }
        
        std::cout << "手动分割录像文件成功" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cout << "分割录像文件失败: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> VideoRecordingManager::GetRecordingFiles() const {
    std::lock_guard<std::mutex> lock(m_file_list_mutex);
    
    std::vector<std::string> filenames;
    filenames.reserve(m_file_list.size());
    
    for (const auto& file_info : m_file_list) {
        filenames.push_back(file_info.filename);
    }
    
    return filenames;
}

bool VideoRecordingManager::initializeStorage() {
    // 创建存储目录
    if (!createDirectoryIfNotExists(m_config.storage_path)) {
        return false;
    }
    
    // 扫描现有文件
    try {
#if HAS_STD_FILESYSTEM
        for (const auto& entry : fs::directory_iterator(m_config.storage_path)) {
            if (fs::is_regular_file(entry) && entry.path().extension() == ".mp4") {
                VideoFileInfo info;
                info.filename = entry.path().filename().string();
                info.file_size = fs::file_size(entry);
                info.is_complete = true;
                info.checksum = calculateChecksum(entry.path().string());
                
                // 从文件名解析时间
                // 格式: 20250705085059-20250705085559.mp4
                std::string basename = info.filename.substr(0, info.filename.find_last_of('.'));
                size_t dash_pos = basename.find('-');
                if (dash_pos != std::string::npos) {
                    std::string start_str = basename.substr(0, dash_pos);
                    std::string end_str = basename.substr(dash_pos + 1);
                    info.start_time = stringToTimePoint(start_str);
                    info.end_time = stringToTimePoint(end_str);
                }
                
                m_file_list.push_back(info);
            }
        }
#else
        // 兼容性实现：使用传统方法遍历目录
#ifdef _WIN32
        WIN32_FIND_DATAA findData;
        std::string searchPath = m_config.storage_path + "*.mp4";
        HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    std::string filename = findData.cFileName;
                    if (filename.length() > 4 && filename.substr(filename.length() - 4) == ".mp4") {
                        VideoFileInfo info;
                        info.filename = filename;
                        LARGE_INTEGER fileSize;
                        fileSize.LowPart = findData.nFileSizeLow;
                        fileSize.HighPart = findData.nFileSizeHigh;
                        info.file_size = fileSize.QuadPart;
                        info.is_complete = true;
                        info.checksum = calculateChecksum(m_config.storage_path + filename);
                        
                        // 从文件名解析时间
                        std::string basename = info.filename.substr(0, info.filename.find_last_of('.'));
                        size_t dash_pos = basename.find('-');
                        if (dash_pos != std::string::npos) {
                            std::string start_str = basename.substr(0, dash_pos);
                            std::string end_str = basename.substr(dash_pos + 1);
                            info.start_time = stringToTimePoint(start_str);
                            info.end_time = stringToTimePoint(end_str);
                        }
                        
                        m_file_list.push_back(info);
                    }
                }
            } while (FindNextFileA(hFind, &findData));
            FindClose(hFind);
        }
#else
        DIR* dir = opendir(m_config.storage_path.c_str());
        if (dir) {
            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr) {
                std::string filename = entry->d_name;
                if (filename.length() > 4 && filename.substr(filename.length() - 4) == ".mp4") {
                    std::string full_path = m_config.storage_path + filename;
                    struct stat file_stat;
                    if (stat(full_path.c_str(), &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
                        VideoFileInfo info;
                        info.filename = filename;
                        info.file_size = file_stat.st_size;
                        info.is_complete = true;
                        info.checksum = calculateChecksum(full_path);
                        
                        // 从文件名解析时间
                        std::string basename = info.filename.substr(0, info.filename.find_last_of('.'));
                        size_t dash_pos = basename.find('-');
                        if (dash_pos != std::string::npos) {
                            std::string start_str = basename.substr(0, dash_pos);
                            std::string end_str = basename.substr(dash_pos + 1);
                            info.start_time = stringToTimePoint(start_str);
                            info.end_time = stringToTimePoint(end_str);
                        }
                        
                        m_file_list.push_back(info);
                    }
                }
            }
            closedir(dir);
        }
#endif
#endif
    } catch (const std::exception& e) {
        std::cout << "扫描存储目录失败: " << e.what() << std::endl;
        return false;
    }
    
    // 按时间排序
    std::sort(m_file_list.begin(), m_file_list.end(),
        [](const VideoFileInfo& a, const VideoFileInfo& b) {
            return a.start_time < b.start_time;
        });
    
    std::cout << "初始化存储完成，找到 " << m_file_list.size() << " 个录像文件" << std::endl;
    return true;
}

std::string VideoRecordingManager::generateFilename(
    const std::chrono::system_clock::time_point& start_time,
    const std::chrono::system_clock::time_point& end_time) {
    
    std::string start_str = timePointToString(start_time);
    std::string end_str = timePointToString(end_time);
    
    return start_str + "-" + end_str + ".mp4";
}

bool VideoRecordingManager::startNewSegment() {
    m_current_segment_start = std::chrono::system_clock::now();
    
    // 创建新的转换器
    m_current_converter = std::make_unique<MP4Converter>();
    
    AP4_Result result = m_current_converter->StartEncode(
        m_config.codec_type,
        m_config.video_width,
        m_config.video_height,
        m_config.video_fps
    );
    
    if (result != AP4_SUCCESS) {
        m_current_converter.reset();
        return false;
    }
    
    // 生成临时文件名
    m_current_filename = "temp_" + timePointToString(m_current_segment_start) + ".mp4";
    
    return true;
}

bool VideoRecordingManager::finishCurrentSegment() {
    if (!m_current_converter) {
        return false;
    }
    
    auto end_time = std::chrono::system_clock::now();
    
    // 生成最终文件名
    std::string final_filename = generateFilename(m_current_segment_start, end_time);
    std::string temp_path = m_config.storage_path + m_current_filename;
    std::string final_path = m_config.storage_path + final_filename;
    
    // 完成编码
    AP4_Result result = m_current_converter->EndEncode(temp_path);
    m_current_converter.reset();
    
    if (result != AP4_SUCCESS) {
        std::remove(temp_path.c_str());
        return false;
    }
    
    // 重命名文件
    if (std::rename(temp_path.c_str(), final_path.c_str()) != 0) {
        std::remove(temp_path.c_str());
        return false;
    }
    
    // 更新文件信息
    updateFileInfo(final_filename, true);
    
    std::cout << "录像段完成: " << final_filename << std::endl;
    return true;
}

void VideoRecordingManager::cleanupThread() {
    while (!m_should_stop) {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        
        if (getUsedStorage() > m_config.max_storage_size * m_config.storage_cleanup_threshold) {
            cleanupOldFiles();
        }
    }
}

bool VideoRecordingManager::checkStorageSpace() {
    size_t available = getAvailableStorage();
    
    if (available < m_config.estimated_file_size * 2) {
        cleanupOldFiles();
        available = getAvailableStorage();
    }
    
    return available >= m_config.estimated_file_size;
}

void VideoRecordingManager::deleteOldestFile() {
    std::lock_guard<std::mutex> lock(m_file_list_mutex);
    
    if (m_file_list.empty()) {
        return;
    }
    
    // 找到最早的完整文件
    auto oldest_it = std::min_element(m_file_list.begin(), m_file_list.end(),
        [](const VideoFileInfo& a, const VideoFileInfo& b) {
            if (a.is_complete != b.is_complete) {
                return a.is_complete && !b.is_complete; // 优先删除完整文件
            }
            return a.start_time < b.start_time;
        });
    
    if (oldest_it != m_file_list.end() && oldest_it->is_complete) {
        std::string full_path = m_config.storage_path + oldest_it->filename;
        if (std::remove(full_path.c_str()) == 0) {
            std::cout << "删除最早文件: " << oldest_it->filename << std::endl;
            m_file_list.erase(oldest_it);
        }
    }
}

uint32_t VideoRecordingManager::calculateChecksum(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        return 0;
    }
    
    uint32_t checksum = 0;
    char buffer[4096];
    
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        for (std::streamsize i = 0; i < file.gcount(); ++i) {
            checksum += static_cast<uint8_t>(buffer[i]);
        }
    }
    
    return checksum;
}

void VideoRecordingManager::updateFileInfo(const std::string& filename, bool is_complete) {
    std::lock_guard<std::mutex> lock(m_file_list_mutex);
    
    VideoFileInfo info;
    info.filename = filename;
    info.start_time = m_current_segment_start;
    info.end_time = std::chrono::system_clock::now();
    info.is_complete = is_complete;
    
    std::string full_path = m_config.storage_path + filename;
    try {
#if HAS_STD_FILESYSTEM
        info.file_size = fs::file_size(full_path);
#else
        // 兼容性实现：获取文件大小
#ifdef _WIN32
        WIN32_FILE_ATTRIBUTE_DATA fileInfo;
        if (GetFileAttributesExA(full_path.c_str(), GetFileExInfoStandard, &fileInfo)) {
            LARGE_INTEGER fileSize;
            fileSize.LowPart = fileInfo.nFileSizeLow;
            fileSize.HighPart = fileInfo.nFileSizeHigh;
            info.file_size = fileSize.QuadPart;
        } else {
            info.file_size = 0;
        }
#else
        struct stat file_stat;
        if (stat(full_path.c_str(), &file_stat) == 0) {
            info.file_size = file_stat.st_size;
        } else {
            info.file_size = 0;
        }
#endif
#endif
        info.checksum = calculateChecksum(full_path);
    } catch (const std::exception&) {
        info.file_size = 0;
        info.checksum = 0;
    }
    
    // 检查是否有时间重合的文件
    auto overlapping_it = std::find_if(m_file_list.begin(), m_file_list.end(),
        [&info](const VideoFileInfo& existing) {
            return (info.start_time < existing.end_time && info.end_time > existing.start_time);
        });
    
    if (overlapping_it != m_file_list.end()) {
        // 删除重合的旧文件
        std::string old_path = m_config.storage_path + overlapping_it->filename;
        std::remove(old_path.c_str());
        std::cout << "删除重合文件: " << overlapping_it->filename << std::endl;
        m_file_list.erase(overlapping_it);
    }
    
    m_file_list.push_back(info);
    
    // 保持按时间排序
    std::sort(m_file_list.begin(), m_file_list.end(),
        [](const VideoFileInfo& a, const VideoFileInfo& b) {
            return a.start_time < b.start_time;
        });
}

bool VideoRecordingManager::createDirectoryIfNotExists(const std::string& path) {
    try {
#if HAS_STD_FILESYSTEM
        if (!fs::exists(path)) {
            return fs::create_directories(path);
        }
        return true;
#else
        // 兼容性实现：创建目录
#ifdef _WIN32
        DWORD attrs = GetFileAttributesA(path.c_str());
        if (attrs == INVALID_FILE_ATTRIBUTES) {
            // 目录不存在，尝试创建
            std::string temp_path = path;
            if (temp_path.back() == '\\' || temp_path.back() == '/') {
                temp_path.pop_back();
            }
            
            // 递归创建父目录
            size_t pos = temp_path.find_last_of("\\/");
            if (pos != std::string::npos) {
                std::string parent = temp_path.substr(0, pos);
                if (!createDirectoryIfNotExists(parent)) {
                    return false;
                }
            }
            
            return CreateDirectoryA(temp_path.c_str(), NULL) != 0;
        }
        return (attrs & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else
        struct stat st;
        if (stat(path.c_str(), &st) != 0) {
            // 目录不存在，尝试创建
            std::string temp_path = path;
            if (temp_path.back() == '/') {
                temp_path.pop_back();
            }
            
            // 递归创建父目录
            size_t pos = temp_path.find_last_of('/');
            if (pos != std::string::npos) {
                std::string parent = temp_path.substr(0, pos);
                if (!createDirectoryIfNotExists(parent)) {
                    return false;
                }
            }
            
            return mkdir(temp_path.c_str(), 0755) == 0;
        }
        return S_ISDIR(st.st_mode);
#endif
#endif
    } catch (const std::exception& e) {
        std::cout << "创建目录失败: " << e.what() << std::endl;
        return false;
    }
}

std::string VideoRecordingManager::timePointToString(const std::chrono::system_clock::time_point& tp) {
    auto time_t = std::chrono::system_clock::to_time_t(tp);
    std::tm* tm = std::localtime(&time_t);
    
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y%m%d%H%M%S");
    return oss.str();
}

std::chrono::system_clock::time_point VideoRecordingManager::stringToTimePoint(const std::string& str) {
    std::tm tm = {};
    std::istringstream ss(str);
    ss >> std::get_time(&tm, "%Y%m%d%H%M%S");
    
    auto time_t = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(time_t);
}

void VideoRecordingManager::handleRecordingError(const std::string& error_msg) {
    std::cout << "录像错误: " << error_msg << std::endl;
    
    // 尝试恢复
    if (!recoverFromError()) {
        std::cout << "无法恢复录像，停止录像" << std::endl;
        stopRecording();
    }
}

bool VideoRecordingManager::recoverFromError() {
    // 重置当前转换器
    m_current_converter.reset();
    
    // 尝试重新开始录像段
    return startNewSegment();
}

// GlobalRecordingManager 实现
std::unique_ptr<VideoRecordingManager> GlobalRecordingManager::s_instance;
std::mutex GlobalRecordingManager::s_mutex;

VideoRecordingManager& GlobalRecordingManager::getInstance() {
    std::lock_guard<std::mutex> lock(s_mutex);
    if (!s_instance) {
        s_instance = std::make_unique<VideoRecordingManager>();
    }
    return *s_instance;
}

void GlobalRecordingManager::cleanup() {
    std::lock_guard<std::mutex> lock(s_mutex);
    s_instance.reset();
}