/*******************************************************************************
* devVideoEnc.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  wang_zhengxue <wang_zhengxue@dahuatech.com>
* Version:  V1.0.0  2022-01-24 Create
* Description: Binder Parcel Interface
*
*       1.  硬件说明。
*
*       2.  程序结构说明。
*
*       3.  使用说明。
*
*       4.  局限性说明。
*
*       5.  其他说明。
*           无
*
* Modification:
*    Date    :
*    Revision:
*    Author  :
*    Contents:
*******************************************************************************/

#ifndef __IMOUOS_MEDIA_DEFS_H__
#define __IMOUOS_MEDIA_DEFS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"

/// 通用图像大小类型
typedef enum ImageSizeType
{
    imageSizeD1 = 0,    ///< 704*576(PAL)   704*480(NTSC)
    imageSizeHD1,       ///< 352*576(PAL)   352*480(NTSC)
    imageSizeBCIF,      ///< 704*288(PAL)   704*240(NTSC)
    imageSizeCIF,       ///< 352*288(PAL)   352*240(NTSC)
    imageSizeQCIF,      ///< 176*144(PAL)   176*120(NTSC)
    imageSizeVGA,       ///< 640*480
    imageSizeQVGA,      ///< 320*240
    imageSizeSVCD,      ///< 480*480
    imageSizeQQVGA,     ///< 160*128
    imageSizeSVGA,      ///< 800*592
    imageSizeXVGA,      ///< 1024*768
    imageSizeWXGA,      ///< 1280*800
    imageSizeSXGA,      ///< 1280*1024
    imageSizeWSXGA,     ///< 1600*1024
    imageSizeUXGA,      ///< 1600*1200
    imageSizeWUXGA,     ///< 1920*1200
    imageSizeND1,       ///< 240*192
    imageSize720P,      ///< 1280*720
    imageSize1080P,     ///< 1920*1080
    imageSize1_3M,      ///< 1280*960(1.3M)
    imageSize1280_960 = imageSize1_3M,
    imageSize2_5M,      ///< 1872*1408(2.5M)
    imageSize1872_1408 = imageSize2_5M,
    imageSize5M,        ///< 3744*1408(5M)
    imageSize3744_1408 = imageSize5M,
    imageSize3M,        ///< 2048*1536(3M)
    imageSize2048_1536 = imageSize3M,
    imageSize2432_2048, ///< 2432*2048(5M)
    imageSize1216_1024, ///< 1216*1024(1.2M)
    imageSize1408_1024, ///< 1408*1024(1.5M)
    imageSize3296_2472, ///< 3296*2472(8M)
    imageSize2560_1920, ///< 2560*1920(5M)
    imageSize960H,      ///< 928*576(PAL) 928*480(NTSC)
    imageSize960_720,   ///< 960*720(DV720P)
    imageSize2560_1600, ///< 2560*1600(4M)
    imageSize2336_1752, ///< 2336_1752(4_0M)
    imageSize2592_2048, ///< 2592*2048
    imageSize1920_1440, ///< 1920*1440
    imageSize2448_2048, ///< 2448*2048
    imageSize2752_2208, ///< 2752*2208
    imageSizeWVGA,      ///< 800*480
    imageSize3840_2160, ///< 3840*2160(8_0M)
    imageSize4096_2160, ///< 4096*2160(8_8M)
    imageSize3072_2048, ///< 3072*2048(6_3M)
    imageSize2048_2048, ///< 2048*2048(4M)
    imageSize2304_1296, ///< 2304*1296,
    imageSize4000_3000, ///< 4000*3000(12M)
    imageSize2880_2880, ///< 2880*2880(9M)
    imageSize2880_2160, ///< 2880*2160(6M)
    imageSize640_360,   ///< 640*360
    imageSize320_180,   ///< 320*180
    imageSize3648_2736, ///< 3648*2736
    imageSize2592_1520, ///< 2592*1520(4_1M)
    imageSize2560_2048, ///< 2560*2048
    imageSize1920_1088, ///< 1920*1088
    imageSize2560_1440, ///< 2560*1440
    imageSize2048_1520, ///< 2048*1520(3M)
    imageSize2592_2592, ///< 2592*2592(6M)
    imageSize3392_2008, ///< 3392*2008
    imageSize2592_1944, ///< 2592*1944
    imageSize640_512,   ///< 640*512
    imageSize320_256,   ///< 320*256
    imageSize2688_1520, ///<2688*1520
    imageSize2912_2336, ///<2912*2336
    imageSize960_540,   ///< 960*540
    imageSize800_448,   ///< 800*448
    imageSize720_404,   ///< 720*404
    imageSize480_272,   ///< 480*272
    imageSize320_320,   ///< 320*320
    imageSize352_360,   ///< 352*360
    imageSize480_480,   ///< 480*480
    imageSize512_256,   ///< 512*256
    imageSize512_512,   ///< 512*512
    imageSize576_576,   ///< 576*576
    imageSize640_320,   ///< 640*320
    imageSize640_640,   ///< 640*640
    imageSize704_256,   ///< 704*256
    imageSize704_320,   ///< 704*320
    imageSize720_360,   ///< 720*360
    imageSize720_720,   ///< 720*720
    imageSize768_768,   ///< 768*768
    imageSize960_480,   ///< 960*480
    imageSize960_960,   ///< 960*960
    imageSize1024_512,  ///< 1024*512
    imageSize1024_1024, ///< 1024*1024
    imageSize1152_576,  ///< 1152*576
    imageSize1152_1152, ///< 1152*1152
    imageSize1216_1216, ///< 1216*1216
    imageSize1280_480,  ///< 1280*480
    imageSize1440_360,  ///< 1440*360
    imageSize1536_768,  ///< 1536*768
    imageSize1536_1536, ///< 1536*1536
    imageSize1920_960,  ///< 1920*960
    imageSize1920_1920, ///< 1920*1920
    imageSize2048_512,  ///< 2048*512
    imageSize2144_1072, ///< 2144*1072
    imageSize2304_576,  ///< 2304*576
    imageSize2304_1152, ///< 2304*1152
    imageSize2304_2304, ///< 2304*2304
    imageSize2432_1216, ///< 2432*1216
    imageSize2432_2432, ///< 2432*2432
    imageSize3072_768,  ///< 3072*768
    imageSize3840_960,  ///< 3840*960
    imageSize4288_1072, ///< 4288*1072
    imageSize4608_1152, ///< 4608*1152
    imageSize4864_1216, ///< 4864*1216
    imageSize1072_1072, ///< 1072*1072
    imageSize2144_2144, ///< 2144*2144
    imageSize336_256,   ///< 336*256
    imageSize960_1080,  ///< 960*1080
    imageSize4096_832,  ///< 4096*832
    imageSize3840_780,  ///< 3840*780
    imageSize2560_520,  ///< 2560*520
    imageSize1024_208,  ///< 1024*208
    imageSize1280_260,  ///< 1280*260
    imageSize720_576,   ///< 720*576
    imageSize4096_1800, ///< 4096*1800
    imageSize3840_1680, ///< 3840*1680
    imageSize2880_1264, ///< 2880*1264
    imageSize1024_452,  ///< 1024*452
    imageSize1920_832,  ///< 1920*832
    imageSize1280_560,  ///< 1280*560
    imageSize3072_1728, ///< 3072*1728
    imageSize432_240,   ///< 432*240
    imageSize784_440,   ///< 784*440
    imageSize2544_1920, ///< 2544*1920
    imageSize3008_2264, ///< 3008*2264
    imageSize4096_1024, ///< 4096*1024
    imageSize704_288,   ///< 704*288
    imageSize1408_576,  ///< 1408*576
    imageSize2560_720,  ///< 2560*720
    imageSize2560_960,  ///< 2560*960
    imageSize2560_1024, ///< 2560*1024
    imageSize3840_1080, ///< 3840*1080
    imageSize1936_1452, ///< 1936*1452
    imageSize265_192,   ///< 256*192
    imageSize400_300,   ///< 400*300
    imageSize480_640,   ///< 480*640(热成像-手持测温仪)
    imageSize4K,        ///< 4K分辨率
    imageSize8K,        ///< 8K分辨率
    imageSize2880_1620, ///< 2880*1620
    imageSize4K_4_3,    ///< [4K分辨率4:3] [2880 - 2160] [2880x2160]
    imageSize4K_3_2,    ///< [4K分辨率3:2] [3240 - 2160] [3240x2160]
    imageSize4K_17_9,   ///< [4K分辨率17:9] [3840 - 2032] [3840x2032]
    imageSize720_540,   ///< [720*540热成像手持望远镜] [720 - 540] [720x540]
    imageSize1440_1080, ///< [DB60分辨率4:3][1440 - 1080][1440*1080]
    imageSize2400_1800, ///< [DB60分辨率4:3][2400 - 1800][2400*1800]
    imageSize720_480,   ///< 720*480
    imageSize704_480,   ///< 704*480
    imageSizeNR,        ///< 枚举的图形大小种类的数目。
    /***************************评审规则**********************************
     1、新增分辨率枚举说明格式[分辨率说明][实际宽-实际高][分辨率名称]
        如：[4K分辨率][3840-2160]["3840x2160"]
     2、新增分辨率时注意同步修改ImageSize.c文件中的customImagesizeTable、imageSize
        数据结构，为其同步新增分辨宽高、名称；对于ImageSize.c中遗漏的分辨率及名称应
        主动根据说明补齐ImageSize.c中的相关数据，保证        数据的一致性
    *********************************************************************/
}ImageSizeType;


/// DHAV帧类型
typedef enum EFrameType
{
    EFrameType_smartIFrame  = 0x01, /// Smart编码(长GOP)重定位I帧，作为当前GOP所有重定位P帧的参考
    EFrameType_smartPFrame  = 0x02, /// 长GOP编码(长GOP)重定位P帧，只参考当前GOP的I帧
    EFrameType_audioFrame   = 0x41, /// 音频帧
    EFrameType_IFrame       = 0x49, /// 视频I帧
    EFrameType_PFrame       = 0x50, /// 视频P帧
    EFrameType_BFrame       = 0x42, /// 视频B帧
    EFrameType_GPSFrame     = 0x47, /// GPS辅助帧
    EFrameType_motionFrame  = 0x4D, /// 视频动检帧，MediaFrameInfo.type='V'
    EFrameType_metadataFrame= 0x4D, /// 元数据辅助帧，MediaFrameInfo.type='X'
    EFrameType_SmartFrame   = 0x53, /// 智能分析帧
    EFrameType_WaterFrame   = 0x57, /// 水印帧
}EFrameType;

typedef struct FrameInfo
{
    Int8 type;          ///< 'A'/'V'/'X', 'X'(辅助帧)
    Int8 head;          ///< 0/1, if include a frame head
    Int8 tail;          ///< 0/1, if include a frame tail
    Int8 newFormat;     ///< for encode format params, 0-none changed, 1-resolution changed, 2-other changed.
    Int32 channel;  ///< video channel or audio channel
    Uint64 pts;     ///< presentation time stamp, in millisecond
    union
    {
        struct
        {
            Uint8 group;    ///< video group mode, see IDevVideoEnc::GroupMode
            Uint8 stream;   ///< video stream type, see IDevVideoEnc::StreamType
            Uint8 type; ///< video frame type, 'I','B','P',(为兼容老版本,以下类型保留:'W','G','M')
            Uint8 mark; ///< 1-255, mark of specified frame, 0-ignore
            Uint8 spec; ///< if include a codec spec info such as PPS/SPS of H.264 codec
            Uint8 encode; ///< stream encode type, 见：enum EncodeType
            Uint8 svcLayer; ///svc层数
        } video;
        struct
        {
            Uint8 stream;       ///< audio stream type, see IDevAudioEnc::StreamType
        } audio;
        struct
        {
            Uint8 type;     ///< 辅助帧类型，'S'(智能分析帧),'W'(Water),'G'(GPS),'M'(Metadata), 'D'(全屏动检数据-标准模式)
            Uint8 stream;       ///< stream type, see IDevVideoEnc::StreamType
        }extra;                 ///< 辅助帧信息
        Int8 placeholder[8];    ///< placeholder for 8 bytes
    } detail;
    Uint32 length;          ///< length of a completed frame
    Uint32 utc;             ///< seconds from utc(1970-1-1 00:00:00)

    // 以下数据放在CPacket头32字节以后，前提是CPacket头有足够长度，如果没有则不能使用
    Uint64 ptsBeforeEnc;    ///< 音视频帧编码前的时间戳
    Uint64 ptsAfterEnc; ///< 音视频帧编码后的时间戳
    Uint64 ptsLib;      ///< 编码库从驱动取到本数据包的时间戳
    Uint64 ptsApp;      ///< 应用层取到本数据包后的时间戳
    Uint64 ptsNet;      ///< 网络模块取到本数据包后的时间戳
    Uint32 sequence;        ///< 音视频帧帧序号
    Uint16 utcms;           ///< UTC对应毫秒数
    Uint16 reserved1;       ///< 保留
    Uint32 encodeSequence;///< 合成图片编码序号（由应用生成，用于编码完匹配使用）,非合成图片填0
    Uint32 reserved[11];    ///< 保留
}FrameInfo;

typedef struct FrameStatInfo
{
    Uint32 frameNumber;
    Uint32 totalSize;
    Uint32 maxSize;
    Uint32 curSize;
}FrameStatInfo;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __IMOUOS_MEDIA_DEFS_H__ */
