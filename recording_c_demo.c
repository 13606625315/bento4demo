/*******************************************************************************
* recording_c_demo.c
*
* Copyright (C) 2024 Recording System
*
* Author: AI Assistant
* Version: V1.0.0 2024-01-01 Create
* Description: 清扫机器人录像系统C接口使用示例
*
*******************************************************************************/

#include "recording_c_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#define sleep(x) Sleep((x) * 1000)
#else
#include <unistd.h>
#include <termios.h>
#endif

// 全局变量
static volatile int g_running = 1;
static int g_recording_active = 0;

/**
 * @brief 错误回调函数
 */
void error_callback(RecordingErrorCode error_code, const char* error_msg) {
    printf("[ERROR] Code: %d, Message: %s\n", error_code, error_msg ? error_msg : "Unknown error");
}

/**
 * @brief 信号处理函数
 */
void signal_handler(int sig) {
    printf("\n[INFO] Received signal %d, shutting down...\n", sig);
    g_running = 0;
}

/**
 * @brief 打印录像状态
 */
void print_recording_status(void) {
    RecordingStatus status;
    RecordingErrorCode ret = recording_get_status(&status);
    
    if (ret != RECORDING_SUCCESS) {
        printf("[ERROR] Failed to get recording status: %d\n", ret);
        return;
    }
    
    printf("\n=== Recording Status ===\n");
    printf("State: ");
    switch (status.state) {
        case RECORDING_STATE_STOPPED:
            printf("STOPPED\n");
            break;
        case RECORDING_STATE_RECORDING:
            printf("RECORDING\n");
            break;
        default:
            printf("UNKNOWN\n");
            break;
    }
    
    printf("Total Size: %.2f MB\n", status.total_size_bytes / (1024.0 * 1024.0));
    printf("File Count: %u\n", status.file_count);
    printf("Available Space: %.2f GB\n", status.available_space_bytes / (1024.0 * 1024.0 * 1024.0));
    printf("Current Segment Duration: %u seconds\n", status.current_segment_duration_sec);
    
    if (status.current_filename && strlen(status.current_filename) > 0) {
        printf("Current File: %s\n", status.current_filename);
    }
    
    if (status.last_error && strlen(status.last_error) > 0) {
        printf("Last Error: %s\n", status.last_error);
    }
    
    printf("========================\n\n");
}

/**
 * @brief 打印录像文件列表
 */
void print_file_list(void) {
    char** file_list = NULL;
    unsigned int count = 0;
    
    RecordingErrorCode ret = recording_get_file_list(&file_list, &count);
    if (ret != RECORDING_SUCCESS) {
        printf("[ERROR] Failed to get file list: %d\n", ret);
        return;
    }
    
    printf("\n=== Recording Files (%u) ===\n", count);
    for (unsigned int i = 0; i < count; i++) {
        printf("%u. %s\n", i + 1, file_list[i]);
    }
    printf("============================\n\n");
    
    recording_free_file_list(file_list, count);
}

/**
 * @brief 打印帮助信息
 */
void print_help(void) {
    printf("\n=== Commands ===\n");
    printf("s - Start recording\n");
    printf("p - Pause recording\n");
    printf("r - Resume recording\n");
    printf("t - Stop recording\n");
    printf("f - Split current file\n");
    printf("l - List recording files\n");
    printf("c - Cleanup old files (keep 24 hours)\n");
    printf("i - Show recording status\n");
    printf("h - Show this help\n");
    printf("q - Quit\n");
    printf("================\n\n");
}

/**
 * @brief 获取用户输入字符
 */
int get_char(void) {
#ifdef _WIN32
    return _getch();
#else
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

/**
 * @brief 主函数
 */
int main(int argc, char* argv[]) {
    (void)argc;  // 标记参数为故意未使用
    (void)argv;  // 标记参数为故意未使用
    
    printf("=== 清扫机器人录像系统 C接口演示 ===\n");
    printf("Version: %s\n", recording_get_version());
    printf("=====================================\n\n");
    
    // 注册信号处理
    signal(SIGINT, signal_handler);
#ifdef SIGTERM
    signal(SIGTERM, signal_handler);
#endif
    
    // 设置错误回调
    recording_set_error_callback(error_callback);
    
    // 初始化录像系统
    RecordingConfig config = {
        .storage_path = "./recordings_c",
        .segment_duration_sec = 300,        // 5分钟分段
        .max_storage_gb = 5,                // 最大5GB
        .memory_pool_size_mb = 50,          // 50MB内存池
        .min_file_duration_sec = 60,        // 最小1分钟
        .enable_audio = 0,                  // 暂不启用音频
        .video_stream_type = 0              // 主码流
    };
    
    RecordingErrorCode ret = recording_init(&config);
    if (ret != RECORDING_SUCCESS) {
        printf("[ERROR] Failed to initialize recording system: %d\n", ret);
        return -1;
    }
    
    printf("[INFO] Recording system initialized successfully\n");
    printf("[INFO] Storage path: %s\n", config.storage_path);
    printf("[INFO] Segment duration: %u seconds\n", config.segment_duration_sec);
    printf("[INFO] Max storage: %u GB\n", config.max_storage_gb);
    
    print_help();
    
    // 主循环
    while (g_running) {
        printf("Enter command (h for help): ");
        fflush(stdout);
        
        int ch = get_char();
        printf("%c\n", ch);
        
        switch (ch) {
            case 's':
            case 'S':
                if (!g_recording_active) {
                    ret = recording_start(0); // 通道0
                    if (ret == RECORDING_SUCCESS) {
                        printf("[INFO] Recording started\n");
                        g_recording_active = 1;
                    } else {
                        printf("[ERROR] Failed to start recording: %d\n", ret);
                    }
                } else {
                    printf("[INFO] Recording is already active\n");
                }
                break;
                
           
                
            case 't':
            case 'T':
                if (g_recording_active) {
                    ret = recording_stop();
                    if (ret == RECORDING_SUCCESS) {
                        printf("[INFO] Recording stopped\n");
                        g_recording_active = 0;
                    } else {
                        printf("[ERROR] Failed to stop recording: %d\n", ret);
                    }
                } else {
                    printf("[INFO] Recording is not active\n");
                }
                break;
                
            case 'f':
            case 'F':
                if (g_recording_active) {
                    ret = recording_split_file();
                    if (ret == RECORDING_SUCCESS) {
                        printf("[INFO] File split successfully\n");
                    } else {
                        printf("[ERROR] Failed to split file: %d\n", ret);
                    }
                } else {
                    printf("[INFO] Recording is not active\n");
                }
                break;
                
            case 'l':
            case 'L':
                print_file_list();
                break;
                
            case 'c':
            case 'C':
                ret = recording_cleanup_old_files(24); // 保留24小时
                if (ret == RECORDING_SUCCESS) {
                    printf("[INFO] Old files cleaned up (kept 24 hours)\n");
                } else {
                    printf("[ERROR] Failed to cleanup old files: %d\n", ret);
                }
                break;
                
            case 'i':
            case 'I':
                print_recording_status();
                break;
                
            case 'h':
            case 'H':
                print_help();
                break;
                
            case 'q':
            case 'Q':
                printf("[INFO] Quitting...\n");
                g_running = 0;
                break;
                
            default:
                printf("[INFO] Unknown command. Press 'h' for help\n");
                break;
        }
        
        if (g_running) {
            printf("\n");
        }
    }
    
    // 清理
    if (g_recording_active) {
        printf("[INFO] Stopping recording...\n");
        recording_stop();
    }
    
    printf("[INFO] Deinitializing recording system...\n");
    ret = recording_deinit();
    if (ret != RECORDING_SUCCESS) {
        printf("[ERROR] Failed to deinitialize recording system: %d\n", ret);
    } else {
        printf("[INFO] Recording system deinitialized successfully\n");
    }
    
    printf("[INFO] Program exited\n");
    return 0;
}