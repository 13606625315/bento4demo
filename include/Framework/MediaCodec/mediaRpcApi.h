/*******************************************************************************
* mediaRpcApi.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  wang_zhengxue <wang_zhengxue@dahuatech.com>
* Version:  V1.0.0  2022-03-02 Create
* Description: meida模块对外提供的跨进程API
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

#ifndef __MEDIA_RPC_API_H__
#define __MEDIA_RPC_API_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "OSA/ImousTypes.h"
#include "OSA/IntTypes.h"
#include "mediaTypes.h"

/// iot连接媒体进程,远程交互初始化
/// iot和媒体进程都可以主动向对方发起调用
/// \retval 参考IMOUOS_ErrCode_e
Int32 imouos_connectIot2Media(void);

/// 连接媒体进程,远程交互初始化
/// 只有当前进程可以向媒体进程发起调用
/// \retval 参考IMOUOS_ErrCode_e
Int32 imouos_connectMedia(void);

/// 提交一次精确抓图请求
///
/// \param [in]  chan 视频通道(0\1...)
/// \param [in]  u64YuvSeq 需要抓图的YUV序号(同imouos_mediaGetYuv pstYuvPacket->u64Seq)
/// \param [out] pId 图片的序列ID,用于精确匹配,传给iot SDK(iot进程),
/// \ iot进程转给meida进程,调用snapshot,参数 IMOU_SnapshotParams成员userdata存放pId
/// \retval 参考IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_mediaRequestSnap(Uint32 chan, Uint64 u64YuvSeq, Uint32 *pId);

/// 获取yuv数据 (必须与 imouos_mediaReleaseYuv 配套使用)
///
/// \param [in]  chan 视频通道(0\1...)
/// \param [in]  type yuv 功能 参考 EMeidaYuvFuncType
/// \param [out] pstYuvPacket yuv数据
/// \retval 参考IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_mediaGetYuv(Uint32 chan, Uint32 type, MediaYuvPacket *pstYuvPacket);

/// 释放yuv数据 (必须与 imouos_mediaGetYuv 配套使用)
///
/// \param [in]  chan 视频通道(0\1...)
/// \param [in]  type yuv 功能 参考 EMeidaYuvFuncType
/// \param [out] pstYuvPacket yuv数据
/// \retval 参考IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_mediaReleaseYuv(Uint32 chan, Uint32 type, MediaYuvPacket *pstYuvPacket);

/// 初始化播放通道audioTrack
///
/// \param [in]  audioTrack 播放Track对象
/// \param [in]  statusListener 播放状态回调
/// \param [out] 无
/// \retval 参考IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_mediaAudioTrackInit(AudioTrack *audioTrack, audioTrackStatusListener *statusListener);

/// 设置Track文件路径
///
/// \param [in]  audioTrack 播放Track对象
/// \param [in]  path 待播文件路径
/// \param [out] 无
/// \retval 参考IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_mediaAudioTrackPath(AudioTrack audioTrack, Int8 *path);

/// 文件预解析
///
/// \param [in]  audioTrack 播放Track对象
/// \param [out] 无
/// \retval 参考IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_mediaAudioTrackPrepare(AudioTrack audioTrack);

/// 设置Track类型
///
/// \param [in]  audioTrack 播放Track对象
/// \param [in]  audioType 待播语音类型
/// \param [out] 无
/// \retval 参考IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_mediaAudioTrackType(AudioTrack audioTrack, AudioStreamType audioType);

/// 设置Track音量
///
/// \param [in]  audioTrack 播放Track对象
/// \param [in]  volume 音量：[0, 100]
/// \param [out] 无
/// \retval 参考IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_mediaAudioTrackSetVolume(AudioTrack audioTrack, Int32 volume);

/// 获取Track音量
///
/// \param [in]  audioTrack 播放Track对象
/// \param [out] volume：Track音量，[0, 100]
/// \retval 参考IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_mediaAudioTrackGetVolume(AudioTrack audioTrack, Int32 *volume);

/// 启动Track开始播放
///
/// \param [in]  audioTrack 播放Track对象
/// \param [out] 无
/// \retval 参考IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_mediaAudioTrackStart(AudioTrack audioTrack);

/// 停止Track结束播放
///
/// \param [in]  audioTrack 播放Track对象
/// \param [out] 无
/// \retval 参考IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_mediaAudioTrackStop(AudioTrack audioTrack);

/// 销毁Track对象
///
/// \param [in]  audioTrack 播放Track对象
/// \param [out] 无
/// \retval 参考IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_mediaAudioTrackDeInit(AudioTrack audioTrack);

/// 查询Track对应文件的播放状态
///
/// \param [in]  audioTrack 播放Track对象
/// \param [in]  path 需要查询的文件路径，空则为Track状态
/// \param [out] 无
/// \retval >0 audioTrackStatusEnum：播放状态
/// \retval <0 播放错误       
IMOUOS_API Int32 imouos_mediaAudioTrackQueryStat(AudioTrack audioTrack, Int8 *path);

/// 视频流订阅
///
/// \param [in]  chId 通道id，一般为0，目前不支持多通道
/// \param [in]  streamId   见VideoStreamType
/// \param [in]  streamProc 码流推流钩子
/// \param [out] 无
/// \retval 参考IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_mediaVideoSubStream(Int32 chId, Int32 streamId, videoStreamEncProc streamProc);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __MEDIA_RPC_API_H__ */

