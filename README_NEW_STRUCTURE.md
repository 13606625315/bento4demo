# H264H265ToFMP4Converter 重构说明

## 概述

本项目已经重构，将原来的单文件 `h264_h265_to_fmp4.cpp` 拆分为多个文件，实现了更好的代码组织和接口设计。

## 文件结构

### 核心文件

- **`H264H265ToFMP4Converter.h`** - 转换器类的头文件
- **`H264H265ToFMP4Converter.cpp`** - 转换器类的实现文件
- **`main.cpp`** - 主程序，负责命令行解析和调用转换器
- **`example_usage.cpp`** - 使用示例代码

### 原有文件

- **`h264_h265_to_fmp4.cpp`** - 原始的单文件实现（保留用于兼容性）
- **`simple_converter.cpp`** - 简单转换器（未修改）

## 新的接口设计

### 1. 分步骤编码接口

```cpp
H264H265ToFMP4Converter converter;

// 步骤1: 开始编码
AP4_Result result = converter.StartEncode("h264", 1920, 1080);

// 步骤2: 添加样本
for (each_sample) {
    converter.AddSample(data, size, is_keyframe, dts, cts);
}

// 步骤3: 结束编码
converter.EndEncode("output.mp4");
```

### 2. 流式编码接口

```cpp
H264H265ToFMP4Converter converter;
AP4_ByteStream* output_stream = /* 创建输出流 */;

// 开始编码（直接输出到流）
converter.StartEncode("h265", output_stream, 1280, 720);

// 添加样本
converter.AddSample(data, size, is_keyframe, dts, cts);

// 结束编码
converter.EndEncode();
```

### 3. 兼容性接口

```cpp
// 保留原有的一步转换接口
H264H265ToFMP4Converter converter;
converter.ConvertToFMP4("input.264", "output.mp4", "h264");
```

## 主要改进

### 1. 职责分离

- **main.cpp**: 专门负责命令行解析、文件读取、NAL单元分割
- **H264H265ToFMP4Converter**: 专门负责MP4编码逻辑

### 2. 灵活的接口

- **StartEncode**: 初始化编码器，设置编码参数
- **AddSample**: 逐个添加视频帧数据
- **EndEncode**: 完成编码并输出文件

### 3. 状态管理

- 内部维护编码状态，防止错误的调用顺序
- 提供 `Reset()` 方法重置编码器状态
- 自动资源管理，防止内存泄漏

### 4. 错误处理

- 完善的错误检查和状态验证
- 清晰的错误返回码
- 异常安全的资源管理

## 编译

### 新版本

```bash
make h264_h265_to_fmp4_new
```

### 原版本（兼容性）

```bash
make h264_h265_to_fmp4_old
```

## 使用方法

### 命令行工具

```bash
# 基本用法
./h264_h265_to_fmp4_new input.264 output.mp4 h264

# 分步骤模式
./h264_h265_to_fmp4_new --step-by-step input.265 output.mp4 h265

# 自定义分辨率
./h264_h265_to_fmp4_new --width 1280 --height 720 input.264 output.mp4 h264

# 组合选项
./h264_h265_to_fmp4_new --step-by-step --width 1920 --height 1080 input.265 output.mp4 h265
```

### 编程接口

参考 `example_usage.cpp` 文件中的详细示例。

## API 参考

### 构造函数和析构函数

```cpp
H264H265ToFMP4Converter();  // 构造函数
~H264H265ToFMP4Converter(); // 析构函数
```

### 编码控制方法

```cpp
// 开始编码（输出到文件）
AP4_Result StartEncode(const std::string& codec_type, int width = 1920, int height = 1080);

// 开始编码（输出到流）
AP4_Result StartEncode(const std::string& codec_type, AP4_ByteStream* output_stream, 
                      int width = 1920, int height = 1080);

// 添加样本
AP4_Result AddSample(const unsigned char* data, size_t size, bool is_keyframe = false, 
                    AP4_UI64 dts = 0, AP4_UI64 cts = 0);

// 结束编码（输出到文件）
AP4_Result EndEncode(const std::string& output_file);

// 结束编码（输出到流）
AP4_Result EndEncode();

// 重置编码器
void Reset();
```

### 兼容性方法

```cpp
// 一步转换（保持向后兼容）
AP4_Result ConvertToFMP4(const std::string& input_file, 
                         const std::string& output_file,
                         const std::string& codec_type);
```

## 支持的编码类型

- `"h264"` 或 `"avc"` - H.264/AVC 编码
- `"h265"` 或 `"hevc"` - H.265/HEVC 编码

## 错误码

- `AP4_SUCCESS` - 操作成功
- `AP4_ERROR_INVALID_STATE` - 无效状态（如在未开始编码时添加样本）
- `AP4_ERROR_INVALID_PARAMETERS` - 无效参数（如不支持的编码类型）
- `AP4_ERROR_INTERNAL` - 内部错误
- 其他 Bento4 库定义的错误码

## 注意事项

1. **线程安全**: 当前实现不是线程安全的，如需在多线程环境使用，请添加适当的同步机制。

2. **内存管理**: 编码器会自动管理内部资源，但用户需要确保传入的数据指针在调用期间有效。

3. **文件格式**: 输入文件应为原始的H.264或H.265码流文件（Annex B格式）。

4. **性能**: 分步骤接口提供了更好的控制，但可能比一步转换接口稍慢。

5. **兼容性**: 保留了原有的 `ConvertToFMP4` 方法以确保向后兼容。

## 示例代码

完整的使用示例请参考 `example_usage.cpp` 文件，其中包含：

- 分步骤编码示例
- 流式编码示例
- 一步转换示例
- 错误处理示例

## 构建目标

- `h264_h265_to_fmp4_new` - 新的重构版本
- `h264_h265_to_fmp4_old` - 原始版本（兼容性）
- `simple_converter` - 简单转换器（未修改）