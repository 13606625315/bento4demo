#!/bin/bash

echo "Testing FMP4Converter..."
echo

echo "Building the project..."
cd build
make
if [ $? -ne 0 ]; then
    echo "Build failed!"
    read -p "Press any key to continue..."
    exit 1
fi

echo
echo "Running FMP4 conversion test..."
./bin/h264_h265_to_fmp4_new --fmp4 ../v_demo.dav output_fmp4.mp4 h264

echo
echo "Running regular MP4 conversion test for comparison..."
./bin/h264_h265_to_fmp4_new ../v_demo.dav output_regular.mp4 h264

echo
echo "Test completed! Check the output files:"
echo "- output_fmp4.mp4 (fragmented MP4)"
echo "- output_regular.mp4 (regular MP4)"

read -p "Press any key to continue..."