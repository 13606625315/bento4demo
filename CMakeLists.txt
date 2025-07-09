cmake_minimum_required(VERSION 3.10)

project(VideoRecordingSystem 
        VERSION 1.0.0
        DESCRIPTION "Video Recording System with H264/H265 to fMP4 converter using Bento4"
        LANGUAGES CXX C)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 设置构建类型默认值
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# 调试和优化设置
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0)  # 调试模式：添加调试符号，禁用优化
else()
    add_compile_options(-O2)     # 发布模式：启用优化
endif()

# 编译器特定设置
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 创建 Bento4 导入库目标
add_library(Bento4::libap4 STATIC IMPORTED)

# 查找 Bento4 库
find_library(BENTO4_LIBRARY
             NAMES ap4 libap4
             PATHS ${CMAKE_SOURCE_DIR}/lib
             NO_DEFAULT_PATH)

if(NOT BENTO4_LIBRARY)
    message(FATAL_ERROR "Bento4 library not found. Please ensure libap4.a is in the lib/ directory.")
endif()

# 设置导入库属性
set_target_properties(Bento4::libap4 PROPERTIES
    IMPORTED_LOCATION ${BENTO4_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include;${CMAKE_SOURCE_DIR}/include/core;${CMAKE_SOURCE_DIR}/include/Codecs;${CMAKE_SOURCE_DIR}/include/Crypto;${CMAKE_SOURCE_DIR}/include/MetaData;${CMAKE_SOURCE_DIR}/include/Adapters;${CMAKE_SOURCE_DIR}/include/CApi;${CMAKE_SOURCE_DIR}/include/Framework;${CMAKE_SOURCE_DIR}/include/SystemLibs"
)

message(STATUS "Found Bento4 library: ${BENTO4_LIBRARY}")

# 创建可执行文件函数
function(create_converter_executable target_name source_file)
    add_executable(${target_name} ${source_file})
    
    # 设置 C++ 标准
    target_compile_features(${target_name} PRIVATE cxx_std_11)
    
    # 链接 Bento4 库
    target_link_libraries(${target_name} PRIVATE Bento4::libap4)
    
    # Windows 特定链接
    if(WIN32)
        target_link_libraries(${target_name} PRIVATE ws2_32)
    endif()
    
    # 设置输出目录
    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endfunction()

# 录像系统核心库
add_library(VideoRecordingCore STATIC
    VideoRecordingManager.cpp
    MP4Converter.cpp
    FMP4Converter.cpp
)

target_compile_features(VideoRecordingCore PRIVATE cxx_std_17)

target_link_libraries(VideoRecordingCore PRIVATE Bento4::libap4)
if(WIN32)
    target_link_libraries(VideoRecordingCore PRIVATE ws2_32)
else()
    # Linux/Unix 特定链接库
    target_link_libraries(VideoRecordingCore PRIVATE pthread stdc++fs)
    # 查找并链接 imouos 库（如果存在）
    find_library(IMOUOS_LIBRARY
                 NAMES imouos libimouos
                 PATHS /usr/lib /usr/local/lib ${CMAKE_SOURCE_DIR}/lib
                 NO_DEFAULT_PATH)
    if(IMOUOS_LIBRARY)
        target_link_libraries(VideoRecordingCore PRIVATE ${IMOUOS_LIBRARY})
        message(STATUS "Found imouos library: ${IMOUOS_LIBRARY}")
    else()
        message(WARNING "imouos library not found. Some functionality may not work.")
    endif()
endif()

# 录像系统 C API 库
add_library(VideoRecordingCAPI STATIC
    recording_c_api.cpp
)

target_compile_features(VideoRecordingCAPI PRIVATE cxx_std_17)
target_include_directories(VideoRecordingCAPI PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/Framework
    ${CMAKE_SOURCE_DIR}/include/SystemLibs
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/include/Codecs
    ${CMAKE_SOURCE_DIR}/include/Crypto
    ${CMAKE_SOURCE_DIR}/include/MetaData
    ${CMAKE_SOURCE_DIR}/include/Adapters
    ${CMAKE_SOURCE_DIR}/include/CApi
)
target_link_libraries(VideoRecordingCAPI PRIVATE VideoRecordingCore)
if(NOT WIN32)
    # Linux/Unix 特定链接库
    target_link_libraries(VideoRecordingCAPI PRIVATE pthread stdc++fs)
endif()

# 创建可执行文件
create_converter_executable(h264_h265_to_fmp4_converter "main.cpp;MP4Converter.cpp;FMP4Converter.cpp")

# 录像系统 C++ 演示程序
create_converter_executable(recording_demo "recording_demo.cpp")
target_link_libraries(recording_demo PRIVATE VideoRecordingCore)
if(NOT WIN32)
    # Linux/Unix 特定链接库
    target_link_libraries(recording_demo PRIVATE pthread stdc++fs)
endif()

# 录像系统 C API 演示程序
add_executable(recording_c_demo recording_c_demo.c)
target_compile_features(recording_c_demo PRIVATE c_std_11)
target_include_directories(recording_c_demo PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/Framework
    ${CMAKE_SOURCE_DIR}/include/SystemLibs
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/include/Codecs
    ${CMAKE_SOURCE_DIR}/include/Crypto
    ${CMAKE_SOURCE_DIR}/include/MetaData
    ${CMAKE_SOURCE_DIR}/include/Adapters
    ${CMAKE_SOURCE_DIR}/include/CApi
)
target_link_libraries(recording_c_demo PRIVATE VideoRecordingCAPI)
if(WIN32)
    target_link_libraries(recording_c_demo PRIVATE ws2_32)
else()
    # Linux/Unix 特定链接库
    target_link_libraries(recording_c_demo PRIVATE pthread stdc++fs)
endif()
set_target_properties(recording_c_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# 录像系统测试程序
create_converter_executable(test_recording_system "test_recording_system.cpp")
target_link_libraries(test_recording_system PRIVATE VideoRecordingCore)
if(NOT WIN32)
    # Linux/Unix 特定链接库
    target_link_libraries(test_recording_system PRIVATE pthread stdc++fs)
endif()

# C API 测试程序
add_executable(test_c_api test_c_api.c)
target_compile_features(test_c_api PRIVATE c_std_11)
target_include_directories(test_c_api PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/Framework
    ${CMAKE_SOURCE_DIR}/include/SystemLibs
)
target_link_libraries(test_c_api PRIVATE VideoRecordingCAPI)
if(WIN32)
    target_link_libraries(test_c_api PRIVATE ws2_32)
else()
    # Linux/Unix 特定链接库
    target_link_libraries(test_c_api PRIVATE pthread stdc++fs)
endif()
set_target_properties(test_c_api PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# 安装规则
install(TARGETS 
        h264_h265_to_fmp4_converter
        recording_demo
        recording_c_demo
        test_recording_system
        test_c_api
        RUNTIME DESTINATION bin)

install(TARGETS 
        VideoRecordingCore
        VideoRecordingCAPI
        ARCHIVE DESTINATION lib)

install(FILES 
        VideoRecordingManager.h
        recording_c_api.h
        MP4Converter.h
        FMP4Converter.h
        DESTINATION include)

# 创建测试目录
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test_output)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test_storage)

# 打印配置信息
message(STATUS "=== Video Recording System Configuration ===")
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "C standard: ${CMAKE_C_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Bento4 library: ${BENTO4_LIBRARY}")
message(STATUS "Output directory: ${CMAKE_BINARY_DIR}/bin")
message(STATUS "Test directories created: test_output, test_storage")
message(STATUS "=============================================")