/*******************************************************************************
* mediaTypes.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  longjie <long_jie@dahuatech.com>
* Version:  V1.0.0  2022-05-24 Create
* Description: meida模块公共音视频类型
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

#ifndef __MEDIA_AUDIOVIDEOTYPE_H__
#define __MEDIA_AUDIOVIDEOTYPE_H__
#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"
#include "OSA/Error.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/// Media内存数据，使用Packet系列函数解析，扩展数据见FrameInfo
typedef imouos_handle media_pkt;

/// YUV数据
typedef struct _MediaYuvPacket_
{
    Uint32              format;             ///< YUV 格式描述，参见 DH_PAL_VideoDataFormat
    Uint32              u32Width;           ///< [in]指定获取YUV数据的宽,[out]系统输出YUV数据的宽
    Uint32              u32Height;          ///< [in]指定获取YUV数据的高,[out]系统输出YUV数据的高

    void                *virAddrY;          ///< 可直接访问,代表Y分量
    void                *virAddrU;          ///< 可直接访问,代表U分量
    void                *virAddrV;          ///< 可直接访问,代表V分量
    Uint32              u32Stride[3];       ///< 帧访问的跨距,行与行之间的间距,单位像素
    Uint32L             phyAddrY;           ///< Y分量物理地址
    Uint32L             phyAddrU;           ///< U分量物理地址
    Uint32L             phyAddrV;           ///< V分量物理地址
    Uint64              u64Seq;             ///< yuv的序号
    Uint64              u64Pts;             ///< YUV原始时间戳，和编码帧之前的yuv同源
    void                *priv;              ///< 底层私有指针,释放时需要使用
    Uint32L             res[3];
}MediaYuvPacket;

/// YUV数据功能
typedef enum EMeidaYuvFuncType
{
    MEDIA_YUV_INTELLI_0,           ///< 通用智能0
    MEDIA_YUV_INTELLI_1,           ///< 通用智能1
    MEDIA_YUV_INTELLI_2,           ///< 通用智能2
    MEDIA_YUV_INTELLI_3,           ///< 通用智能3
    MEDIA_YUV_MAX,
}EMeidaYuvFuncType;

///<  VIDEO数据格式
typedef enum IMOUOS_VideoDataFormat
{
    IMOUOS_DF_YUV422I_UYVY = 0x00,
    /// YUV 422 Interleaved format - UYVY.
    IMOUOS_DF_YUV422I_YUYV,
    /// YUV 422 Interleaved format - YUYV.
    IMOUOS_DF_YUV422I_YVYU,
    /// YUV 422 Interleaved format - YVYU.
    IMOUOS_DF_YUV422I_VYUY,
    /// YUV 422 Interleaved format - VYUY.
    IMOUOS_DF_YUV422SP_UV,
    /// YUV 422 Semi-Planar - Y separate, UV interleaved.
    IMOUOS_DF_YUV422SP_VU,
    /// YUV 422 Semi-Planar - Y separate, VU interleaved.
    IMOUOS_DF_YUV422P,
    /// YUV 422 Planar - Y, U and V separate.
    IMOUOS_DF_YUV420SP_UV,
    /// YUV 420 Semi-Planar - Y separate, UV interleaved.
    IMOUOS_DF_YUV420SP_VU,
    /// YUV 420 Semi-Planar - Y separate, VU interleaved.
    IMOUOS_DF_YUV420P,
    /// YUV 420 Planar - Y, U and V separate.
    IMOUOS_DF_YUV444P,
    /// YUV 444 Planar - Y, U and V separate.
    IMOUOS_DF_YUV444I,
    /// YUV 444 interleaved - YUVYUV...

    IMOUOS_DF_RGB16_565 = 0x20,
    /// RGB565 16-bit - 5-bits R, 6-bits G, 5-bits B.
    IMOUOS_DF_ARGB16_1555,
    /// ARGB1555 16-bit - 5-bits R, 5-bits G, 5-bits B, 1-bit Alpha (MSB).
    IMOUOS_DF_RGBA16_5551,
    /// RGBA5551 16-bit - 5-bits R, 5-bits G, 5-bits B, 1-bit Alpha (LSB).
    IMOUOS_DF_ARGB16_4444,
    /// ARGB4444 16-bit - 4-bits R, 4-bits G, 4-bits B, 4-bit Alpha (MSB).
    IMOUOS_DF_RGBA16_4444,
    /// RGBA4444 16-bit - 4-bits R, 4-bits G, 4-bits B, 4-bit Alpha (LSB).
    IMOUOS_DF_ARGB24_6666,
    /// ARGB6666 24-bit - 6-bits R, 6-bits G, 6-bits B, 6-bit Alpha (MSB).
    IMOUOS_DF_RGBA24_6666,
    /// RGBA6666 24-bit - 6-bits R, 6-bits G, 6-bits B, 6-bit Alpha (LSB).
    IMOUOS_DF_RGB24_888,
    /// RGB24 24-bit - 8-bits R, 8-bits G, 8-bits B.
    IMOUOS_DF_ARGB32_8888,
    /// ARGB32 32-bit - 8-bits R, 8-bits G, 8-bits B, 8-bit Alpha (MSB).
    IMOUOS_DF_RGBA32_8888,
    /// RGBA32 32-bit - 8-bits R, 8-bits G, 8-bits B, 8-bit Alpha (LSB).
    IMOUOS_DF_BGR16_565,
    /// BGR565 16-bit -   5-bits B, 6-bits G, 5-bits R.
    IMOUOS_DF_ABGR16_1555,
    /// ABGR1555 16-bit - 5-bits B, 5-bits G, 5-bits R, 1-bit Alpha (MSB).
    IMOUOS_DF_ABGR16_4444,
    /// ABGR4444 16-bit - 4-bits B, 4-bits G, 4-bits R, 4-bit Alpha (MSB).
    IMOUOS_DF_BGRA16_5551,
    /// BGRA5551 16-bit - 5-bits B, 5-bits G, 5-bits R, 1-bit Alpha (LSB).
    IMOUOS_DF_BGRA16_4444,
    /// BGRA4444 16-bit - 4-bits B, 4-bits G, 4-bits R, 4-bit Alpha (LSB).
    IMOUOS_DF_ABGR24_6666,
    /// ABGR6666 24-bit - 6-bits B, 6-bits G, 6-bits R, 6-bit Alpha (MSB).
    IMOUOS_DF_BGR24_888,
    /// BGR888 24-bit - 8-bits B, 8-bits G, 8-bits R.
    IMOUOS_DF_ABGR32_8888,
    /// ABGR8888 32-bit - 8-bits B, 8-bits G, 8-bits R, 8-bit Alpha (MSB).
    IMOUOS_DF_BGRA24_6666,
    /// BGRA6666 24-bit - 6-bits B, 6-bits G, 6-bits R, 6-bit Alpha (LSB).
    IMOUOS_DF_BGRA32_8888,
    /// BGRA8888 32-bit - 8-bits B, 8-bits G, 8-bits R, 8-bit Alpha (LSB).

    IMOUOS_DF_BITMAP8 = 0x40,
    /// BITMAP 8bpp.
    IMOUOS_DF_BITMAP4_LOWER,
    /// BITMAP 4bpp lower address in CLUT.
    IMOUOS_DF_BITMAP4_UPPER,
    /// BITMAP 4bpp upper address in CLUT.
    IMOUOS_DF_BITMAP2_OFFSET0,
    /// BITMAP 2bpp offset 0 in CLUT.
    IMOUOS_DF_BITMAP2_OFFSET1,
    /// BITMAP 2bpp offset 1 in CLUT.
    IMOUOS_DF_BITMAP2_OFFSET2,
    /// BITMAP 2bpp offset 2 in CLUT.
    DH_PAL_DF_BITMAP2_OFFSET3,
    /// BITMAP 2bpp offset 3 in CLUT.
    IMOUOS_DF_BITMAP1_OFFSET0,
    /// BITMAP 1bpp offset 0 in CLUT.
    IMOUOS_DF_BITMAP1_OFFSET1,
    /// BITMAP 1bpp offset 1 in CLUT.
    IMOUOS_DF_BITMAP1_OFFSET2,
    /// BITMAP 1bpp offset 2 in CLUT.
    IMOUOS_DF_BITMAP1_OFFSET3,
    /// BITMAP 1bpp offset 3 in CLUT.
    IMOUOS_DF_BITMAP1_OFFSET4,
    /// BITMAP 1bpp offset 4 in CLUT.
    IMOUOS_DF_BITMAP1_OFFSET5,
    /// BITMAP 1bpp offset 5 in CLUT.
    IMOUOS_DF_BITMAP1_OFFSET6,
    /// BITMAP 1bpp offset 6 in CLUT.
    IMOUOS_DF_BITMAP1_OFFSET7,
    /// BITMAP 1bpp offset 7 in CLUT.
    IMOUOS_DF_BITMAP8_BGRA32,
    /// BITMAP 8bpp BGRA32.
    DIMOUOS_DF_BITMAP4_BGRA32_LOWER,
    /// BITMAP 4bpp BGRA32 lower address in CLUT.
    IMOUOS_DF_BITMAP4_BGRA32_UPPER,
    /// BITMAP 4bpp BGRA32 upper address in CLUT.
    IMOUOS_DF_BITMAP2_BGRA32_OFFSET0,
    /// BITMAP 2bpp BGRA32 offset 0 in CLUT.
    IMOUOS_DF_BITMAP2_BGRA32_OFFSET1,
    /// BITMAP 2bpp BGRA32 offset 1 in CLUT.
    IMOUOS_DF_BITMAP2_BGRA32_OFFSET2,
    /// BITMAP 2bpp BGRA32 offset 2 in CLUT.
    IMOUOS_DF_BITMAP2_BGRA32_OFFSET3,
    /// BITMAP 2bpp BGRA32 offset 3 in CLUT.
    IMOUOS_DF_BITMAP1_BGRA32_OFFSET0,
    /// BITMAP 1bpp BGRA32 offset 0 in CLUT.
    IMOUOS_DF_BITMAP1_BGRA32_OFFSET1,
    /// BITMAP 1bpp BGRA32 offset 1 in CLUT.
    IMOUOS_DF_BITMAP1_BGRA32_OFFSET2,
    /// BITMAP 1bpp BGRA32 offset 2 in CLUT.
    IMOUOS_DF_BITMAP1_BGRA32_OFFSET3,
    /// BITMAP 1bpp BGRA32 offset 3 in CLUT.
    IMOUOS_DF_BITMAP1_BGRA32_OFFSET4,
    /// BITMAP 1bpp BGRA32 offset 4 in CLUT.
    IMOUOS_DF_BITMAP1_BGRA32_OFFSET5,
    /// BITMAP 1bpp BGRA32 offset 5 in CLUT.
    IMOUOS_DF_BITMAP1_BGRA32_OFFSET6,
    /// BITMAP 1bpp BGRA32 offset 6 in CLUT.
    IMOUOS_DF_BITMAP1_BGRA32_OFFSET7,
    /// BITMAP 1bpp BGRA32 offset 7 in CLUT.

    IMOUOS_DF_BAYER_RAW = 0x70,
    /// Bayer pattern.
    IMOUOS_DF_RAW_VBI,
    /// Raw VBI data.
    IMOUOS_DF_RAW,
    /// Raw data - Format not interpreted.
    IMOUOS_DF_MISC,
    /// For future purpose.

    IMOUOS_DF_LATTICE,
    /**< lattice data   点阵数据，一个字节表示8个像素 */


    IMOUOS_DF_INVALID=0xFF
    /// Invalid data format. Could be used to initialize variables.
    /// 新定义的枚举不允许比此项大，因为此枚举有赋给uchar 的情况
}IMOUOS_VideoDataFormat;

/// 音频播放句柄
typedef Handle AudioTrack;

/// 音频播放类型
typedef enum AudioStreamType
{
    audioTypeTalkback = 0, ///< 对讲数据源
    audioTypeNormalPlay,   ///< 普通提示音数据源
    audioTypeAlarmPlay,    ///< 报警提示音数据源
    audioTypeNumber,       ///< 种类数
}AudioStreamType;

/// 音频播放状态回调
typedef void (*audioTrackOnCompletionListener)(Ptr cbPriv);
typedef void (*audioTrackOnErrorListener)(Ptr cbPriv, IMOUOS_ErrCode_e errType);
typedef void (*audioTrackOnPreparedListener)(Ptr cbPriv, IMOUOS_ErrCode_e errType);

typedef struct audioTrackStatusListener
{
    ///< 创建时传入的私有回调参数，回调时原样传回
    Ptr  cbPriv;
    ///< 音频播放完毕的时候回调
    audioTrackOnCompletionListener completionListener;
    ///< 播放中发生错误的时候回调 
    audioTrackOnErrorListener      errorListener;
    ///< 装载音频完毕的时候回调
    audioTrackOnPreparedListener   preparedListener;
}audioTrackStatusListener;

typedef enum audioTrackStatusEnum
{
    E_AUDIOPLAY_INVAILD,   ///< 无效状态
    E_AUDIOPLAY_ASKPLAY,   ///< 请求播放
    E_AUDIOPLAY_PREPARE,   ///< 解析完成
    E_AUDIOPLAT_COMPLETE,  ///< 播放完成
}audioTrackStatusEnum;


/// 码流类型，应和VideoEncStream保持一致
typedef enum VideoStreamType
{
    videoEncTypeMain = 0,         ///< 主码流
    videoEncTypeSub = 1,          ///< 辅码流
    videoEncTypeSnapshot = 2,     ///< 抓图
    videoEncTypeNumber = 3,       ///< 不包含扩展码流种类数(基线默认,无扩展码流时使用)
    videoEncTypeEx1 = videoEncTypeNumber,   ///< 扩展码流1
    videoEncTypeEx2,              ///< 扩展码流2
    videoEncTypeExNumber,         ///< 包含扩展的种类数(扩展码流时使用)
} VideoStreamType;


///< 视频流订阅钩子
///
/// \param [in]  chId 通道id，一般为0，目前不支持多通道
/// \param [in]  streamId 见VideoStreamType
/// \param [in]  mPkt 媒体数据，一般为DHAV格式：STP_Head(128) + Extra + data + STP_Tail(8)
/// \param [out] 无
/// \retval 参考IMOUOS_ErrCode_e
typedef Int32 (*videoStreamEncProc)(Int32 chId, Int32 streamId, media_pkt mPkt);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __MEDIA_AUDIOVIDEOTYPE_H__ */

