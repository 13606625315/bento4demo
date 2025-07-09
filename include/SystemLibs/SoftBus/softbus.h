/*******************************************************************************
* softbus.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  panjianguang <pan_jianguang@dahuatech.com>
* Version:  V1.0.0  2022-04-20 Create
* Description: softbus headfile
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

#ifndef _SOFTBUS_H_
#define _SOFTBUS_H_

#include "OSA/ImousTypes.h"
#include "OSA/IntTypes.h"
/// \ softbus api usage
/// softbus 接口
/// \n 调用流程图:
/// \code
///    ================================
///          imouos_softbusInit
///               |
///      imouos_softbusCreateServer
///               |
///        imouos_softBusSetSerCfg
///               |
///          imouos_softbusStart
///               |
///         imouos_softBusSendMsg
///               |
///         imouos_softBusSubscribe
///               |
///          imouos_softbusStop
///               |
///           imouos_softbusExit
///               |
///    ================================
/// \code

typedef int32_t (*serviceProc)(void);

typedef struct _SerLoadInfo
{
	serviceProc initProc;			/// 服务初始化函数指针，服务启动时调用（消息的订阅，业务初始化等都在这里做）
	serviceProc exitProc;			/// 服务初始化函数指针，服务退出时调用
} SerLoadInfo;

typedef struct SoftBusServiceInfo
{
	uint32_t	msgQueCapacity;	///< 总容量
	uint32_t	msgQueSize;		///< 当前消息量
	uint8_t		reversed[24];	///< 保留字段
}SoftBusServiceInfo;

typedef struct _SoftBusData
{
    void *data;
    uint32_t size;
}SoftBusData;

typedef int32_t (*eventProc)(SoftBusData* msgData,void* arg);

typedef int32_t (*msgProc)(SoftBusData *msgData);

typedef int32_t (*timerEventProc)(void* arg);

typedef void* softbusTimer;

///单个消息处理映射表
typedef struct MsgProcMap
{
    uint32_t    msgID;              ///消息id
    msgProc     proc;               ///消息处理函数
} MsgProcMap;

typedef struct _SoftbusSerAttr
{
    uint32_t*   serviceIdList;
    uint32_t    serviceIdNum;
    char*       name;
    uint16_t    priority;
    uint16_t    policy;
    uint32_t    stackSize;      //< 线程栈大小（0表示默认大小）
	int32_t		cpuNo;			//< 线程依附的核号（-1表示无依附）    
}SoftbusSerAttr;

///  softbus初始化
/// \return     初始化状态 IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_softbusInit(void);

///  softbus退出
/// \return     退出状态 IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_softbusExit(void);

///  softbus开始工作
/// \return     softbus开始成功状态 IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_softbusStart(void);

///  softbus停止工作
/// \return     softbus停止成功状态 IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_softbusStop(void);

/// 创建softbus服务
/// \param [in] attr    设置softbus服务的参数，其中serviceIdList为设置的服务号列表
/// \return          创建softbus服务返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_softbusCreateServer(SoftbusSerAttr *attr);

/// 设置服务的msgMap表和启动退出函数
/// \param [in] serId   服务的ID号
/// \param [in] msgMap  msgMap时间表
/// \param [in] size    msgMap时间表大小
/// \param [in] serInfo 服务ID号的启动退出函数设置
/// \return          设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_softBusSetSerCfg(uint32_t serId, MsgProcMap* msgMap, uint32_t size, SerLoadInfo *serInfo);

/// 往特定服务发送消息
/// \param [in] serId  接收服务的ID号
/// \param [in] msgID  接收服务的msgID号，对应msgMAP表中的msgID号
/// \param [in] data   发送数据的地址
/// \param [in] size   发送数据的大小
/// \param [in] respProc  应答接收函数
/// \return         往特定服务发送消息返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_softBusSendMsg(uint32_t serId, uint32_t msgID, void*data,uint32_t size,msgProc respProc);

/// 往特定服务返回消息
/// \param [in] msgData 函数返回句柄，为msgProc对应的首个参数
/// \param [in] msgID  返回服务的msgID号，对应msgMAP表中的msgID号
/// \param [in] data   返回数据的地址
/// \param [in] size   返回数据的大小
/// \return         往特定服务返回消息的状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_softbusSendRespMsg(SoftBusData* msgData,uint32_t msgID, void *data, uint32_t size);

/// 通知消息
/// \param [in] evtId 通知消息的evtID号
/// \param [in] data  通知消息的数据地址
/// \param [in] size  通知消息的数据大小
/// \return         通知消息返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_softBusNotifyEvent(uint32_t evtId,void *data, uint32_t size);

/// 订阅消息
/// \param [in] evtId 订阅消息的evtID号，这个订阅号必须小于0xFFFF0000
/// \param [in] proc  订阅消息的回调函数
/// \param [in] priv  订阅消息的私有数据
/// \return         订阅消息返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_softBusSubscribe(uint32_t evtId, eventProc proc, void* priv);

/// 取消订阅消息
/// \param [in] evtId 取消订阅消息的evtID号，这个订阅号必须小于0xFFFF0000
/// \param [in] proc  取消订阅消息的回调函数
/// \param [in] priv  取消订阅消息的私有数据
/// \return         取消订阅消息返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_softBusUnsubscribe(uint32_t evtId, eventProc proc, void* priv);

/// 查询特定服务的信息
/// \param [in] serID 查询服务的ID号
/// \param [in] info  查询服务的状态信息返回，主要为待处理msgqueue大小
/// \return         查询特定服务消息返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_softBusGetServiceInfo(uint32_t serID, SoftBusServiceInfo* info);

///  创建定时器
/// \param [in]  name  名称
/// \return     softbusTimer 定时器句柄
IMOUOS_API softbusTimer imouos_softbusTimerCreate(char* name);

///  销毁消息
/// \param [in]  timer  定时器句柄
/// \return     IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_softbusTimerDestroy(softbusTimer timer);

/// 打开定时器
/// \param [in] timer   定时器句柄
/// \param [in] addr    事件通知地址
/// \param [in] proc    事件处理函数指针
/// \param [in] delay   指定启动后延时多少时间调用，单位为毫秒，如果为0表示立即开始调用
/// \param [in] period  定时器的周期，指定距上次调用多少时间后再次调用，单位为毫秒，
///                    如果为0表示是非周期定时器，第一次调用完毕后会自动停止。
/// \param [in] arg		自定义参数
/// \return          IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_softbusTimerStart(softbusTimer timer,timerEventProc proc, uint32_t delay, uint32_t period, void* arg);

/// 停止定时器
/// \param [in] timer   定时器句柄
/// \return          IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_softbusTimerStop(softbusTimer timer);

/// 定时器是否启动
/// \param [in]  timer     定时器句柄
/// \return    IMOUOS_ErrCode_e 启动 其他： 没有启动
IMOUOS_API int32_t imouos_softbusTimerIsStart(softbusTimer timer);

/// 设置定时器周期
/// \param [in] timer   定时器句柄
/// \param [in] period  定时器周期
/// \return    IMOUOS_ErrCode_e
IMOUOS_API int32_t imouos_softbusTimerSetPeriodTime(softbusTimer timer, uint32_t period);

#endif

