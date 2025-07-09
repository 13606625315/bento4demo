@echo off
echo ========================================
echo 清扫机器人录像系统构建和测试脚本
echo ========================================

:: 设置编码为UTF-8
chcp 65001 > nul

:: 检查是否存在构建目录
if not exist "build" (
    echo 创建构建目录...
    mkdir build
)

:: 进入构建目录
cd build

:: 清理之前的构建
echo 清理之前的构建文件...
if exist "CMakeCache.txt" del /f /q CMakeCache.txt
if exist "CMakeFiles" rmdir /s /q CMakeFiles

:: 配置项目
echo 配置CMake项目...
cmake .. -G "Visual Studio 16 2019" -A x64
if %errorlevel% neq 0 (
    echo CMake配置失败！
    pause
    exit /b 1
)

:: 构建项目
echo 构建项目...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo 构建失败！
    pause
    exit /b 1
)

echo 构建成功！

:: 创建测试录像目录
if not exist "test_recordings" (
    echo 创建测试录像目录...
    mkdir test_recordings
)

:: 运行测试
echo ========================================
echo 运行录像系统测试...
echo ========================================

if exist "Release\test_recording_system.exe" (
    Release\test_recording_system.exe
    if %errorlevel% equ 0 (
        echo.
        echo ========================================
        echo 所有测试通过！系统工作正常。
        echo ========================================
    ) else (
        echo.
        echo ========================================
        echo 测试失败！请检查系统配置。
        echo ========================================
    )
) else (
    echo 测试程序不存在，请检查构建是否成功。
)

echo.
echo 可用的程序：
if exist "Release\main.exe" echo - main.exe: 原始MP4转换器演示
if exist "Release\fmp4_demo.exe" echo - fmp4_demo.exe: FMP4转换器演示
if exist "Release\recording_demo.exe" echo - recording_demo.exe: 录像系统交互演示
if exist "Release\test_recording_system.exe" echo - test_recording_system.exe: 录像系统自动测试

echo.
echo 要运行交互式演示，请执行：
echo Release\recording_demo.exe

echo.
echo 构建和测试完成！
pause