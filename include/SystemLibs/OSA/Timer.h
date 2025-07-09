//
//  Copyright (c)1992-2016, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//  Description:
//  Revisions:      Year-Month-Day  SVN-Author  Modification
//

#ifndef __IMOUOS_OSAPI_TIMER_H__
#define __IMOUOS_OSAPI_TIMER_H__

#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"
#include "OSA/Semaphore.h"
#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************
 *
 *                            类型定义区
 *
 *******************************************************************/
/// 定时器句柄
typedef imouos_handle imouos_timer;

/// 定时器事件处理函数
/// \param [in]  msg     消息内容
/// \return
typedef Int32 (*timerEvtProc)(void* arg);

typedef struct
{
    Uint32 type;          // 0:由线程timerque完成任务 1:调用imouos_timerPending完成任务
    Uint32 delay;         //指定启动后延时多少时间调用，单位为毫秒，如果为0表示立即开始调用
    Uint32 period;        //定时器的周期，指定距上次调用多少时间后再次调用，单位为ms;为0,执行一次
    timerEvtProc function;   //事件处理函数指针
    void *data;           //function 自定义参数
}imouos_timer_cfg;

/********************************************************************
 *
 *                            函数声明区
 *
 *******************************************************************/
/// 创建一个定时器对象
/// \param [out] name   定时器句柄地址
/// \param [in] imouos_timer_cfg *timercfg   定时器配置
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_timerCreate(imouos_timer *ptimer, imouos_timer_cfg *timercfg);

/// 销毁定时器对象
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_timerDestory(imouos_timer timer);

/// 添加(开启)一个定时器对象
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_timerAdd(imouos_timer timer);

///  取消定时器
/// \param [in]  timer  定时器句柄
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_timerCancel(imouos_timer timer);

/// 设置定时器周期
///  imouos_timerMod(timer, expires) is equivalent to:
///  del_timer(timer); timer->expires = expires; add_timer(timer);
/// \param [in] timer   定时器句柄
/// \param [in] delay   定时器启动延时时间
/// \param [in] period  定时器周期
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_timerMod(imouos_timer timer, Uint32 delay, Uint32 period);

/// 设置定时器周期
/// imouos_timerModPending() is the same for pending timers as imouos_timerMod(),
/// but will not re-activate and modify already deleted timers.
/// \param [in] timer   定时器句柄
/// \param [in] delay   定时器启动延时时间
/// \param [in] period  定时器周期
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_timerModPending(imouos_timer timer, Uint32 delay, Uint32 period);

/// 等待定时器事件(timercfg->type非0时调用执行定时任务)
/// \param [in] timer   定时器句柄
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_timerPending(imouos_timer timer, Uint32 timeout);

/// 初始化定时器模块
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_initTimers(void);

/// 销毁定时器模块
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_deinitTimers(void);


#ifdef __cplusplus
}
#endif

#endif //  __AEDA_INFRA_TIMER_H__

