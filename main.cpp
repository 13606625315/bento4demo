#include "H264H265ToFMP4Converter.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <sys/stat.h>  // 包含必要的头文件
#include <unistd.h>
#include <sys/time.h>

#define ILOGE printf
#define ILOGW printf
#define ILOGD printf

#define APP_SYS_AV_AUDIO_FRAME_SIZE (640) ////音频一帧最大长度640，开发者根据自身的硬件来确定 16000*2/25=1280  8000*2/25=640 ak帧长:512(pcm),256(g711u)
#define APP_SYS_AV_VIDEO_FRAME_SIZE_100K (100 * 1024) //标清子码流最大100k

#define DHAV_HEAD_LENGTH                    (24)
#define DHAV_TAIL_LENGTH                    (8)
#define DHAV_EXTRA_OFFSET                   (22)
#define DHAV_CHECK_SUM_INDEX                (23)
#define VIDEO_MAIN_STREAM_QUEUE_SIZE        (32)
// #define h264

///帧类型
typedef enum
{
	I_FRAME_FLAG        = 0xFD,	///< I
	P_FRAME_FLAG        = 0xFC,	///< P
	B_FRAME_FLAG        = 0xFE,	///< B
	JPEG_FRAME_FLAG     = 0xFB,	///< JPEG
	AUDIO_FRAME_FLAG    = 0xF0, ///< AUDIO
	ASSISTANT_FLAG      = 0xF1,	///< 辅助帧,例如水印智能分析信息等
}DAHUA_FRAME_TYPE;

// 日期时间
typedef struct
{
	unsigned int second : 6;        //	秒	0-59
	unsigned int minute : 6;        //	分	0-59
	unsigned int hour   : 5;        //	时	0-23
	unsigned int day    : 5;        //	日	1-31
	unsigned int month  : 4;        //	月	1-12
	unsigned int year   : 6;        //	年	2000-2063
}DateTime;

// 大华标准帧头
typedef struct
{
	unsigned char frame_head_flag[4];	///>'D' 'H' 'A' 'V'
	unsigned char type;				    ///>帧类型，详见DAHUA_FRAME_TYPE定义
	unsigned char sub_type;			    ///>子类型，辅助帧才用到 0x01–调试信息,0x02-自定义信息....

	unsigned char channel_id;			///>通道号 通道表示回放需要的所有数据.每个通道可以包含1个视频+多个音频+多个辅助数据, 
                                        ///>如果他们的数据在同一个流中,他们的通道号必须填成一样,这样回放程序才能识别他们.
                                        //>通道号是一个相对数值,仅用于区分同一个流中的不同通道.

	unsigned char sub_frame_indx;		///>子帧序号 超长视频帧可以分成多个封装子帧，帧序号不变，子帧序号从大逐步递减到0
                                        //> 正常帧的子帧序号为0

	unsigned int frame_indx;			///>帧序号
	unsigned int frame_len;			    ///>帧长度，帧头+数据长度+帧尾
	DateTime  time;				        ///>时间日期
	unsigned short time_ms;			    ///>绝对时间戳
	unsigned char expand_len;			///>扩展字段长度
	unsigned char verify;				///>校验和，前23字节累加和
}__attribute__((packed)) DAHUA_FRAME_HEAD;


///扩展帧帧头标志位,参照《大华标准码流格式定义.pptx》svn@561961
typedef enum
{
    IMAGE_TYPE_FLAG				=	0x80,	///> 图像尺寸-1字段，4字节
    PLAY_BACK_TYPE_FLAG			=	0x81,	///> 回放类型字段，4字节
    IMAGE_H_TYPE_FLAG			=	0x82,   ///> 图像尺寸-2字段，8字节
    AUDIO_TYPE_FLAG				=	0x83,	///> 音频格式字段，4字节
    IVS_EXPAND_FLAG				=	0x84,	///> 智能扩展字段，8字节
    MODIFY_EXPAND_FLAG			=	0x85, 	///> 修定扩展字段，4字节
    DATA_VERIFY_DATA_FLAG		=	0x88,	///> 数据校验字段，8字节
    DATA_ENCRYPT_FLAG			=	0x89,	///> 数据加密字段，4字节
    FRACTION_FRAMERATE_FLAG 	=   0x8a,   ///> 扩展回放类型分数帧率字段，8字节
    STREAM_ROTATION_ANGLE_FLAG 	=   0x8b,   ///> 码流旋转角度字段，4字节
    AUDIO_TYPE_FLAG_EX			=	0x8c,	///> 扩展音频格式字段，指定长度

    METADATA_EXPAND_LEN_FLAG	=	0x90,	///> 元数据子帧长度扩展字段，8字节
    IMAGE_IMPROVEMENT_FLAG  	=   0x91,	///> 图像优化字段，8字节
    STREAM_MANUFACTURER_FLAG	=   0x92,	///> 码流厂商类型字段，8字节
    PENETRATE_FOG_FLAG      	=   0x93,	///> 偷雾模式标志字段，8字节
    SVC_FLAG					=	0x94,	///> SVC-T可伸缩视频编解码字段，4字节
    FRAME_ENCRYPT_FLAG			=	0x95,	///> 帧加密标志字段，8字节
    AUDIO_CHANNEL_FLAG			=	0x96,   ///> 音频通道扩展帧头标识字段，4字节
    PICTURE_REFORMATION_FLAG	=	0x97, 	///> 图像重组字段, 8+n*16字节
    DATA_ALIGNMENT_FLAG			=	0x98, 	///> 数据对齐字段，4字节
    IMAGE_MOSAIC_FLAG			=	0x99,   ///> 图像拼接扩展字段, 8+n*m*16字节
    FISH_EYE_FLAG           	=   0x9a,   ///> 鱼眼功能字段，8字节
    IMAGE_WH_RATIO_FLAG     	=   0x9b,   ///> 视频宽高比字段，8字节
    DIGITAL_SIGNATRUE_FLAG		=	0x9c,	///> 数字签名字段, 特殊处理

    ABSOLUTE_MILLISED_FLAG		=	0xa0,	///>  绝对毫秒时间字段
    NET_TRANSPORT_FLAG			=	0xa1,	///>  网络传输标识字段

    VEDIO_ENCRYPT_FRAME			=	0xb0, 	///> 录像加密帧字段
    OSD_STRING_FLAG         	=   0xb1,   ///> 码流OSD 字段
    GOP_OFFSET_FLAG         	=   0xb2,   ///> 解码偏移参考字段
    ENCYPT_CHECK_FLAG			=	0xb3,	///> 加密密钥校验字段
    SENSOR_JOIN_FLAG        	=   0xb4,   ///> 多目相机SENSOR 拼接字段
    STREAM_ENCRYPT_FLAG			=	0xb5,	///> 码流加密字段

    EXTERNHEAD_FLAG_RESERVED	=   0xff,	///> 大华扩展帧类型0xFF保留字段
}DAHUA_EXTERNHEAD_FLAG;

typedef struct
{
    uint8_t     type;               ///< 扩展帧标志 DAHUA_EXTERNHEAD_FLAG
    uint8_t 	encode;			    ///< 编码 0:编码时只有一场(帧) 1:编码时两场交织 2:编码时分两场
    uint8_t 	width;			    ///< 宽(8像素点为1单位)
    uint8_t     height;             ///< 高(8像素点为1单位)
}__attribute__((packed)) FRAME_EXTEND_IMAGE_SIZE1;

///视频编码格式
typedef enum
{
	MPEG4 = 1,
	H264 = 2,
	MPEG4_LB = 3,
	H264_GBE = 4,
	JPEG = 5,
	JPEG2000 = 6,
	AVS = 7,
	MPEG2= 9,
	VNC = 10,
	SVAC = 11,
	H265 = 12
}DAHUA_VIDEO_ENCODE_TYPE;

typedef struct
{
    uint8_t     type;               ///< 扩展帧标志 DAHUA_EXTERNHEAD_FLAG
    uint8_t 	interval;			///< I帧间隔(每多少帧一个I帧),取值范围1～255, 0表示老版本的码流
    uint8_t 	protocal;			///< 协议类型 见 DAHUA_VIDEO_ENCODE_TYPE
    uint8_t     fps;                ///< 帧率
}__attribute__((packed)) FRAME_EXTEND_PLAYBACK;

// 扩展帧头 - 数据校验
typedef struct
{
    unsigned char type;				    //0X88 表示校验信息
    unsigned char verify_result[4];     //校验结果
    unsigned char verify_method;		//保留，暂时没用
    unsigned char reserved2;		    //保留，暂时没用
    unsigned char verify_type;		    //校验类型， 目前为2： CRC32
} __attribute__((packed)) DATA_VERIFY;

// 大华帧尾
typedef struct
{
	unsigned char frame_tail_flag[4];	///>'D' 'H' 'A' 'V'
	unsigned int data_len;				///>数据长度，帧头+数据长度+帧尾
}__attribute__((packed)) DAHUA_FRAME_TAIL;


typedef struct
{
    int32_t frametype;
    int32_t usedSize;
    int64_t pts;
    unsigned char* frameBuff;
}VideoMsg;

static int32_t read_video_file(const char* path, char** fileBuf, int32_t* fileLen)
{
    if (path == NULL || fileBuf == NULL || fileLen == NULL)
    {
        ILOGW("[%s] input param err", __func__);
        return -1;
    }
    
    // 使用Linux stat检查文件是否存在
    struct stat st;
    if (stat(path, &st) != 0)
    {
        ILOGW("%s not exist\n", path);
        return -1;
    }

    FILE* file = fopen(path, "rb");
    if (file == NULL)
    {
        ILOGE("[%s] fopen err: %s", __func__, strerror(errno));
        return -1;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    uint32_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    ILOGD("[%s] file size = %d", __func__, fileSize);

    char* pTmpBuf = (char*)malloc(fileSize);
    if (pTmpBuf == NULL)
    {
        ILOGE("[%s] malloc err", __func__);
        fclose(file);
        return -1;
    }

    size_t readSize = fread(pTmpBuf, 1, fileSize, file);
    if (readSize != fileSize)
    {
        ILOGE("[%s] fread err: %s", __func__, strerror(errno));
        fclose(file);
        free(pTmpBuf);
        return -1;
    }

    *fileBuf = pTmpBuf;
    *fileLen = fileSize;
    fclose(file);
    return 0;
}

// 在 main.cpp 顶部添加辅助函数
bool ConvertToAnnexB(unsigned char* data, int size, std::vector<unsigned char>& output) {
    output.clear();
    
    #ifdef mp4_debug
    // 添加调试信息
    std::cout << "ConvertToAnnexB: size=" << size << ", first 8 bytes: ";
    for (int i = 0; i < std::min(8, size); i++) {
        printf("%02X ", data[i]);
    }
    std::cout << std::endl;
    #endif
    // 检查是否已经是 Annex-B 格式（以 0x00000001 开头）
    if (size >= 4 && data[0] == 0x00 && data[1] == 0x00 && data[2] == 0x00 && data[3] == 0x01) {
        #ifdef mp4_debug
        std::cout << "Already Annex-B format" << std::endl;
        #endif
        output.assign(data, data + size);
        return true;
    }
    
    // 检查是否是 3字节起始码格式（0x000001）
    if (size >= 3 && data[0] == 0x00 && data[1] == 0x00 && data[2] == 0x01) {
        std::cout << "3-byte start code format" << std::endl;
        output.assign(data, data + size);
        return true;
    }
    
    // 检查是否是 AVCC 格式（以长度前缀开头）
    if (size >= 4) {
        int pos = 0;
        bool is_avcc = true;
        
        while (pos < size && is_avcc) {
            if (pos + 4 > size) {
                is_avcc = false;
                break;
            }
            
            // 读取 NAL 单元长度（大端序）
            int nal_length = (data[pos] << 24) | (data[pos+1] << 16) | (data[pos+2] << 8) | data[pos+3];
            
            if (nal_length <= 0 || nal_length > 1000000 || pos + 4 + nal_length > size) {
                is_avcc = false;
                break;
            }
            
            pos += 4 + nal_length;
        }
        
        if (is_avcc && pos == size) {
            std::cout << "AVCC format detected, converting..." << std::endl;
            pos = 0;
            while (pos < size) {
                int nal_length = (data[pos] << 24) | (data[pos+1] << 16) | (data[pos+2] << 8) | data[pos+3];
                
                // 添加 Annex-B 起始码
                output.push_back(0x00);
                output.push_back(0x00);
                output.push_back(0x00);
                output.push_back(0x01);
                
                // 添加 NAL 单元数据
                output.insert(output.end(), data + pos + 4, data + pos + 4 + nal_length);
                
                pos += 4 + nal_length;
            }
            return true;
        }
    }
    
    // 如果无法识别格式，检查是否包含 NAL 单元类型
    std::cout << "Unknown format, adding start code prefix" << std::endl;
    
    // 添加 Annex-B 起始码
    output.push_back(0x00);
    output.push_back(0x00);
    output.push_back(0x00);
    output.push_back(0x01);
    output.insert(output.end(), data, data + size);
    
    return true;
}
static int32_t dahua_head_check_sum(char* head, uint8_t sum)
{
    uint8_t calSum = 0;
    for (int32_t i = 0; i < 23; i++)
    {
        calSum += head[i];
    }
    if (calSum != sum)
    {
        ILOGE("[%s] src[0x%x] dst[0x%x]", __func__, sum, calSum);
        return false;
    }
    return true;
}

// 获取ms 时间
static unsigned long __get_time_ms()
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) < 0) {
        return 0;
    }
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void PrintUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [options] <input_file> <output_file> <codec_type>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --step-by-step    Use step-by-step encoding interface" << std::endl;
    std::cout << "  --width <width>   Video width (default: 1920)" << std::endl;
    std::cout << "  --height <height> Video height (default: 1080)" << std::endl;
    std::cout << "codec_type: h264 or h265" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  " << program_name << " input.264 output.mp4 h264" << std::endl;
    std::cout << "  " << program_name << " --step-by-step --width 1280 --height 720 input.265 output.mp4 h265" << std::endl;
}

std::vector<unsigned char> ReadFileData(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return std::vector<unsigned char>();
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        return std::vector<unsigned char>();
    }
    
    return buffer;
}



int ConvertWithStepByStep(const std::string& input_file, const std::string& output_file, 
                         const std::string& codec_type, int width, int height) {
    (void)input_file; // 标记为未使用以避免警告
    std::cout << "Using step-by-step encoding interface..." << std::endl;
    
    H264H265ToFMP4Converter converter;
    
    // 步骤1: 开始编码（在这里确定帧率）
    std::cout << "Step 1: Starting encode..." << std::endl;
    const double fps = 24.0; // 在start时确定帧率
    AP4_Result result = converter.StartEncode(codec_type, width, height, fps);
    if (AP4_FAILED(result)) {
        std::cerr << "Failed to start encoding: " << result << std::endl;
        return 1;
    }
    
    char* fileBuf = NULL;
    int32_t fileLen = 0;
    char path[] = "./v_demo.dav";
    int32_t ret = read_video_file(path, &fileBuf, &fileLen);
    if (ret) {
        ILOGE("[%s] read_video_file err", __func__);
        usleep(1000 * 1000);
        return false;
    }

    AP4_UI64 dts = 0;
    AP4_UI64 cts = 0;
    const AP4_UI64 frame_duration = static_cast<AP4_UI64>(TIME_SCALE / fps); // 根据fps计算帧间隔(90000时间刻度)

    char* pTmpHead = fileBuf;
    // 在 ConvertWithStepByStep 函数中，替换现有的 AddSample 调用
    while (1) {
        if (!(pTmpHead[0] == 'D' && pTmpHead[1] == 'H' && pTmpHead[2] == 'A' && pTmpHead[3] == 'V')) {
            ILOGE("[%s] invalid frame", __func__);
            break;
        }
        
        DAHUA_FRAME_HEAD* head = (DAHUA_FRAME_HEAD*)pTmpHead;
        
        if (dahua_head_check_sum((char*)head, head->verify) == false) {
            ILOGE("[%s] dahua_head_check_sum", __func__);
            break;
        }
        
        int32_t data_length = head->frame_len - DHAV_HEAD_LENGTH - DHAV_TAIL_LENGTH - head->expand_len;
        int32_t data_offset = DHAV_HEAD_LENGTH + head->expand_len;
        
        VideoMsg msg;
        memset(&msg, 0, sizeof(msg));
        msg.frametype = (head->type == I_FRAME_FLAG) ? 1 : 0;
        msg.usedSize = data_length;
        msg.pts = static_cast<int64_t>(__get_time_ms());
        msg.frameBuff = (unsigned char*)(pTmpHead + data_offset);
        
        #ifdef mp4_debug
        // 在处理每一帧时添加调试信息
        std::cout << "Processing frame: type=" << msg.frametype 
                  << ", size=" << msg.usedSize 
                  << ", first 8 bytes: ";
        for (int i = 0; i < std::min(8, msg.usedSize); i++) {
            printf("%02X ", msg.frameBuff[i]);
        }
        std::cout << std::endl;

        // 检查数据格式并转换为 Annex-B 格式
        std::vector<unsigned char> annexb_data;
        if (!ConvertToAnnexB(msg.frameBuff, msg.usedSize, annexb_data)) {
            std::cerr << "Failed to convert frame data to Annex-B format" << std::endl;
            continue;
        }
        result = converter.AddSample(annexb_data.data(), annexb_data.size(), msg.frametype, dts, cts);
        #else     
        // 写入转换后的帧数据
        result = converter.AddSample(msg.frameBuff, msg.usedSize, msg.frametype, dts, cts);
        #endif  
        pTmpHead += head->frame_len;
        dts += frame_duration;
        cts += frame_duration;
        if (AP4_FAILED(result)) {
            std::cerr << "Failed to add sample chunk at offset " << data_offset << ": " << result << std::endl;
            continue;
        }
        


    }
    
    if (fileBuf) {
        free(fileBuf);
        fileBuf = NULL;
    }
    
    // 步骤3: 处理数据（支持分段处理，模拟网络数据）
  
    
    std::cout << "Processed all data chunks successfully." << std::endl;
    
    // 步骤4: 结束编码并输出文件
    std::cout << "Step 4: Ending encode and writing output..." << std::endl;
    result = converter.EndEncode(output_file);
    if (AP4_FAILED(result)) {
        std::cerr << "Failed to end encoding: " << result << std::endl;
        return 1;
    }
    
    std::cout << "Step-by-step conversion completed successfully!" << std::endl;
    return 0;
}



int main(int argc, char* argv[]) {
    if (argc < 4) {
        PrintUsage(argv[0]);
        return 1;
    }
    
    // 解析命令行参数
    bool step_by_step = false;
    int width = 1920;
    int height = 1080;
    std::string input_file;
    std::string output_file;
    std::string codec_type;
    
    int arg_index = 1;
    while (arg_index < argc) {
        std::string arg = argv[arg_index];
        
        if (arg == "--step-by-step") {
            step_by_step = true;
            arg_index++;
        } else if (arg == "--width" && arg_index + 1 < argc) {
            width = std::atoi(argv[arg_index + 1]);
            arg_index += 2;
        } else if (arg == "--height" && arg_index + 1 < argc) {
            height = std::atoi(argv[arg_index + 1]);
            arg_index += 2;
        } else {
            // 非选项参数，应该是输入文件、输出文件和编码类型
            if (input_file.empty()) {
                input_file = arg;
            } else if (output_file.empty()) {
                output_file = arg;
            } else if (codec_type.empty()) {
                codec_type = arg;
            } else {
                std::cerr << "Too many arguments" << std::endl;
                PrintUsage(argv[0]);
                return 1;
            }
            arg_index++;
        }
    }
    
    // 验证参数
    if (input_file.empty() || output_file.empty() || codec_type.empty()) {
        std::cerr << "Missing required arguments" << std::endl;
        PrintUsage(argv[0]);
        return 1;
    }
    
    if (codec_type != "h264" && codec_type != "avc" && codec_type != "h265" && codec_type != "hevc") {
        std::cerr << "Unsupported codec type: " << codec_type << std::endl;
        std::cerr << "Supported types: h264, avc, h265, hevc" << std::endl;
        return 1;
    }
    
    if (width <= 0 || height <= 0) {
        std::cerr << "Invalid video dimensions: " << width << "x" << height << std::endl;
        return 1;
    }
    
    std::cout << "Input file: " << input_file << std::endl;
    std::cout << "Output file: " << output_file << std::endl;
    std::cout << "Codec type: " << codec_type << std::endl;
    std::cout << "Video dimensions: " << width << "x" << height << std::endl;
    std::cout << "Mode: " << (step_by_step ? "Step-by-step" : "One-step") << std::endl;
    std::cout << std::endl;
    
    // 执行转换

    return ConvertWithStepByStep(input_file, output_file, codec_type, width, height);

}
