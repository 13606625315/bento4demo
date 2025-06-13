# FMP4Converter 使用说明

## 概述

本项目现在支持两种MP4格式的输出：
- **MP4Converter**: 生成标准MP4文件
- **FMP4Converter**: 生成分片MP4（fMP4）文件

## 新增功能

### FMP4Converter类

`FMP4Converter`类基于现有的`MP4Converter`实现，专门用于生成符合DASH/HLS标准的分片MP4文件。

#### 主要特性：
- 支持H.264和H.265编码
- 生成符合ISO BMFF标准的fMP4文件
- 包含初始化段（initialization segment）和媒体段（media segment）
- 适用于流媒体应用

#### 文件结构：
- `FMP4Converter.h` - 头文件
- `FMP4Converter.cpp` - 实现文件

## 编译

确保项目包含了新的源文件：

```bash
cd build
cmake ..
make
```

## 使用方法

### 命令行参数

新增了 `--fmp4` 参数来启用fMP4输出：

```bash
# 生成标准MP4文件
./h264_h265_to_fmp4_new input.dav output.mp4 h264

# 生成fMP4文件
./h264_h265_to_fmp4_new --fmp4 input.dav output.mp4 h264

# 结合其他参数使用
./h264_h265_to_fmp4_new --step-by-step --fmp4 --width 1280 --height 720 input.dav output.mp4 h265
```

### 参数说明

- `--fmp4`: 启用fMP4格式输出
- `--step-by-step`: 使用逐步编码接口
- `--width <width>`: 视频宽度（默认：1920）
- `--height <height>`: 视频高度（默认：1080）

## 代码示例

### 使用FMP4Converter类

```cpp
#include "FMP4Converter.h"

FMP4Converter converter;

// 1. 开始编码
AP4_Result result = converter.StartEncode("h264", 1920, 1080, 25.0);
if (AP4_FAILED(result)) {
    // 处理错误
}

// 2. 添加视频帧
AP4_UI64 dts = 0, cts = 0;
result = converter.AddSample(frame_data, frame_size, is_keyframe, dts, cts);
if (AP4_FAILED(result)) {
    // 处理错误
}

// 3. 结束编码并输出文件
result = converter.EndEncode("output.mp4");
if (AP4_FAILED(result)) {
    // 处理错误
}
```

## fMP4 vs 标准MP4

### 标准MP4特点：
- 单一文件结构
- 所有元数据在文件开头或结尾
- 适合本地播放和下载

### fMP4特点：
- 分片结构，包含初始化段和媒体段
- 每个片段可以独立处理
- 适合流媒体传输（DASH/HLS）
- 支持自适应比特率流

## 测试

运行测试脚本：

```bash
# Windows
test_fmp4.bat

# Linux/Mac
# 可以参考bat文件内容创建对应的shell脚本
```

## 注意事项

1. fMP4文件主要用于流媒体场景
2. 某些播放器可能对fMP4支持有限
3. 文件大小可能略大于标准MP4
4. 确保输入的视频流包含完整的参数集（SPS/PPS/VPS）

## 故障排除

### 常见问题：

1. **编译错误**：确保所有新文件都已添加到CMakeLists.txt中
2. **运行时错误**：检查输入文件格式和参数设置
3. **输出文件无法播放**：验证编码参数和参数集是否正确

### 调试模式：

编译时启用调试模式可以获得更多信息：

```cpp
#define mp4_debug
```

## 技术细节

### fMP4文件结构：

1. **ftyp** - 文件类型标识
2. **moov** - 包含mvex的movie header
3. **moof** - movie fragment header
4. **mdat** - 媒体数据

### 关键差异：

- 使用`AP4_FTYP_BRAND_DASH`作为兼容品牌
- 添加`mvex`和`trex` atoms
- 使用`moof`和`mdat`结构存储媒体数据