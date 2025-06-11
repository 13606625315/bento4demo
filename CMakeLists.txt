cmake_minimum_required(VERSION 3.10)

project(H264_H265_to_fMP4 
        VERSION 1.0.0
        DESCRIPTION "H264/H265 to fMP4 converter using Bento4"
        LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-g -O0)  # 添加调试符号，禁用优化
# 设置构建类型默认值
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
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
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include;${CMAKE_SOURCE_DIR}/include/core;${CMAKE_SOURCE_DIR}/include/Codecs;${CMAKE_SOURCE_DIR}/include/Crypto;${CMAKE_SOURCE_DIR}/include/MetaData;${CMAKE_SOURCE_DIR}/include/Adapters;${CMAKE_SOURCE_DIR}/include/CApi"
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

# 创建可执行文件
create_converter_executable(h264_h265_to_fmp4_new "main.cpp;H264H265ToFMP4Converter.cpp")


# 安装规则（可选）
install(TARGETS h264_h265_to_fmp4_new 
        RUNTIME DESTINATION bin)

# 打印配置信息
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")