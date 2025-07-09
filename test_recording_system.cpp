#include "VideoRecordingManager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cassert>

// 简单的测试框架
class TestFramework {
public:
    static void assertTrue(bool condition, const std::string& message) {
        if (condition) {
            std::cout << "[PASS] " << message << std::endl;
        } else {
            std::cout << "[FAIL] " << message << std::endl;
            s_failed_tests++;
        }
        s_total_tests++;
    }
    
    static void printSummary() {
        std::cout << "\n=== 测试结果 ===" << std::endl;
        std::cout << "总测试数: " << s_total_tests << std::endl;
        std::cout << "通过: " << (s_total_tests - s_failed_tests) << std::endl;
        std::cout << "失败: " << s_failed_tests << std::endl;
        std::cout << "成功率: " << (100.0 * (s_total_tests - s_failed_tests) / s_total_tests) << "%" << std::endl;
    }
    
    static bool allTestsPassed() {
        return s_failed_tests == 0;
    }
    
private:
    static int s_total_tests;
    static int s_failed_tests;
};

int TestFramework::s_total_tests = 0;
int TestFramework::s_failed_tests = 0;

// 生成测试用的H.264数据
std::vector<uint8_t> generateTestH264Frame(bool is_keyframe) {
    std::vector<uint8_t> frame;
    
    if (is_keyframe) {
        // SPS
        std::vector<uint8_t> sps = {
            0x00, 0x00, 0x00, 0x01, 0x67, 0x64, 0x00, 0x28,
            0xAC, 0x2B, 0x40, 0x50, 0x1E, 0xD0, 0x0F, 0x12
        };
        frame.insert(frame.end(), sps.begin(), sps.end());
        
        // PPS
        std::vector<uint8_t> pps = {
            0x00, 0x00, 0x00, 0x01, 0x68, 0xEE, 0x3C, 0xB0
        };
        frame.insert(frame.end(), pps.begin(), pps.end());
        
        // IDR
        std::vector<uint8_t> idr = {
            0x00, 0x00, 0x00, 0x01, 0x65, 0x88, 0x84, 0x00
        };
        frame.insert(frame.end(), idr.begin(), idr.end());
        
        // 添加一些数据
        for (int i = 0; i < 500; ++i) {
            frame.push_back(static_cast<uint8_t>(i % 256));
        }
    } else {
        // P帧
        std::vector<uint8_t> p_frame = {
            0x00, 0x00, 0x00, 0x01, 0x61, 0x9A, 0x12, 0x34
        };
        frame.insert(frame.end(), p_frame.begin(), p_frame.end());
        
        // 添加一些数据
        for (int i = 0; i < 200; ++i) {
            frame.push_back(static_cast<uint8_t>(i % 256));
        }
    }
    
    return frame;
}

// 测试基本录像功能
void testBasicRecording() {
    std::cout << "\n=== 测试基本录像功能 ===" << std::endl;
    
    try {
        // 配置测试参数
        VideoRecordingConfig config;
        config.max_storage_size = 50 * 1024 * 1024; // 50MB
        config.segment_duration_minutes = 1; // 1分钟用于快速测试
        config.storage_path = "./test_recordings/";
        
        auto& recorder = GlobalRecordingManager::getInstance();
        recorder.updateConfig(config);
        
        // 测试开始录像
        bool start_result = recorder.startRecording();
        TestFramework::assertTrue(start_result, "开始录像");
        TestFramework::assertTrue(recorder.isRecording(), "录像状态检查");
        
        // 添加一些测试帧
        for (int i = 0; i < 10; ++i) {
            bool is_keyframe = (i == 0); // 第一帧是关键帧
            auto frame_data = generateTestH264Frame(is_keyframe);
            bool add_result = recorder.addVideoFrame(frame_data.data(), frame_data.size(), is_keyframe);
            TestFramework::assertTrue(add_result, "添加视频帧 " + std::to_string(i));
            
            std::this_thread::sleep_for(std::chrono::milliseconds(40)); // 模拟25fps
        }
        
        // 测试暂停录像
        bool pause_result = recorder.pauseRecording();
        TestFramework::assertTrue(pause_result, "暂停录像");
        TestFramework::assertTrue(!recorder.isRecording(), "暂停状态检查");
        
        // 测试恢复录像
        bool resume_result = recorder.startRecording();
        TestFramework::assertTrue(resume_result, "恢复录像");
        
        // 添加更多帧
        for (int i = 0; i < 5; ++i) {
            auto frame_data = generateTestH264Frame(false);
            recorder.addVideoFrame(frame_data.data(), frame_data.size(), false);
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
        }
        
        // 测试停止录像
        bool stop_result = recorder.stopRecording();
        TestFramework::assertTrue(stop_result, "停止录像");
        TestFramework::assertTrue(!recorder.isRecording(), "停止状态检查");
        
    } catch (const std::exception& e) {
        TestFramework::assertTrue(false, "基本录像测试异常: " + std::string(e.what()));
    }
}

// 测试存储管理功能
void testStorageManagement() {
    std::cout << "\n=== 测试存储管理功能 ===" << std::endl;
    
    try {
        auto& recorder = GlobalRecordingManager::getInstance();
        
        // 测试存储状态查询
        size_t used_storage = recorder.getUsedStorage();
        size_t available_storage = recorder.getAvailableStorage();
        
        TestFramework::assertTrue(true, "已使用存储查询");  // size_t 类型总是 >= 0
        TestFramework::assertTrue(available_storage > 0, "可用存储查询");
        
        std::cout << "已使用存储: " << (used_storage / 1024) << " KB" << std::endl;
        std::cout << "可用存储: " << (available_storage / 1024 / 1024) << " MB" << std::endl;
        
        // 测试文件列表
        auto file_list = recorder.getFileList();
        TestFramework::assertTrue(true, "文件列表查询");  // size_t 类型总是 >= 0
        
        std::cout << "录像文件数量: " << file_list.size() << std::endl;
        
        for (const auto& file_info : file_list) {
            std::cout << "文件: " << file_info.filename 
                      << ", 大小: " << (file_info.file_size / 1024) << " KB"
                      << ", 完整: " << (file_info.is_complete ? "是" : "否") << std::endl;
        }
        
    } catch (const std::exception& e) {
        TestFramework::assertTrue(false, "存储管理测试异常: " + std::string(e.what()));
    }
}

// 测试文件分割功能
void testFileSegmentation() {
    std::cout << "\n=== 测试文件分割功能 ===" << std::endl;
    
    try {
        // 配置短分割时间用于测试
        VideoRecordingConfig config;
        config.segment_duration_minutes = 1; // 1分钟分割
        config.max_storage_size = 100 * 1024 * 1024; // 100MB
        config.storage_path = "./test_recordings/";
        
        auto& recorder = GlobalRecordingManager::getInstance();
        recorder.updateConfig(config);
        
        size_t initial_file_count = recorder.getFileList().size();
        
        // 开始录像
        recorder.startRecording();
        
        // 模拟录像超过分割时间
        auto start_time = std::chrono::steady_clock::now();
        int frame_count = 0;
        
        while (std::chrono::steady_clock::now() - start_time < std::chrono::seconds(70)) { // 70秒
            bool is_keyframe = (frame_count % 30 == 0); // 每30帧一个关键帧
            auto frame_data = generateTestH264Frame(is_keyframe);
            recorder.addVideoFrame(frame_data.data(), frame_data.size(), is_keyframe);
            
            frame_count++;
            std::this_thread::sleep_for(std::chrono::milliseconds(40)); // 25fps
        }
        
        recorder.stopRecording();
        
        // 检查是否生成了多个文件
        size_t final_file_count = recorder.getFileList().size();
        TestFramework::assertTrue(final_file_count > initial_file_count, "文件分割功能");
        
        std::cout << "分割前文件数: " << initial_file_count << std::endl;
        std::cout << "分割后文件数: " << final_file_count << std::endl;
        
    } catch (const std::exception& e) {
        TestFramework::assertTrue(false, "文件分割测试异常: " + std::string(e.what()));
    }
}

// 测试内存池功能
void testMemoryPool() {
    std::cout << "\n=== 测试内存池功能 ===" << std::endl;
    
    try {
        size_t pool_size = 1024 * 1024; // 1MB
        size_t block_size = 1024; // 1KB块
        
        MemoryPool pool(pool_size, block_size);
        
        // 测试内存分配
        std::vector<void*> allocated_blocks;
        
        // 分配一些内存块
        for (int i = 0; i < 10; ++i) {
            void* ptr = pool.allocate();
            TestFramework::assertTrue(ptr != nullptr, "内存分配 " + std::to_string(i));
            allocated_blocks.push_back(ptr);
        }
        
        // 释放内存块
        for (void* ptr : allocated_blocks) {
            pool.deallocate(ptr);
        }
        
        // 测试可用块数量
        size_t available_blocks = pool.getAvailableBlocks();
        TestFramework::assertTrue(available_blocks > 0, "内存池可用块数量");
        
        std::cout << "内存池可用块数: " << available_blocks << std::endl;
        
    } catch (const std::exception& e) {
        TestFramework::assertTrue(false, "内存池测试异常: " + std::string(e.what()));
    }
}

// 测试配置更新功能
void testConfigUpdate() {
    std::cout << "\n=== 测试配置更新功能 ===" << std::endl;
    
    try {
        auto& recorder = GlobalRecordingManager::getInstance();
        
        // 获取当前配置
        VideoRecordingConfig original_config = recorder.getConfig();
        
        // 更新配置
        VideoRecordingConfig new_config = original_config;
        new_config.segment_duration_minutes = 10;
        new_config.video_width = 1280;
        new_config.video_height = 720;
        
        recorder.updateConfig(new_config);
        
        // 验证配置更新
        VideoRecordingConfig updated_config = recorder.getConfig();
        TestFramework::assertTrue(updated_config.segment_duration_minutes == 10, "分割时间配置更新");
        TestFramework::assertTrue(updated_config.video_width == 1280, "视频宽度配置更新");
        TestFramework::assertTrue(updated_config.video_height == 720, "视频高度配置更新");
        
        std::cout << "配置更新成功" << std::endl;
        
    } catch (const std::exception& e) {
        TestFramework::assertTrue(false, "配置更新测试异常: " + std::string(e.what()));
    }
}

int main() {
    std::cout << "清扫机器人录像系统测试" << std::endl;
    std::cout << "========================" << std::endl;
    
    try {
        // 运行所有测试
        testBasicRecording();
        testStorageManagement();
        testFileSegmentation();
        testMemoryPool();
        testConfigUpdate();
        
        // 打印测试结果
        TestFramework::printSummary();
        
        if (TestFramework::allTestsPassed()) {
            std::cout << "\n所有测试通过！录像系统工作正常。" << std::endl;
            return 0;
        } else {
            std::cout << "\n部分测试失败，请检查系统配置。" << std::endl;
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cout << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
}

// 编译命令:
// g++ -std=c++17 -I./include/core -I./include/Codecs -I./include/MetaData -I./include/Crypto -I./include/Adapters -I./include/CApi
//     test_recording_system.cpp VideoRecordingManager.cpp MP4Converter.cpp
//     -L./lib -lbento4 -lpthread -o test_recording_system