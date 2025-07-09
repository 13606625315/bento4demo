@echo off
echo ========================================
echo 清扫机器人录像系统 C接口构建脚本
echo ========================================
echo.

:: 设置编码为UTF-8
chcp 65001 > nul

:: 创建构建目录
if not exist "build" (
    echo [INFO] 创建构建目录...
    mkdir build
) else (
    echo [INFO] 清理旧的构建文件...
    del /Q build\* 2>nul
)

cd build

:: 配置CMake项目
echo [INFO] 配置CMake项目...
cmake .. -G "Visual Studio 16 2019" -A x64
if %ERRORLEVEL% neq 0 (
    echo [ERROR] CMake配置失败!
    pause
    exit /b 1
)

:: 构建项目
echo [INFO] 构建项目...
cmake --build . --config Release
if %ERRORLEVEL% neq 0 (
    echo [ERROR] 项目构建失败!
    pause
    exit /b 1
)

echo [INFO] 构建完成!
echo.

:: 创建测试录像目录
if not exist "recordings_c" (
    echo [INFO] 创建测试录像目录...
    mkdir recordings_c
)

:: 检查可执行文件
if exist "Release\recording_c_demo.exe" (
    echo [INFO] C接口演示程序构建成功: Release\recording_c_demo.exe
) else (
    echo [ERROR] C接口演示程序构建失败!
)

if exist "Release\test_c_api.exe" (
    echo [INFO] C接口测试程序构建成功: Release\test_c_api.exe
    echo.
    
    :: 询问是否运行测试
    set /p run_test="是否运行C接口单元测试? (y/n): "
    if /i "%run_test%"=="y" (
        echo [INFO] 运行C接口单元测试...
        echo.
        Release\test_c_api.exe
        echo.
    )
    
    :: 询问是否运行演示程序
    set /p run_demo="是否运行C接口演示程序? (y/n): "
    if /i "%run_demo%"=="y" (
        echo [INFO] 启动C接口演示程序...
        echo [INFO] 使用说明:
        echo   - 按 's' 开始录像
        echo   - 按 'p' 暂停录像
        echo   - 按 'r' 恢复录像
        echo   - 按 't' 停止录像
        echo   - 按 'i' 查看状态
        echo   - 按 'l' 列出文件
        echo   - 按 'h' 查看帮助
        echo   - 按 'q' 退出程序
        echo.
        pause
        Release\recording_c_demo.exe
    )
) else (
    echo [ERROR] C接口测试程序构建失败!
)

echo.
echo [INFO] 脚本执行完成
pause