/*******************************************************************************
* test_c_api.c
*
* Copyright (C) 2024 Recording System
*
* Author: AI Assistant
* Version: V1.0.0 2024-01-01 Create
* Description: 清扫机器人录像系统C接口单元测试
*
*******************************************************************************/

#include "recording_c_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep((x) * 1000)
#else
#include <unistd.h>
#endif

// 测试结果统计
static int g_test_count = 0;
static int g_test_passed = 0;
static int g_test_failed = 0;

// 错误回调测试
static RecordingErrorCode g_last_error_code = RECORDING_SUCCESS;
static char g_last_error_msg[256] = {0};

/**
 * @brief 测试框架宏
 */
#define TEST_START(name) \
    do { \
        printf("\n[TEST] %s\n", name); \
        g_test_count++; \
    } while(0)

#define TEST_ASSERT(condition, message) \
    do { \
        if (condition) { \
            printf("  ✓ %s\n", message); \
        } else { \
            printf("  ✗ %s\n", message); \
            g_test_failed++; \
            return; \
        } \
    } while(0)

#define TEST_END() \
    do { \
        g_test_passed++; \
        printf("  [PASS]\n"); \
    } while(0)

/**
 * @brief 错误回调函数
 */
void test_error_callback(RecordingErrorCode error_code, const char* error_msg) {
    g_last_error_code = error_code;
    if (error_msg) {
        strncpy(g_last_error_msg, error_msg, sizeof(g_last_error_msg) - 1);
        g_last_error_msg[sizeof(g_last_error_msg) - 1] = '\0';
    }
    printf("  [ERROR_CALLBACK] Code: %d, Message: %s\n", error_code, error_msg ? error_msg : "NULL");
}

/**
 * @brief 测试版本信息
 */
void test_version_info() {
    TEST_START("Version Info Test");
    
    const char* version = recording_get_version();
    TEST_ASSERT(version != NULL, "Version string is not NULL");
    TEST_ASSERT(strlen(version) > 0, "Version string is not empty");
    printf("  Version: %s\n", version);
    
    TEST_END();
}

/**
 * @brief 测试错误回调设置
 */
void test_error_callback_setup() {
    TEST_START("Error Callback Setup Test");
    
    // 重置错误状态
    g_last_error_code = RECORDING_SUCCESS;
    memset(g_last_error_msg, 0, sizeof(g_last_error_msg));
    
    // 设置错误回调
    recording_set_error_callback(test_error_callback);
    
    // 测试无效参数触发错误回调
    RecordingErrorCode ret = recording_init(NULL);
    TEST_ASSERT(ret == RECORDING_ERROR_INVALID_PARAM, "Invalid parameter returns correct error code");
    
    TEST_END();
}

/**
 * @brief 测试初始化和反初始化
 */
void test_init_deinit() {
    TEST_START("Init/Deinit Test");
    
    // 测试有效配置初始化
    RecordingConfig config = {
        .storage_path = "./test_recordings",
        .segment_duration_sec = 60,
        .max_storage_gb = 1,
        .memory_pool_size_mb = 10,
        .min_file_duration_sec = 30,
        .enable_audio = 0,
        .video_stream_type = 0
    };
    
    RecordingErrorCode ret = recording_init(&config);
    TEST_ASSERT(ret == RECORDING_SUCCESS, "Valid config initialization succeeds");
    
    // 测试重复初始化
    ret = recording_init(&config);
    TEST_ASSERT(ret == RECORDING_ERROR_ALREADY_STARTED, "Duplicate initialization returns error");
    
    // 测试反初始化
    ret = recording_deinit();
    TEST_ASSERT(ret == RECORDING_SUCCESS, "Deinitialization succeeds");
    
    // 测试重复反初始化
    ret = recording_deinit();
    TEST_ASSERT(ret == RECORDING_ERROR_NOT_INITIALIZED, "Duplicate deinitialization returns error");
    
    TEST_END();
}

/**
 * @brief 测试未初始化状态下的操作
 */
void test_uninitialized_operations() {
    TEST_START("Uninitialized Operations Test");
    
    // 确保系统未初始化
    recording_deinit();
    
    RecordingErrorCode ret;
    
    // 测试未初始化状态下的各种操作
    ret = recording_start(0);
    TEST_ASSERT(ret == RECORDING_ERROR_NOT_INITIALIZED, "Start recording without init returns error");
    
    ret = recording_stop();
    TEST_ASSERT(ret == RECORDING_ERROR_NOT_INITIALIZED, "Stop recording without init returns error");
    
    RecordingStatus status;
    ret = recording_get_status(&status);
    TEST_ASSERT(ret == RECORDING_ERROR_NOT_INITIALIZED, "Get status without init returns error");
    
    TEST_END();
}

/**
 * @brief 测试录像控制流程
 */
void test_recording_control() {
    TEST_START("Recording Control Test");
    
    // 初始化系统
    RecordingConfig config = {
        .storage_path = "./test_recordings",
        .segment_duration_sec = 30,  // 短分段便于测试
        .max_storage_gb = 1,
        .memory_pool_size_mb = 20,
        .min_file_duration_sec = 10,
        .enable_audio = 0,
        .video_stream_type = 0
    };
    
    RecordingErrorCode ret = recording_init(&config);
    TEST_ASSERT(ret == RECORDING_SUCCESS, "System initialization succeeds");
    
    // 测试开始录像
    ret = recording_start(0);
    TEST_ASSERT(ret == RECORDING_SUCCESS, "Start recording succeeds");
    

    // 等待一小段时间
    sleep(1);
    
    // 测试停止录像
    ret = recording_stop();
    TEST_ASSERT(ret == RECORDING_SUCCESS, "Stop recording succeeds");
    
    // 清理
    recording_deinit();
    
    TEST_END();
}

/**
 * @brief 测试状态查询
 */
void test_status_query() {
    TEST_START("Status Query Test");
    
    // 初始化系统
    RecordingConfig config = {
        .storage_path = "./test_recordings",
        .segment_duration_sec = 60,
        .max_storage_gb = 1,
        .memory_pool_size_mb = 20,
        .min_file_duration_sec = 30,
        .enable_audio = 0,
        .video_stream_type = 0
    };
    
    RecordingErrorCode ret = recording_init(&config);
    TEST_ASSERT(ret == RECORDING_SUCCESS, "System initialization succeeds");
    
    // 测试停止状态
    RecordingStatus status;
    ret = recording_get_status(&status);
    TEST_ASSERT(ret == RECORDING_SUCCESS, "Get status succeeds");
    TEST_ASSERT(status.state == RECORDING_STATE_STOPPED, "Initial state is STOPPED");
    
    // 开始录像并测试状态
    recording_start(0);
    ret = recording_get_status(&status);
    TEST_ASSERT(ret == RECORDING_SUCCESS, "Get status during recording succeeds");
    TEST_ASSERT(status.state == RECORDING_STATE_RECORDING, "State is RECORDING");
    

    
    // 停止录像
    recording_stop();
    ret = recording_get_status(&status);
    TEST_ASSERT(ret == RECORDING_SUCCESS, "Get status after stop succeeds");
    TEST_ASSERT(status.state == RECORDING_STATE_STOPPED, "State is STOPPED after stop");
    
    // 清理
    recording_deinit();
    
    TEST_END();
}

/**
 * @brief 测试文件列表功能
 */
void test_file_list() {
    TEST_START("File List Test");
    
    // 初始化系统
    RecordingConfig config = {
        .storage_path = "./test_recordings",
        .segment_duration_sec = 60,
        .max_storage_gb = 1,
        .memory_pool_size_mb = 20,
        .min_file_duration_sec = 30,
        .enable_audio = 0,
        .video_stream_type = 0
    };
    
    RecordingErrorCode ret = recording_init(&config);
    TEST_ASSERT(ret == RECORDING_SUCCESS, "System initialization succeeds");
    
    // 测试获取文件列表
    char** file_list = NULL;
    unsigned int count = 0;
    
    ret = recording_get_file_list(&file_list, &count);
    TEST_ASSERT(ret == RECORDING_SUCCESS, "Get file list succeeds");
    printf("  File count: %u\n", count);
    
    // 释放文件列表
    recording_free_file_list(file_list, count);
    
    // 测试无效参数
    ret = recording_get_file_list(NULL, &count);
    TEST_ASSERT(ret == RECORDING_ERROR_INVALID_PARAM, "Get file list with NULL pointer returns error");
    
    // 清理
    recording_deinit();
    
    TEST_END();
}

/**
 * @brief 测试配置更新
 */
void test_config_update() {
    TEST_START("Config Update Test");
    
    // 初始化系统
    RecordingConfig config = {
        .storage_path = "./test_recordings",
        .segment_duration_sec = 60,
        .max_storage_gb = 1,
        .memory_pool_size_mb = 20,
        .min_file_duration_sec = 30,
        .enable_audio = 0,
        .video_stream_type = 0
    };
    
    RecordingErrorCode ret = recording_init(&config);
    TEST_ASSERT(ret == RECORDING_SUCCESS, "System initialization succeeds");
    
    // 更新配置
    RecordingConfig new_config = config;
    new_config.segment_duration_sec = 120;
    new_config.max_storage_gb = 2;
    
    ret = recording_update_config(&new_config);
    TEST_ASSERT(ret == RECORDING_SUCCESS, "Config update succeeds");
    
    // 测试无效参数
    ret = recording_update_config(NULL);
    TEST_ASSERT(ret == RECORDING_ERROR_INVALID_PARAM, "Update config with NULL returns error");
    
    // 清理
    recording_deinit();
    
    TEST_END();
}

/**
 * @brief 测试高级功能
 */
void test_advanced_features() {
    TEST_START("Advanced Features Test");
    
    // 初始化系统
    RecordingConfig config = {
        .storage_path = "./test_recordings",
        .segment_duration_sec = 60,
        .max_storage_gb = 1,
        .memory_pool_size_mb = 20,
        .min_file_duration_sec = 30,
        .enable_audio = 0,
        .video_stream_type = 0
    };
    
    RecordingErrorCode ret = recording_init(&config);
    TEST_ASSERT(ret == RECORDING_SUCCESS, "System initialization succeeds");
    
    // 测试文件清理
    ret = recording_cleanup_old_files(24);
    TEST_ASSERT(ret == RECORDING_SUCCESS, "Cleanup old files succeeds");
    
    // 开始录像以测试文件分割
    recording_start(0);
    
    // 测试手动文件分割
    ret = recording_split_file();
    TEST_ASSERT(ret == RECORDING_SUCCESS, "Manual file split succeeds");
    
    recording_stop();
    
    // 清理
    recording_deinit();
    
    TEST_END();
}

/**
 * @brief 主测试函数
 */
int main() {
    printf("========================================\n");
    printf("清扫机器人录像系统 C接口单元测试\n");
    printf("Version: %s\n", recording_get_version());
    printf("========================================\n");
    
    // 运行所有测试
    test_version_info();
    test_error_callback_setup();
    test_init_deinit();
    test_uninitialized_operations();
    test_recording_control();
    test_status_query();
    test_file_list();
    test_config_update();
    test_advanced_features();
    
    // 输出测试结果
    printf("\n========================================\n");
    printf("测试结果统计:\n");
    printf("总测试数: %d\n", g_test_count);
    printf("通过: %d\n", g_test_passed);
    printf("失败: %d\n", g_test_failed);
    
    if (g_test_failed == 0) {
        printf("\n🎉 所有测试通过!\n");
    } else {
        printf("\n❌ 有 %d 个测试失败\n", g_test_failed);
    }
    printf("========================================\n");
    
    return g_test_failed == 0 ? 0 : 1;
}