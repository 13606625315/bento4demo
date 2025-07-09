/*******************************************************************************
* systemServer.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  panjianguang <pan_jianguang@dahuatech.com>
* Version:  V1.0.0  2022-08-24 Create
* Description: systemServer headfile 
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

#ifndef _SYSTEMSERVER_H_
#define _SYSTEMSERVER_H_

#include "OSA/ImousTypes.h"
#include "OSA/Time.h"

typedef enum
{
    SYSTEM_REBOOT_NORMAL,
    SYSTEM_REBOOT_RESETFACTORY,
    SYSTEM_REBOOT_UPGRADE,
    SYSTEM_REBOOT_EXCEPTION,
    SYSTEM_SHUTDOWN,
}SYSTEM_REBOOT_REASON;

typedef enum
{
	SYSTEM_USB_NONE,
    SYSTEM_ADD_USB_DISK,
    SYSTEM_REMOVE_USB_DISK,
    SYSTEM_ADD_USB_SERIAL,
    SYSTEM_REMOVE_USB_SERIAL,
    SYSTEM_UDISK_SAME_VERSION
}SYSTEM_USB_EVENT;

typedef enum
{
    SYSTEM_USB_UPGRADE,
	SYSTEM_APP_UPGRADE,
    SYSTEM_APP_UPGRADE_SINGLE_PART,
    SYSTEM_USB_UPGRADE_SINGLE_PART
}SYSTEM_UPGRADE_TYPE;

typedef enum
{
    SYSTEM_FREQ_LOW,
    SYSTEM_FREQ_HIGH
}SYSTEM_FREQ_TYPE;

#define    SYSTEM_UPGRADE_FAILED	0
#define    SYSTEM_UPGRADE_RUNNING	1
#define    SYSTEM_UPGRADE_SUCCESS	2

#define		MAINPROCESS		1
#define		SLAVEPROCESS		0

typedef struct upgradeMsg {
    int16_t percent;
    int16_t state;
	int16_t type;
	char mcu_fw_path[128];
} upgrade_msg;

typedef int32_t (*systemRebootEventCallback)(int32_t reason);

typedef int32_t (*systemUsbEventCallback)(SYSTEM_USB_EVENT event);

typedef int32_t (*systemUpgradeEventCallback)(upgrade_msg Msg);

/**
 * @brief    reboot 重启
 * @return   IMOUOS_ErrCode_e value.
 */
IMOUOS_API int32_t imouos_systemReboot(void);

/**
 * @brief    resetFactory 恢复出产设置
 * @return   IMOUOS_ErrCode_e value.
 */
IMOUOS_API int32_t imouos_systemResetFactory(void);

/**
 * @brief    shutdown 关机
 * @return   IMOUOS_ErrCode_e value.
 */
IMOUOS_API int32_t imouos_systemShutDown(void);

/**
 * @brief    系统升级 
 * @param    [in] path  升级文件路径
 * @return   IMOUOS_ErrCode_e value.
 */
IMOUOS_API int32_t imouos_systemUpgrade(char *path);

/**
 * @brief    cpufreq settting 
 * @param    [in] speed
 * @return   IMOUOS_ErrCode_e value.
 */
IMOUOS_API int32_t imouos_systemFreq(SYSTEM_FREQ_TYPE speed);

/**
 * @brief    usb插拔事件 
 * @param    [in] event  事件内容,参考 SYSTEM_USB_EVENT
 * @return   IMOUOS_ErrCode_e value.
 */
IMOUOS_API int32_t imouos_systemUevent(int32_t event);

/**
 * @brief    usb事件查询 
 * @param    [in] event  事件内容
 * @return   IMOUOS_ErrCode_e value.
 */
IMOUOS_API int32_t imouos_systemUdiscovery(SYSTEM_USB_EVENT *event);

/**
 * @brief    获取产测模式 
 * @param    [out] mode 模式类型 
 * @return   IMOUOS_ErrCode_e value.
 */
IMOUOS_API int32_t imouos_systemGetFactoryMode(int32_t *mode);

/**
 * @brief    获取重启原因
 * @param    [out] reason
 * @return   IMOUOS_ErrCode_e value.
 */
IMOUOS_API int32_t imouos_systemGetRebootReason(uint32_t *reason);

/**
 * @brief    设置时区 
 * @param    [in] seconds  时区偏移秒数
 * @return   IMOUOS_ErrCode_e value.
 */
IMOUOS_API int32_t imouos_systemSetTimeZone(int32_t seconds);

/**
 * @brief    获取时区 
 * @param    [out] seconds  时区偏移秒数
 * @return   IMOUOS_ErrCode_e value.
 */
IMOUOS_API int32_t imouos_systemGetTimeZone(int32_t *seconds);

/**
 * @brief    设置时区 
 * @param    [in] seconds  时区偏移秒数
 * @return   IMOUOS_ErrCode_e value.
 */
IMOUOS_API int32_t imouos_systemSetCalendarTime(imouos_calendar_time_t *calendarTime);

/**
 * @brief    增加系统监听回调
 * @param    [in] event  监听回调函数，一个进程只允许设置一个
 * @return   IMOUOS_ErrCode_e value.
 */
IMOUOS_API int32_t imouos_systemAddEventListener(systemRebootEventCallback rebootEvent,	systemUpgradeEventCallback upgradeEvent, systemUsbEventCallback usbEvent);

/**
 * @brief    标记主进程
 * @param    [in] flag  主进程标志
 * @return   IMOUOS_ErrCode_e value.
 */
IMOUOS_API int32_t imouos_systemAddMainProcess(int32_t flag);

/**
 * @brief    删除该进程系统监听回调
 * @return   IMOUOS_ErrCode_e value.
 */
IMOUOS_API int32_t imouos_systemDelEventListener(void);

/**
 * @brief    获取预留内存地址
 * @param    [in]	name	进程名
 * @param    [out] addr	mmap地址
 * @param    [out] len		可映射内存大小
 * @return   IMOUOS_ErrCode_e value.
 */
IMOUOS_API int32_t imouos_systemGetMemAddr(char *name, size_t *addr, int *len);

#endif

