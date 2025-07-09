#include "VideoRecordingManager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <vector>
#include <fstream>

// 模拟清扫状态
enum class CleaningState {
    IDLE,
    CLEANING,
    PAUSED
};

// 模拟视频帧生成器
class MockVideoFrameGenerator {
public:
    MockVideoFrameGenerator() : m_frame_counter(0) {
        // 生成模拟的H.264数据
        generateMockH264Data();
    }
    
    std::vector<uint8_t> generateFrame(bool& is_keyframe) {
        m_frame_counter++;
        
        // 每30帧生成一个关键帧
        is_keyframe = (m_frame_counter % 30 == 1);
        
        std::vector<uint8_t> frame_data;
        
        if (is_keyframe) {
            // 添加SPS
            frame_data.insert(frame_data.end(), m_sps_data.begin(), m_sps_data.end());
            // 添加PPS
            frame_data.insert(frame_data.end(), m_pps_data.begin(), m_pps_data.end());
            // 添加IDR帧
            frame_data.insert(frame_data.end(), m_idr_data.begin(), m_idr_data.end());
        } else {
            // 添加P帧
            frame_data.insert(frame_data.end(), m_p_frame_data.begin(), m_p_frame_data.end());
        }
        
        return frame_data;
    }
    
private:
    uint32_t m_frame_counter;
    std::vector<uint8_t> m_sps_data;
    std::vector<uint8_t> m_pps_data;
    std::vector<uint8_t> m_idr_data;
    std::vector<uint8_t> m_p_frame_data;
    
    void generateMockH264Data() {
        // 模拟SPS (Sequence Parameter Set)
        m_sps_data = {
            0x00, 0x00, 0x00, 0x01, // 起始码
            0x67, 0x64, 0x00, 0x28, // SPS NAL header + profile_idc
            0xAC, 0x2B, 0x40, 0x50, 0x1E, 0xD0, 0x0F, 0x12,
            0x26, 0xA0, 0x00, 0x00, 0x03, 0x00, 0x20, 0x00,
            0x00, 0x06, 0x51, 0xE3, 0x06, 0x54
        };
        
        // 模拟PPS (Picture Parameter Set)
        m_pps_data = {
            0x00, 0x00, 0x00, 0x01, // 起始码
            0x68, 0xEE, 0x3C, 0xB0  // PPS NAL header + data
        };
        
        // 模拟IDR帧数据
        m_idr_data = {
            0x00, 0x00, 0x00, 0x01, // 起始码
            0x65, 0x88, 0x84, 0x00, // IDR NAL header
        };
        // 添加一些模拟的压缩数据
        for (int i = 0; i < 1000; ++i) {
            m_idr_data.push_back(static_cast<uint8_t>(rand() % 256));
        }
        
        // 模拟P帧数据
        m_p_frame_data = {
            0x00, 0x00, 0x00, 0x01, // 起始码
            0x61, 0x9A, 0x12, 0x34, // P帧 NAL header
        };
        // 添加一些模拟的压缩数据
        for (int i = 0; i < 500; ++i) {
            m_p_frame_data.push_back(static_cast<uint8_t>(rand() % 256));
        }
    }
};

// 清扫机器人模拟器
class CleaningRobotSimulator {
public:
    CleaningRobotSimulator() 
        : m_state(CleaningState::IDLE)
        , m_cleaning_start_time(std::chrono::system_clock::now())
        , m_should_stop(false) {
    }
    
    void startCleaning() {
        if (m_state != CleaningState::IDLE) {
            std::cout << "清扫已经在进行中或暂停状态" << std::endl;
            return;
        }
        
        m_state = CleaningState::CLEANING;
        m_cleaning_start_time = std::chrono::system_clock::now();
        
        std::cout << "=== 开始清扫 ===" << std::endl;
        
        // 开始录像
        auto& recorder = GlobalRecordingManager::getInstance();
        if (recorder.startRecording()) {
            std::cout << "录像开始" << std::endl;
        } else {
            std::cout << "录像启动失败" << std::endl;
        }
    }
    
    void pauseCleaning() {
        if (m_state != CleaningState::CLEANING) {
            std::cout << "当前不在清扫状态" << std::endl;
            return;
        }
        
        m_state = CleaningState::PAUSED;
        std::cout << "=== 暂停清扫 ===" << std::endl;
        
        // 暂停录像
        auto& recorder = GlobalRecordingManager::getInstance();
        if (recorder.pauseRecording()) {
            std::cout << "录像暂停" << std::endl;
        }
    }
    
    void resumeCleaning() {
        if (m_state != CleaningState::PAUSED) {
            std::cout << "当前不在暂停状态" << std::endl;
            return;
        }
        
        m_state = CleaningState::CLEANING;
        std::cout << "=== 恢复清扫 ===" << std::endl;
        
        // 恢复录像
        auto& recorder = GlobalRecordingManager::getInstance();
        if (recorder.startRecording()) {
            std::cout << "录像恢复" << std::endl;
        }
    }
    
    void stopCleaning() {
        if (m_state == CleaningState::IDLE) {
            std::cout << "当前不在清扫状态" << std::endl;
            return;
        }
        
        m_state = CleaningState::IDLE;
        std::cout << "=== 结束清扫 ===" << std::endl;
        
        // 停止录像
        auto& recorder = GlobalRecordingManager::getInstance();
        if (recorder.stopRecording()) {
            std::cout << "录像停止" << std::endl;
        }
        
        m_should_stop = true;
    }
    
    CleaningState getState() const { return m_state; }
    bool shouldStop() const { return m_should_stop; }
    
    std::chrono::system_clock::time_point getCleaningStartTime() const {
        return m_cleaning_start_time;
    }
    
private:
    CleaningState m_state;
    std::chrono::system_clock::time_point m_cleaning_start_time;
    bool m_should_stop;
};

// 显示存储状态
void displayStorageStatus() {
    auto& recorder = GlobalRecordingManager::getInstance();
    
    size_t used = recorder.getUsedStorage();
    size_t available = recorder.getAvailableStorage();
    auto files = recorder.getFileList();
    
    std::cout << "\n=== 存储状态 ===" << std::endl;
    std::cout << "已使用: " << (used / 1024 / 1024) << " MB" << std::endl;
    std::cout << "可用: " << (available / 1024 / 1024) << " MB" << std::endl;
    std::cout << "录像文件数量: " << files.size() << std::endl;
    
    if (!files.empty()) {
        std::cout << "最新文件: " << files.back().filename << std::endl;
    }
    std::cout << "==================" << std::endl;
}

// 模拟用户控制
void simulateUserControl(CleaningRobotSimulator& robot) {
    std::cout << "\n控制命令:" << std::endl;
    std::cout << "1 - 开始清扫" << std::endl;
    std::cout << "2 - 暂停清扫" << std::endl;
    std::cout << "3 - 恢复清扫" << std::endl;
    std::cout << "4 - 停止清扫" << std::endl;
    std::cout << "5 - 显示存储状态" << std::endl;
    std::cout << "0 - 退出" << std::endl;
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1:
            robot.startCleaning();
            break;
        case 2:
            robot.pauseCleaning();
            break;
        case 3:
            robot.resumeCleaning();
            break;
        case 4:
            robot.stopCleaning();
            break;
        case 5:
            displayStorageStatus();
            break;
        case 0:
            robot.stopCleaning();
            break;
        default:
            std::cout << "无效选择" << std::endl;
            break;
    }
}

int main() {
    std::cout << "清扫机器人录像系统演示" << std::endl;
    std::cout << "========================" << std::endl;
    
    try {
        // 配置录像参数
        VideoRecordingConfig config;
        config.max_storage_size = 100 * 1024 * 1024; // 100MB用于演示
        config.segment_duration_minutes = 1; // 1分钟分割用于演示
        config.estimated_file_size = 2 * 1024 * 1024; // 2MB预估
        config.storage_path = "./test_recordings/";
        config.codec_type = "h264";
        config.video_width = 1920;
        config.video_height = 1080;
        config.video_fps = 25.0;
        
        // 初始化录像管理器
        auto& recorder = GlobalRecordingManager::getInstance();
        recorder.updateConfig(config);
        
        // 创建清扫机器人模拟器
        CleaningRobotSimulator robot;
        
        // 创建视频帧生成器
        MockVideoFrameGenerator frame_generator;
        
        // 主循环
        std::thread control_thread([&robot]() {
            while (!robot.shouldStop()) {
                simulateUserControl(robot);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
        
        // 视频帧生成循环
        while (!robot.shouldStop()) {
            if (robot.getState() == CleaningState::CLEANING) {
                bool is_keyframe;
                auto frame_data = frame_generator.generateFrame(is_keyframe);
                
                if (!recorder.addVideoFrame(frame_data.data(), frame_data.size(), is_keyframe)) {
                    std::cout << "添加视频帧失败" << std::endl;
                }
                
                // 检查清扫时间，演示超过5分钟的分割
                auto now = std::chrono::system_clock::now();
                auto cleaning_duration = std::chrono::duration_cast<std::chrono::minutes>(
                    now - robot.getCleaningStartTime());
                
                if (cleaning_duration.count() >= 6) { // 6分钟后自动停止演示
                    std::cout << "\n演示：清扫超过6分钟，自动停止" << std::endl;
                    robot.stopCleaning();
                }
            }
            
            // 模拟25fps
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
        }
        
        if (control_thread.joinable()) {
            control_thread.join();
        }
        
        // 显示最终状态
        displayStorageStatus();
        
        std::cout << "\n演示结束" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

// 编译命令示例:
// g++ -std=c++17 -I./include/core -I./include/Codecs -I./include/MetaData -I./include/Crypto -I./include/Adapters -I./include/CApi
//     recording_demo.cpp VideoRecordingManager.cpp MP4Converter.cpp
//     -L./lib -lbento4 -o recording_demo