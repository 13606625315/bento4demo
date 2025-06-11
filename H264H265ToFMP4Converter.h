#ifndef H264H265TOFMP4CONVERTER_H
#define H264H265TOFMP4CONVERTER_H

#include "Ap4.h"
#include <string>
#include <vector>
#include <memory>
#define TIME_SCALE 24000

class H264H265ToFMP4Converter {
public:
    H264H265ToFMP4Converter();
    ~H264H265ToFMP4Converter();
    
    // 编码控制接口
    AP4_Result StartEncode(const std::string& codec_type, int width = 1920, int height = 1080, double fps = 25.0);
    AP4_Result AddSample(const unsigned char* data, size_t size, bool is_keyframe, 
                        AP4_UI64 &dts, AP4_UI64 &cts);
    AP4_Result EndEncode(const std::string& output_file);
    
    
    // 重置编码器状态
    void Reset();
    
private:
    // 内部状态
    std::string m_codec_type;
    AP4_Movie* m_movie;
    AP4_Track* m_video_track;
    AP4_SyntheticSampleTable* m_sample_table;
    AP4_ByteStream* m_output_stream;
    AP4_UI64 m_total_duration;
    bool m_owns_output_stream;
    bool m_encoding_started;
    AP4_UI32 m_sample_count;
    int m_width;
    int m_height;
    double m_fps;
    
    // 参数集数据
    std::vector<unsigned char> m_vps_data;
    std::vector<unsigned char> m_sps_data;
    std::vector<unsigned char> m_pps_data;
    
    // 内部方法
    AP4_SyntheticSampleTable* CreateSampleTable(const std::string& codec_type, int width, int height);

    // NAL单元处理方法
    bool parseNALU(const uint8_t* data, size_t size, std::vector<std::pair<const uint8_t*, size_t>>& nalus);
    bool IsKeyFrame(const std::vector<unsigned char>& nal_unit, const std::string& codec_type);
    AP4_Result WriteInitializationSegment();
    void CleanupResources();
};

#endif // H264H265TOFMP4CONVERTER_H