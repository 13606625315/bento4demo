# 清扫机器人录像系统 C接口文档

## 概述

本文档介绍清扫机器人录像系统的C接口，该接口基于现有的C++录像管理系统，提供了简洁易用的C语言API，支持与imouos视频流订阅的集成。

## 核心特性

### 🎯 主要功能
- **录像控制**: 开始、暂停、恢复、停止录像
- **视频流集成**: 自动订阅imouos视频流
- **智能分割**: 按时间自动分割录像文件
- **存储管理**: 自动清理旧文件，防止存储溢出
- **状态监控**: 实时获取录像状态和统计信息
- **错误处理**: 完善的错误回调机制
- **线程安全**: 支持多线程环境

### 🔧 技术特点
- **C语言接口**: 兼容性强，易于集成
- **零拷贝设计**: 高效的内存管理
- **异步处理**: 不阻塞主线程
- **跨平台支持**: Windows/Linux兼容

## 快速开始

### 1. 编译构建

#### Windows环境
```bash
# 使用提供的批处理脚本
build_c_api.bat

# 或手动构建
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

#### Linux环境
```bash
mkdir build && cd build
cmake ..
make -j4
```

### 2. 基本使用示例

```c
#include "recording_c_api.h"
#include <stdio.h>

void error_callback(RecordingErrorCode error_code, const char* error_msg) {
    printf("Error: %d - %s\n", error_code, error_msg);
}

int main() {
    // 1. 配置录像参数
    RecordingConfig config = {
        .storage_path = "./recordings",
        .segment_duration_sec = 300,     // 5分钟分段
        .max_storage_gb = 10,            // 最大10GB
        .memory_pool_size_mb = 100,      // 100MB内存池
        .min_file_duration_sec = 60,     // 最小1分钟
        .enable_audio = 0,               // 暂不启用音频
        .video_stream_type = 0           // 主码流
    };
    
    // 2. 设置错误回调
    recording_set_error_callback(error_callback);
    
    // 3. 初始化录像系统
    RecordingErrorCode ret = recording_init(&config);
    if (ret != RECORDING_SUCCESS) {
        printf("Failed to initialize: %d\n", ret);
        return -1;
    }
    
    // 4. 开始录像
    ret = recording_start(0);  // 通道0
    if (ret != RECORDING_SUCCESS) {
        printf("Failed to start recording: %d\n", ret);
        recording_deinit();
        return -1;
    }
    
    printf("Recording started. Press Enter to stop...\n");
    getchar();
    
    // 5. 停止录像
    recording_stop();
    
    // 6. 清理资源
    recording_deinit();
    
    return 0;
}
```

## API参考

### 数据结构

#### RecordingConfig
录像配置结构体
```c
typedef struct {
    const char* storage_path;           // 存储路径
    unsigned int segment_duration_sec;  // 分段时长(秒)
    unsigned int max_storage_gb;        // 最大存储空间(GB)
    unsigned int memory_pool_size_mb;   // 内存池大小(MB)
    unsigned int min_file_duration_sec; // 最小文件时长(秒)
    int enable_audio;                   // 是否启用音频
    int video_stream_type;              // 视频流类型
} RecordingConfig;
```

#### RecordingStatus
录像状态信息
```c
typedef struct {
    RecordingState state;               // 当前状态
    unsigned long long total_size_bytes; // 总录像大小
    unsigned int file_count;            // 录像文件数量
    unsigned long long available_space_bytes; // 可用空间
    unsigned int current_segment_duration_sec; // 当前段时长
    const char* current_filename;       // 当前文件名
    const char* last_error;             // 最后错误信息
} RecordingStatus;
```

### 核心函数

#### 系统管理

```c
// 初始化录像系统
RecordingErrorCode recording_init(const RecordingConfig* config);

// 反初始化录像系统
RecordingErrorCode recording_deinit(void);

// 获取版本信息
const char* recording_get_version(void);
```

#### 录像控制

```c
// 开始录像
RecordingErrorCode recording_start(int channel_id);

// 暂停录像
RecordingErrorCode recording_pause(void);

// 恢复录像
RecordingErrorCode recording_resume(void);

// 停止录像
RecordingErrorCode recording_stop(void);
```

#### 状态查询

```c
// 获取录像状态
RecordingErrorCode recording_get_status(RecordingStatus* status);

// 获取录像文件列表
RecordingErrorCode recording_get_file_list(char*** file_list, unsigned int* count);

// 释放文件列表内存
void recording_free_file_list(char** file_list, unsigned int count);
```

#### 高级功能

```c
// 更新配置
RecordingErrorCode recording_update_config(const RecordingConfig* config);

// 手动分割文件
RecordingErrorCode recording_split_file(void);

// 清理旧文件
RecordingErrorCode recording_cleanup_old_files(unsigned int keep_duration_hours);

// 设置错误回调
void recording_set_error_callback(RecordingErrorCallback callback);
```

### 错误码

```c
typedef enum {
    RECORDING_SUCCESS = 0,              // 成功
    RECORDING_ERROR_INVALID_PARAM = -1, // 无效参数
    RECORDING_ERROR_NOT_INITIALIZED = -2, // 未初始化
    RECORDING_ERROR_ALREADY_STARTED = -3, // 已经开始
    RECORDING_ERROR_NOT_STARTED = -4,    // 未开始
    RECORDING_ERROR_STORAGE_FULL = -5,   // 存储满
    RECORDING_ERROR_FILE_OPERATION = -6, // 文件操作失败
    RECORDING_ERROR_MEMORY_ALLOC = -7,   // 内存分配失败
    RECORDING_ERROR_INTERNAL = -8        // 内部错误
} RecordingErrorCode;
```

## 集成指南

### 与imouos系统集成

录像系统自动集成imouos视频流订阅：

1. **自动订阅**: 调用`recording_start()`时自动订阅视频流
2. **数据处理**: 内部处理DHAV格式数据
3. **编码转换**: 自动转换为MP4格式
4. **流类型**: 支持主码流、辅码流等多种类型

### 视频流处理流程

```
imouos视频流 → DHAV解析 → H.264/H.265提取 → MP4编码 → 文件存储
```

### 内存管理

- **内存池**: 预分配固定大小内存池
- **零拷贝**: 最小化内存拷贝操作
- **自动清理**: 自动释放过期内存块
- **溢出保护**: 防止内存泄漏

## 配置优化

### 存储配置

```c
RecordingConfig config = {
    .storage_path = "/data/recordings",
    .segment_duration_sec = 600,        // 10分钟分段(适合长时间录像)
    .max_storage_gb = 50,               // 根据设备存储容量调整
    .memory_pool_size_mb = 200,         // 高分辨率时增加内存池
    .min_file_duration_sec = 120,       // 避免产生过小文件
    .enable_audio = 1,                  // 根据需求启用音频
    .video_stream_type = videoEncTypeMain // 主码流
};
```

### 性能优化建议

1. **内存池大小**: 根据视频分辨率和码率调整
   - 1080P: 100-200MB
   - 4K: 300-500MB

2. **分段时长**: 平衡文件管理和存储效率
   - 短时录像: 2-5分钟
   - 长时录像: 10-30分钟

3. **存储路径**: 使用高速存储设备
   - SSD优于HDD
   - 避免网络存储延迟

## 故障排除

### 常见问题

#### 1. 初始化失败
```c
if (recording_init(&config) != RECORDING_SUCCESS) {
    // 检查存储路径是否存在
    // 检查权限是否足够
    // 检查磁盘空间是否充足
}
```

#### 2. 录像开始失败
```c
if (recording_start(0) != RECORDING_SUCCESS) {
    // 检查是否已经初始化
    // 检查视频流是否可用
    // 检查内存是否充足
}
```

#### 3. 文件分割异常
- 检查最小文件时长设置
- 确认存储空间充足
- 验证文件权限

### 调试技巧

1. **启用错误回调**
```c
void debug_callback(RecordingErrorCode code, const char* msg) {
    printf("[DEBUG] Error %d: %s\n", code, msg);
}
recording_set_error_callback(debug_callback);
```

2. **定期检查状态**
```c
RecordingStatus status;
if (recording_get_status(&status) == RECORDING_SUCCESS) {
    printf("State: %d, Files: %u, Size: %llu\n", 
           status.state, status.file_count, status.total_size_bytes);
}
```

## 示例程序

### 运行演示程序

```bash
# 构建并运行
build_c_api.bat

# 或直接运行
./build/Release/recording_c_demo.exe
```

### 演示程序功能

- **交互式控制**: 键盘命令控制录像
- **状态显示**: 实时显示录像状态
- **文件管理**: 查看和清理录像文件
- **错误处理**: 演示错误处理机制

## 许可证

本项目采用MIT许可证，详见LICENSE文件。

## 技术支持

如有问题或建议，请联系开发团队。

---

**版本**: 1.0.0  
**更新日期**: 2024-01-01  
**兼容性**: Windows 10+, Linux (Ubuntu 18.04+)