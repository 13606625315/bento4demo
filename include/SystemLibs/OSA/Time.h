#ifndef __IMOUOS_OSAPI_TIME_H__
#define __IMOUOS_OSAPI_TIME_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"

typedef struct imouos_clock_time
{
    Uint64    sec;            /** 秒 */
    Uint64    nsec;           /** 纳秒 */
}imouos_clock_time_t;

typedef struct imouos_calendar_time
{
    Int32         year;           /** 年 */
    Int32         month;          /** 月(1-12) */
    Int32         day;            /** 日(1-31) */
    Int32         wday;           /** 星期(0-周日 1-周一 ... 6-周六) */
    Int32         hour;           /** 时(0-23) */
    Int32         minute;         /** 分(0-59) */
    Int32         sec;            /** 秒(0-59) */
    Int32         usec;           /** 微秒(0-999999) */
}imouos_calendar_time_t;

typedef struct imouos_timezone
{
    Int32         timezone;       /** 时区信息(秒)，西半球为正数，东半球为负数 */
    Int32         is_dst;         /** 是否为夏令时时间 */
}imouos_timezone_t;

typedef struct imouos_dst_time
{
    Int32         year;           /** 年 */
    Int32         month;          /** 月(1-12) */
    Int32         day;            /** 日(1-31) */
}imouos_dst_time;

typedef struct imouos_dst_config
{
    int32_t enable;         /* 是否开启夏令时 */
    int32_t offset;         /* 指定夏令时时区时间偏移(分钟),提前为+,延后为- */
    imouos_dst_time startTime;
    imouos_dst_time endTime;
}imouos_dst_config;


/// 获取(UTC)日历时间(如:2022.4.22 10:53)
///
/// \param [in] utc    UTC时间
/// \return       0：成功；<0：失败
IMOUOS_API Int32 imouos_timeGetCalendar(imouos_calendar_time_t* calendar_time);

/// 获取(UTC)日历时间 (如:2022.4.22 10:53)
/// \warning  设置时，`day_of_week`字段将被忽略
/// param [in] utc    UTC时间
/// @return       0：成功；<0：失败
IMOUOS_API Int32 imouos_timeSetCalendar(imouos_calendar_time_t* calendar_time);

/// UTC时间转(UTC)日历时间
/// param [in] utcTime  UTC时间(自1770以来的秒)
/// @return       0：成功；<0：失败
IMOUOS_API imouos_calendar_time_t imouos_timeUtc2Calender(Uint64 utcTime);

/// (UTC)日历时间转UTC时间
/// param [in] pTime (UTC)日历时间
/// @return utcTime  UTC时间(自1770以来的秒)
IMOUOS_API Uint64 imouos_timeCalender2Utc(imouos_calendar_time_t* calendar_time);

/// 获取本地时间 (如:日历时间为 2022.4.22 10:53, 东八区本地时间为:2022.4.22 18:53)
/// \param [out] localtime  本地时间，已经包含时区、夏令时（=imouos_timeGetCalendar + 时区+夏令时）
/// \param [out] tz     时区信息
/// \return       0：成功；<0：失败
IMOUOS_API Int32 imouos_timeGetLocalTime(imouos_calendar_time_t* localtime);

/// 本地时间到日历时间转换
/// \param [out] localtime  本地时间，已经包含时区、夏令时（=imouos_timeGetCalendar + 时区+夏令时）
/// \param [out] tz     时区信息
/// \return       0：成功；<0：失败
IMOUOS_API imouos_calendar_time_t imouos_timeLocal2Calender(imouos_calendar_time_t* localtime);

/// 获取从一个标准时间点（epoch:1970-1-10时0秒）到现在的时间经过的秒数(随UTC变化而变)
/// 随UTC时间变化而变化
/// \param [out] utc  自1970年以来的秒和微秒
/// \return       0：成功；<0：失败
IMOUOS_API Int32 imouos_timeGetEpochClock(imouos_clock_time_t* clock_time);

/// 得到从系统启动到现在的毫秒数 (和UTC无关)
/// \return     相对时间
IMOUOS_API Uint64 imouos_timeGetMilliSecond(void);

/// 得到从系统启动到现在的微秒数 (和UTC无关)
/// \return     相对时间
IMOUOS_API Uint64 imouos_timeGetMicroSecond(void);

/// 得到从系统启动到现在的时间 (和UTC无关)
/// 包含秒和纳秒(=imouos_timeGetMilliSecond + imouos_timeGetMicroSecond)
/// @param [out] clocktime     自系统启动的秒和微秒
/// @return       0：成功；<0：失败
IMOUOS_API Int32 imouos_timeGetClock(imouos_clock_time_t* clocktime);

/// 设置和0时区(UTC时间)之间的时区差值
/// \param [in] seconds 当前时区和0时区相差的秒数,如果当前是东八区，那应该传入(8*3600),西八区则传入(-8*3600)
/// \return ETimeRet
IMOUOS_API Int32 imouos_timeSetTimezone(int32_t seconds);

/// 配置当前是否处于夏令时
/// \param [in] dstcfg  夏令时配置
/// \return             0：成功；<0：失败
IMOUOS_API Int32 imouos_timeSetDst(imouos_dst_config *dstcfg);

#ifdef __cplusplus
}
#endif
#endif
