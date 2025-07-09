# 清扫机器人录像存储管理系统

## 系统概述

本系统是基于Bento4库开发的智能录像存储管理系统，专为清扫机器人设计，实现了自动录像分割、存储管理、容量控制等功能。

## 核心功能

### 1. 自动录像控制
- **开始清扫时自动开始录像**
- **暂停/结束清扫时保存录像文件**
- **支持录像的暂停和恢复**

### 2. 智能文件分割
- **超过5分钟自动分割录像**
- **文件按时间命名**: `20250705085059-20250705085559.mp4`
- **不足5分钟直接保存完整文件**

### 3. 存储容量管理
- **固定6GB存储空间分配**
- **预分配内存池避免存储碎片**
- **最大支持1228个录像文件**（按5MB/文件计算）
- **自动删除最早录像文件**

### 4. 文件冲突处理
- **检测时间重合的录像文件**
- **自动删除旧的重合文件**
- **保证录像时间线的连续性**

### 5. 异常处理机制
- **关机时自动保存当前录像**
- **支持录像过程中的错误恢复**
- **硬复位时丢失未保存录像（符合设计要求）**

## 系统架构

### 核心类设计

```cpp
// 录像管理器主类
class VideoRecordingManager {
    // 录像控制
    bool startRecording();
    bool pauseRecording();
    bool stopRecording();
    
    // 帧数据处理
    bool addVideoFrame(const unsigned char* data, size_t size, bool is_keyframe);
    
    // 存储管理
    void cleanupOldFiles();
    size_t getUsedStorage();
};

// 内存池管理
class MemoryPool {
    void* allocate();
    void deallocate(void* ptr);
};

// 文件信息结构
struct VideoFileInfo {
    std::string filename;
    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point end_time;
    size_t file_size;
    bool is_complete;
    uint32_t checksum;
};
```

### 配置参数

```cpp
struct RecordingConfig {
    size_t max_storage_size = 6LL * 1024 * 1024 * 1024; // 6GB
    int segment_duration_minutes = 5;                    // 5分钟分割
    size_t estimated_file_size = 5 * 1024 * 1024;      // 预估5MB
    std::string storage_path = "./recordings/";
    std::string codec_type = "h264";
    int video_width = 1920;
    int video_height = 1080;
    double video_fps = 25.0;
};
```

## 使用方法

### 1. 编译系统

```bash
# 创建构建目录
mkdir build
cd build

# 配置和编译
cmake ..
make

# 或者在Windows上使用Visual Studio
cmake .. -G "Visual Studio 16 2019"
msbuild Bento4Demo.sln
```

### 2. 基本使用示例

```cpp
#include "VideoRecordingManager.h"

int main() {
    // 获取录像管理器实例
    auto& recorder = GlobalRecordingManager::getInstance();
    
    // 开始录像（清扫开始）
    recorder.startRecording();
    
    // 添加视频帧
    bool is_keyframe = true;
    recorder.addVideoFrame(frame_data, frame_size, is_keyframe);
    
    // 暂停录像（清扫暂停）
    recorder.pauseRecording();
    
    // 恢复录像（清扫恢复）
    recorder.startRecording();
    
    // 停止录像（清扫结束）
    recorder.stopRecording();
    
    return 0;
}
```

### 3. 运行演示程序

```bash
# 运行录像系统演示
./recording_demo

# 演示程序提供交互式控制：
# 1 - 开始清扫
# 2 - 暂停清扫
# 3 - 恢复清扫
# 4 - 停止清扫
# 5 - 显示存储状态
# 0 - 退出
```

## 文件结构

```
Bento4demo/
├── VideoRecordingManager.h      # 录像管理器头文件
├── VideoRecordingManager.cpp    # 录像管理器实现
├── recording_demo.cpp           # 演示程序
├── MP4Converter.h               # MP4转换器（基础组件）
├── MP4Converter.cpp
├── CMakeLists.txt               # 构建配置
├── README_Recording_System.md   # 本文档
└── recordings/                  # 录像文件存储目录（自动创建）
    ├── 20250705085059-20250705090059.mp4
    ├── 20250705090059-20250705091559.mp4
    └── ...
```

## 技术特性

### 1. 线程安全
- 使用互斥锁保护共享资源
- 支持多线程环境下的并发访问
- 异步清理线程定期维护存储空间

### 2. 内存管理
- 预分配内存池避免频繁内存分配
- 环形缓冲区设计减少内存碎片
- 智能指针管理资源生命周期

### 3. 错误处理
- 完善的异常处理机制
- 自动错误恢复功能
- 详细的日志输出

### 4. 性能优化
- 异步I/O操作
- 批量文件操作
- 智能缓存策略

## 存储策略详解

### 1. 容量管理算法

```cpp
void cleanupOldFiles() {
    // 当使用空间超过90%时开始清理
    while (getUsedStorage() > max_storage_size * 0.9) {
        deleteOldestFile();
    }
}
```

### 2. 文件命名规则

- **格式**: `YYYYMMDDHHMMSS-YYYYMMDDHHMMSS.mp4`
- **示例**: `20250705085059-20250705085559.mp4`
- **含义**: 从2025年7月5日8:50:59到8:55:59的录像

### 3. 时间重合处理

```cpp
// 检测时间重合并删除旧文件
if (new_start_time < existing_end_time && new_end_time > existing_start_time) {
    deleteFile(existing_filename);
}
```

## 配置优化建议

### 1. 根据实际需求调整参数

```cpp
RecordingConfig config;
config.max_storage_size = 6LL * 1024 * 1024 * 1024;  // 根据硬件调整
config.segment_duration_minutes = 5;                   // 根据使用场景调整
config.estimated_file_size = 5 * 1024 * 1024;        // 根据视频质量调整
```

### 2. 视频编码参数优化

```cpp
config.video_width = 1920;   // 根据需要的清晰度调整
config.video_height = 1080;
config.video_fps = 25.0;     // 根据流畅度需求调整
```

### 3. 存储路径配置

```cpp
config.storage_path = "/var/recordings/";  // Linux
config.storage_path = "C:\\Recordings\\";    // Windows
```

## 故障排除

### 1. 常见问题

**问题**: 录像文件无法创建
**解决**: 检查存储路径权限和磁盘空间

**问题**: 内存不足
**解决**: 调整内存池大小或减少并发录像数量

**问题**: 文件损坏
**解决**: 检查磁盘健康状态，启用文件校验

### 2. 调试模式

```cpp
// 启用详细日志
#define ENABLE_DEBUG_LOG

// 减小文件大小用于测试
config.segment_duration_minutes = 1;  // 1分钟分割
config.max_storage_size = 100 * 1024 * 1024;  // 100MB
```

## 扩展功能

### 1. 支持的扩展
- 音频录制支持
- 多路视频流录制
- 云存储同步
- 实时流媒体推送
- 智能事件检测

### 2. API扩展示例

```cpp
// 添加音频支持
bool addAudioFrame(const unsigned char* data, size_t size);

// 添加元数据
bool addMetadata(const std::string& key, const std::string& value);

// 设置录像质量
void setRecordingQuality(RecordingQuality quality);
```

## 许可证

本项目基于Bento4库开发，遵循相应的开源许可证。

## 联系信息

如有问题或建议，请通过以下方式联系：
- 项目仓库: [GitHub链接]
- 技术支持: [邮箱地址]
- 文档更新: [文档链接]