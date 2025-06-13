@echo off
echo Testing FMP4Converter...
echo.

echo Building the project...
cd build
make
if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo Running FMP4 conversion test...
bin\h264_h265_to_fmp4_new.exe --fmp4 ..\v_demo.dav output_fmp4.mp4 h264

echo.
echo Running regular MP4 conversion test for comparison...
bin\h264_h265_to_fmp4_new.exe ..\v_demo.dav output_regular.mp4 h264

echo.
echo Test completed! Check the output files:
echo - output_fmp4.mp4 (fragmented MP4)
echo - output_regular.mp4 (regular MP4)

pause