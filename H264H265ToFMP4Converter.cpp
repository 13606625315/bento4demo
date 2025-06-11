#include "H264H265ToFMP4Converter.h"
#include <iostream>
#include <fstream>

H264H265ToFMP4Converter::H264H265ToFMP4Converter() 
    : m_movie(nullptr)
    , m_video_track(nullptr)
    , m_sample_table(nullptr)
    , m_output_stream(nullptr)
    , m_total_duration(0)
    , m_owns_output_stream(false)
    , m_encoding_started(false)
    , m_sample_count(0)
    , m_width(1920)
    , m_height(1080)
    , m_fps(25.0) {
}

H264H265ToFMP4Converter::~H264H265ToFMP4Converter() {
    CleanupResources();
}

// NAL单元分割函数
bool H264H265ToFMP4Converter::parseNALU(const uint8_t* data, size_t size, std::vector<std::pair<const uint8_t*, size_t>>& nalus)
{
    if (!data || size < 4) {
        return false;
    }
    
    nalus.clear();
    
    // 查找起始码并解析NALU
    const uint8_t* start = nullptr;
    size_t i = 0;
    
    // 查找第一个起始码
    while (i + 3 < size) {
        // 检查起始码 (0x00 0x00 0x00 0x01 或 0x00 0x00 0x01)
        if ((data[i] == 0 && data[i+1] == 0 && data[i+2] == 0 && data[i+3] == 1) ||
            (data[i] == 0 && data[i+1] == 0 && data[i+2] == 1)) {
            
            // 确定起始码长度
            int startCodeLen = (data[i+2] == 0 && data[i+3] == 1) ? 4 : 3;
            
            // 如果已经找到了一个NALU，添加到列表
            if (start) {
                nalus.push_back(std::make_pair(start, data + i - start));
            }
            
            // 更新起始位置为当前NALU的开始
            start = data + i + startCodeLen;
            i += startCodeLen;
        } else {
            i++;
        }
    }
    
    // 添加最后一个NALU
    if (start && start < data + size) {
        nalus.push_back(std::make_pair(start, data + size - start));
    }
    
    return !nalus.empty();
}

// 检查是否为关键帧
bool H264H265ToFMP4Converter::IsKeyFrame(const std::vector<unsigned char>& nal_unit, const std::string& codec_type) {
    if (nal_unit.empty()) return false;
    
    if (codec_type == "h264" || codec_type == "avc") {
        // H.264: NAL单元类型在第一个字节的低5位
        unsigned char nal_type = nal_unit[0] & 0x1F;
        return (nal_type == 5); // IDR slice
    } else if (codec_type == "h265" || codec_type == "hevc") {
        // H.265: NAL单元类型在第一个字节的高6位
        unsigned char nal_type = (nal_unit[0] >> 1) & 0x3F;
        return (nal_type >= 16 && nal_type <= 21); // IDR slices
    }
    
    return false;
}


AP4_Result H264H265ToFMP4Converter::StartEncode(const std::string& codec_type, int width, int height, double fps) {
    if (m_encoding_started) {
        return AP4_ERROR_INVALID_STATE;
    }
    
    m_codec_type = codec_type;
    m_width = width;
    m_height = height;
    m_fps = fps;
    m_sample_count = 0;
    m_total_duration = 0;
    // 创建Movie对象
    AP4_UI64 creation_time = 0;
    time_t now = time(NULL);
    if (now != (time_t)-1) {
        // adjust the time based on the MPEG time origin
        creation_time = (AP4_UI64)now + 0x7C25B080;
    }
    m_movie = new AP4_Movie(0, 0, creation_time, creation_time);
    
    m_encoding_started = true;
    return AP4_SUCCESS;
}


AP4_Result H264H265ToFMP4Converter::AddSample(const unsigned char* data, size_t size, bool is_keyframe, AP4_UI64& dts, AP4_UI64& cts) {
    (void)dts;
    (void)cts;
    if (!m_encoding_started) {
        std::cout<<"StartEncode failed"<<std::endl;
        return AP4_ERROR_INVALID_STATE;
    }
    
    std::vector<std::pair<const uint8_t*, size_t>> nalus;
    if (!parseNALU(data, size, nalus) || nalus.empty()) {
        std::cout<<"parseNALU failed"<<std::endl;
        return AP4_ERROR_INVALID_FORMAT;
    }
    

    
    // 如果还没有创建video track，并且我们有了必要的参数集，现在创建它
    if (!m_sample_table) {

        // 检测并保存参数集
        for (const auto& nal_unit : nalus) {
            if (nal_unit.second > 0) {
                unsigned char nal_type;
                if (m_codec_type == "h264" || m_codec_type == "avc") {
                    nal_type = nal_unit.first[0] & 0x1F;
                    if (nal_type == 7) { // SPS
                        m_sps_data.assign(nal_unit.first, nal_unit.first + nal_unit.second);
                    } else if (nal_type == 8) { // PPS
                        m_pps_data.assign(nal_unit.first, nal_unit.first + nal_unit.second);
                    }
                } else if (m_codec_type == "h265" || m_codec_type == "hevc") {
                    nal_type = (nal_unit.first[0] >> 1) & 0x3F;
                    if (nal_type == 32) { // VPS
                        m_vps_data.assign(nal_unit.first, nal_unit.first + nal_unit.second);
                        std::cout << "提取到 VPS，大小: " << nal_unit.second << " 字节" << std::endl;
                    } else if (nal_type == 33) { // SPS
                        m_sps_data.assign(nal_unit.first, nal_unit.first + nal_unit.second);
                        std::cout << "提取到 SPS，大小: " << nal_unit.second << " 字节" << std::endl;
                    } else if (nal_type == 34) { // PPS
                        m_pps_data.assign(nal_unit.first, nal_unit.first + nal_unit.second);
                        std::cout << "提取到 PPS，大小: " << nal_unit.second << " 字节" << std::endl;
                    }
                }
            }
        }


        bool has_required_params = false;
        if (m_codec_type == "h264" || m_codec_type == "avc") {
            has_required_params = !m_sps_data.empty() && !m_pps_data.empty();
        } else if (m_codec_type == "h265" || m_codec_type == "hevc") {
            has_required_params = !m_vps_data.empty() && !m_sps_data.empty() && !m_pps_data.empty();
        }
        
        if (has_required_params) {
            // 创建视频track
            m_sample_table = CreateSampleTable(m_codec_type, m_width, m_height);
            if (!m_sample_table) {
                Reset();
                return AP4_ERROR_INVALID_FORMAT;
            }
            
           
        }
    }
    
    // 如果还没有video track，跳过这个样本（可能是参数集不完整）
    if ( !m_sample_table) {
        return AP4_SUCCESS; // 不是错误，只是还没准备好
    }
    
    // 将所有NAL单元合并为一个访问单元（帧）
    std::vector<unsigned char> frame_data;
    bool frame_is_keyframe = is_keyframe;
    
    // 计算总大小并检查关键帧
    size_t total_size = 0;
    for (const auto& nal_unit : nalus) {
        total_size += 4 + nal_unit.second; // 4字节长度前缀 + NAL数据
        
        if (!is_keyframe) {
            std::vector<unsigned char> nal_data(nal_unit.first, nal_unit.first + nal_unit.second);
            if (IsKeyFrame(nal_data, m_codec_type)) {
                frame_is_keyframe = true;
            }
        }
    }
    
    // 预分配空间
    frame_data.reserve(total_size);
    
    // 将所有NAL单元转换为AVCC格式并合并
    for (const auto& nal_unit : nalus) {
        AP4_UI32 nal_size = nal_unit.second;
        
        // 添加4字节长度前缀（大端序）
        frame_data.push_back((nal_size >> 24) & 0xFF);
        frame_data.push_back((nal_size >> 16) & 0xFF);
        frame_data.push_back((nal_size >> 8) & 0xFF);
        frame_data.push_back(nal_size & 0xFF);
        
        // 添加NAL单元数据
        frame_data.insert(frame_data.end(), nal_unit.first, nal_unit.first + nal_unit.second);
    }
    
    // 创建持久化的数据缓冲区
    AP4_DataBuffer* persistent_data = new AP4_DataBuffer();
    persistent_data->SetData(frame_data.data(), frame_data.size());
    
    // 创建内存字节流
    AP4_MemoryByteStream* sample_stream = new AP4_MemoryByteStream(*persistent_data);
    
    // 使用正确的时间刻度计算时长（与track时间刻度一致）
     AP4_UI32 timescale = TIME_SCALE; // 标准视频时间刻度
     AP4_UI32 duration = static_cast<AP4_UI32>(timescale / m_fps);
     

    AP4_Result result = m_sample_table->AddSample(*sample_stream, 0, frame_data.size(), duration, 0, 0, 0, frame_is_keyframe);
    
    if (AP4_SUCCEEDED(result)) {
        m_sample_count++;
    }
    #ifdef mp4_debug
    std::cout<<"AddSample result:"<<result<<std::endl;
    #endif
    return result;
}

AP4_Result H264H265ToFMP4Converter::EndEncode(const std::string& output_file) {
    if (!m_encoding_started) {
        return AP4_ERROR_INVALID_STATE;
    }
    AP4_UI32 movie_timescale      = TIME_SCALE/m_fps;
    AP4_UI32 media_timescale      = TIME_SCALE;
    AP4_UI64 video_track_duration = AP4_ConvertTime(1000*m_sample_table->GetSampleCount(), media_timescale, movie_timescale);
    AP4_UI64 video_media_duration = 1000*m_sample_table->GetSampleCount();
    // 创建track
    m_video_track = new AP4_Track(AP4_Track::TYPE_VIDEO,
        m_sample_table,
        0, // track_id
        TIME_SCALE/m_fps, // movie_time_scale
        video_track_duration, // track_duration
        TIME_SCALE, // media_time_scale
        video_media_duration, // media_duration
        "und", // language
        m_width << 16, // width (16.16 fixed point)
        m_height << 16); // height (16.16 fixed point)

        // AP4_UI64 total_duration = 8; // header size
        // for (AP4_Cardinal i = 0; i < m_sample_count; i++) {
        //     AP4_Sample sample;
        //     m_video_track->GetSample(i, sample);
        //     total_duration += sample.GetDuration();
        // }

    // 计算正确的总时长
    // Track duration 基于 movie timescale (90000)
    //AP4_UI64 track_duration = static_cast<AP4_UI64>(m_sample_count * TIME_SCALE / m_fps);
    // Media duration 基于 media timescale (90000)
    // AP4_UI32 sample_duration = static_cast<AP4_UI32>(90000.0 / m_fps);
    // AP4_UI64 media_duration = static_cast<AP4_UI64>(m_sample_count) * sample_duration;

    //m_video_track->UseTrakAtom()->SetDuration(track_duration);
    
    // m_video_track->UseTrakAtom()->SetMediaDuration(media_duration);

    // 添加track到movie
    m_movie->AddTrack(m_video_track);
    m_movie->GetMvhdAtom()->SetNextTrackId(m_movie->GetTracks().ItemCount() + 1);

    // 创建输出流
    AP4_ByteStream* output_stream = nullptr;
    AP4_Result result = AP4_FileByteStream::Create(output_file.c_str(), 
                                                  AP4_FileByteStream::STREAM_MODE_WRITE, 
                                                  output_stream);
    if (AP4_FAILED(result)) {
        return result;
    }
    
    m_output_stream = output_stream;
    m_owns_output_stream = true;
    
    result = WriteInitializationSegment();
    if (AP4_FAILED(result)) {
        CleanupResources();
        return result;
    }
    
    
    CleanupResources();
    return result;
}

void H264H265ToFMP4Converter::Reset() {
    if (m_movie) {
        delete m_movie;
        m_movie = nullptr;
    }
    
    m_video_track = nullptr; // 由movie管理
    m_sample_table = nullptr; // 由track管理
    
    if (m_owns_output_stream && m_output_stream) {
        m_output_stream->Release();
    }
    m_output_stream = nullptr;
    m_owns_output_stream = false;
    
    m_encoding_started = false;
    m_sample_count = 0;
    m_total_duration = 0;
    m_codec_type.clear();
    
    // 清空参数集数据
    m_vps_data.clear();
    m_sps_data.clear();
    m_pps_data.clear();
}

AP4_SyntheticSampleTable* H264H265ToFMP4Converter::CreateSampleTable(const std::string& codec_type, int width, int height)
{
    AP4_SampleDescription* sample_desc = nullptr;
    
    if (codec_type == "h264" || codec_type == "avc") {
        // 创建H264样本描述
        AP4_Array<AP4_DataBuffer> sps_array;
        AP4_Array<AP4_DataBuffer> pps_array;
        
        // 如果有保存的参数集，使用真实的参数集
        if (!m_sps_data.empty()) {
            AP4_DataBuffer sps_buffer;
            sps_buffer.SetData(m_sps_data.data(), m_sps_data.size());
            sps_array.Append(sps_buffer);
        }
        
        if (!m_pps_data.empty()) {
            AP4_DataBuffer pps_buffer;
            pps_buffer.SetData(m_pps_data.data(), m_pps_data.size());
            pps_array.Append(pps_buffer);
        }
        
        // 如果没有参数集，使用默认的
        if (sps_array.ItemCount() == 0 || pps_array.ItemCount() == 0) {
            AP4_DataBuffer sps_buffer, pps_buffer;
            const unsigned char sample_sps[] = {
                0x67, 0x42, 0x00, 0x1e, 0x9a, 0x66, 0x14, 0x05, 0xff, 0xff, 0x01, 0x00, 0x01, 0x67, 0x42, 0x00, 0x1e, 0x9a, 0x66, 0x14, 0x05, 0xff, 0xff, 0x01, 0x00
            };
            const unsigned char sample_pps[] = {
                0x68, 0xce, 0x3c, 0x80
            };
            sps_buffer.SetData(sample_sps, sizeof(sample_sps));
            pps_buffer.SetData(sample_pps, sizeof(sample_pps));
            if (sps_array.ItemCount() == 0) sps_array.Append(sps_buffer);
            if (pps_array.ItemCount() == 0) pps_array.Append(pps_buffer);
        }
        
        AP4_AvccAtom* avcc = new AP4_AvccAtom(0x42, 0x00, 0x1e, 0, 0, 0, 4, sps_array, pps_array);
        
        sample_desc = new AP4_AvcSampleDescription(AP4_SAMPLE_FORMAT_AVC1,
                                                  width, height,
                                                  24,
                                                  "AVC Coding",
                                                  avcc);
    } else if (codec_type == "h265" || codec_type == "hevc") {
        // 创建H265样本描述
        AP4_Array<AP4_DataBuffer> vps_array;
        AP4_Array<AP4_DataBuffer> sps_array;
        AP4_Array<AP4_DataBuffer> pps_array;
        
        // 如果有保存的参数集，使用真实的参数集
        if (!m_vps_data.empty()) {
            AP4_DataBuffer vps_buffer;
            vps_buffer.SetData(m_vps_data.data(), m_vps_data.size());
            vps_array.Append(vps_buffer);
        }
        
        if (!m_sps_data.empty()) {
            AP4_DataBuffer sps_buffer;
            sps_buffer.SetData(m_sps_data.data(), m_sps_data.size());
            sps_array.Append(sps_buffer);
        }
        
        if (!m_pps_data.empty()) {
            AP4_DataBuffer pps_buffer;
            pps_buffer.SetData(m_pps_data.data(), m_pps_data.size());
            pps_array.Append(pps_buffer);
        }
        
        // 添加调试输出
        std::cout << "HEVC 参数集状态:" << std::endl;
        std::cout << "  VPS 数量: " << vps_array.ItemCount() << ", 大小: " << (m_vps_data.empty() ? 0 : m_vps_data.size()) << std::endl;
        std::cout << "  SPS 数量: " << sps_array.ItemCount() << ", 大小: " << (m_sps_data.empty() ? 0 : m_sps_data.size()) << std::endl;
        std::cout << "  PPS 数量: " << pps_array.ItemCount() << ", 大小: " << (m_pps_data.empty() ? 0 : m_pps_data.size()) << std::endl;
        
        // 如果没有参数集，使用更简单的默认参数集
        if (vps_array.ItemCount() == 0 || sps_array.ItemCount() == 0 || pps_array.ItemCount() == 0) {
            std::cout << "使用默认 HEVC 参数集" << std::endl;
            AP4_DataBuffer vps_buffer, sps_buffer, pps_buffer;
            
            // 更简单的 VPS (Main profile, level 3.1)
            const unsigned char sample_vps[] = {
                0x40, 0x01, 0x0C, 0x01, 0xFF, 0xFF, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00, 0x90, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x5D, 0x9C, 0x09
            };
            
            // 更简单的 SPS (1920x1080, Main profile)
            const unsigned char sample_sps[] = {
                0x42, 0x01, 0x01, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00, 0x90, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x5D, 0xA0, 0x02, 0x80, 0x80, 0x2D, 0x1F, 0x59, 0x99, 0xA4, 0x93, 0x2B, 0x80, 0x40, 0x00, 0x00, 0xFA, 0x00, 0x00, 0x17, 0x70, 0x02
            };
            
            // 更简单的 PPS
            const unsigned char sample_pps[] = {
                0x44, 0x01, 0xC1, 0x73, 0xD1, 0x89
            };
            
            vps_buffer.SetData(sample_vps, sizeof(sample_vps));
            sps_buffer.SetData(sample_sps, sizeof(sample_sps));
            pps_buffer.SetData(sample_pps, sizeof(sample_pps));
            if (vps_array.ItemCount() == 0) vps_array.Append(vps_buffer);
            if (sps_array.ItemCount() == 0) sps_array.Append(sps_buffer);
            if (pps_array.ItemCount() == 0) pps_array.Append(pps_buffer);
        }
        
        // 创建 HVCC 配置，使用更合适的参数
        AP4_HvccAtom* hvcc = new AP4_HvccAtom(0,    // general_profile_space
                                               0,    // general_tier_flag
                                               1,    // general_profile (Main = 1)
                                               0x60000000, // general_profile_compatibility_flags
                                               0x90000000, // general_constraint_indicator_flags
                                               120,   // general_level (3.1 = 93)
                                               0,    // min_spatial_segmentation
                                               0,    // parallelism_type
                                               1,    // chroma_format
                                               8,    // luma_bit_depth
                                               8,    // chroma_bit_depth
                                               0,    // average_frame_rate
                                               0,    // constant_frame_rate
                                               1,    // num_temporal_layers
                                               1,    // temporal_id_nested
                                               4,    // nalu_length_size
                                               vps_array, // video_parameters
                                               1,    // video_parameters_completeness
                                               sps_array, // sequence_parameters
                                               1,    // sequence_parameters_completeness
                                               pps_array, // picture_parameters
                                               1);   // picture_parameters_completeness
        
        std::cout << "创建 HVCC 原子完成" << std::endl;
        
        sample_desc = new AP4_HevcSampleDescription(AP4_SAMPLE_FORMAT_HVC1,
                                                   width, height,
                                                   24,
                                                   "HEVC Coding",
                                                   hvcc);
    } else {
        return nullptr;
    }
    
    // 创建sample table
    m_sample_table = new AP4_SyntheticSampleTable();
    m_sample_table->AddSampleDescription(sample_desc); 
    return m_sample_table;
}

AP4_Result H264H265ToFMP4Converter::WriteInitializationSegment() {
    if (!m_movie || !m_output_stream) {
        return AP4_ERROR_INVALID_STATE;
    }
    
    // 创建ftyp atom
    AP4_UI32 compatible_brands[] = {AP4_FTYP_BRAND_ISOM, AP4_FTYP_BRAND_MP42, AP4_FILE_BRAND_HVC1};
    
    AP4_FtypAtom ftyp(AP4_FTYP_BRAND_MP42, 1, compatible_brands, 3);
    ftyp.Write(*m_output_stream);
    
    
    //获取moov atom并添加mvex/trex
    AP4_MoovAtom* moov = m_movie->GetMoovAtom();
    if (moov) {
        AP4_Position position;
        m_output_stream->Tell(position);

        AP4_UI64 total_sample_size = 0;
        for (AP4_List<AP4_Track>::Item* track_item = m_movie->GetTracks().FirstItem();
                                        track_item;
                                        track_item = track_item->GetNext()) {
            AP4_Track*   track = track_item->GetData();
            AP4_Cardinal sample_count = track->GetSampleCount();
            AP4_Sample   sample;
            for (AP4_Ordinal i=0; i<sample_count; i++) {
                track->GetSample(i, sample);
                total_sample_size += sample.GetSize();
            }
        }

        AP4_UI64 mdat_size;
        bool mdat_is_large;
        if (total_sample_size <= 0xFFFFFFFF - AP4_ATOM_HEADER_SIZE) {
            mdat_size = AP4_ATOM_HEADER_SIZE;
            mdat_is_large = false;
        } else {
            mdat_size = 16;
            mdat_is_large = true;
        }

        AP4_UI64   mdat_position = position+moov->GetSize();
        unsigned int t=0;
        (void)t;
        AP4_Result result = AP4_SUCCESS;
        AP4_Array<AP4_Array<AP4_UI64>*> trak_chunk_offsets_backup;
        AP4_Array<AP4_UI64>             chunk_offsets;
        for (AP4_List<AP4_Track>::Item* track_item = m_movie->GetTracks().FirstItem();
                                        track_item;
                                        track_item = track_item->GetNext()) {
            AP4_Track*    track = track_item->GetData();
            AP4_TrakAtom* trak  = track->UseTrakAtom();
            
            // backup the chunk offsets
            AP4_Array<AP4_UI64>* chunk_offsets_backup = new AP4_Array<AP4_UI64>();
            trak_chunk_offsets_backup.Append(chunk_offsets_backup);
            result = trak->GetChunkOffsets(*chunk_offsets_backup);
            if (AP4_FAILED(result)) goto end;
    
            // allocate space for the new chunk offsets
            chunk_offsets.SetItemCount(chunk_offsets_backup->ItemCount());
            
            // compute the new chunk offsets
            AP4_Cardinal     sample_count = track->GetSampleCount();
            AP4_SampleTable* sample_table = track->GetSampleTable();
            AP4_Sample       sample;
            for (AP4_Ordinal i=0; i<sample_count; i++) {
                AP4_Ordinal chunk_index = 0;
                AP4_Ordinal position_in_chunk = 0;
                sample_table->GetSampleChunkPosition(i, chunk_index, position_in_chunk);
                sample_table->GetSample(i, sample);
                if (position_in_chunk == 0) {
                    // this sample is the first sample in a chunk, so this is the start of a chunk
                    if (chunk_index >= chunk_offsets.ItemCount()) return AP4_ERROR_INTERNAL;
                    chunk_offsets[chunk_index] = mdat_position+mdat_size;
                }
                mdat_size += sample.GetSize();
            }        
            result = trak->SetChunkOffsets(chunk_offsets);
        }

        // 写入moov
        moov->Write(*m_output_stream);

        if (mdat_is_large) {
            m_output_stream->WriteUI32(1);
            m_output_stream->WriteUI32(AP4_ATOM_TYPE_MDAT);
            m_output_stream->WriteUI64(mdat_size);
        } else {
            m_output_stream->WriteUI32((AP4_UI32)mdat_size);
            m_output_stream->WriteUI32(AP4_ATOM_TYPE_MDAT);
        }
        
        // write all tracks and restore the chunk offsets to their backed-up values
        for (AP4_List<AP4_Track>::Item* track_item = m_movie->GetTracks().FirstItem();
             track_item;
             track_item = track_item->GetNext(), ++t) {
            AP4_Track*    track = track_item->GetData();
            AP4_TrakAtom* trak  = track->UseTrakAtom();
            
            // restore the backed-up chunk offsets
            result = trak->SetChunkOffsets(*trak_chunk_offsets_backup[t]);
    
            // write all the track's samples
            AP4_Cardinal   sample_count = track->GetSampleCount();
            AP4_Sample     sample;
            AP4_DataBuffer sample_data;
            for (AP4_Ordinal i=0; i<sample_count; i++) {
                track->ReadSample(i, sample, sample_data);
                m_output_stream->Write(sample_data.GetData(), sample_data.GetDataSize());
            }
        }
        end:
        for (unsigned int i=0; i<trak_chunk_offsets_backup.ItemCount(); i++) {
            delete trak_chunk_offsets_backup[i];
        }
    }
    
    return AP4_SUCCESS;
}

void H264H265ToFMP4Converter::CleanupResources() {
    Reset();
}

