#ifndef __IMOUOS_OSAPI_THREAD_H__
#define __IMOUOS_OSAPI_THREAD_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"

/********************************************************************
 *
 *                            宏定义区
 *
 *******************************************************************/

/********************************************************************
 *
 *                            类型定义区
 *
 *******************************************************************/
 /// 线程返回值
typedef enum
{
    EThread_ReturnRun = 0,      //执行下一次loop
    EThread_ReturnExit = -1,    //break,退出loop
} EThreadReturnStat;

/// 线程优先级
typedef enum
{
    EThread_Top = 80,
    EThread_Bottom = 20,
    EThread_Default = 50,
} EThreadPriority;

/// 线程调度枚举
typedef enum
{
    EThread_OTHER  = 0,  ///<  分时调度策略
    EThread_FIFO,        ///<  实时调度策略，先到先服务
    EThread_RR,          ///<  实时调度策略，时间片轮转
    EThread_NUM,
} EThreadPolicy;

typedef struct ThrAttr
{
    const char* name;           ///< 线程名,必须指向const
    Uint16 priority;          ///< 线程优先级,0-99
    Uint16 policy;            ///< 线程策略
    void *stackAddr;            ///< 线程栈地址,低地址
    Uint32 stackSize;         ///< 线程栈大小（0表示默认大小）
    int16_t cpuNo;             ///< 线程依附的核号（-1表示无依附）
    Uint8 res[2];             ///< 预留
} ThrAttr;

/// 线程句柄
typedef imouos_handle imouos_thr;

/// 线程处理函数
typedef Int32 (*thrProc)(void*);

/********************************************************************
 *
 *                            函数声明区
 *
 *******************************************************************/
/// 初始化线程属性
/// thrInAttr->name = "unnamed";
/// thrInAttr->policy = EThread_OTHER;
/// thrInAttr->priority = EThread_Default;
/// thrInAttr->stackAddr = NULL;
/// thrInAttr->stackSize = 0;
/// thrInAttr->cpuNo = -1;
/// \param [inout] imouos_thr thr 线程句柄
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_thrAttrInit(ThrAttr* thrInAttr);

/// 以deteach方式创建线程;线程内部自循环(while),但是不做sleep;
/// \ routine返回值小于0,退出循环 线程自动回收栈资源;
/// \routine返回值大于等于0,线程进入下一次循环
/// \param [inout] imouos_thr *thr 线程句柄地址
/// \param [in] thrAttr 线程属性
/// \param [in] thrProc routine 线程处理函数,
/// \param [in] arg       处理函数参数（外部维护内存）
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32  imouos_thrCreate(imouos_thr *thr, struct ThrAttr* thrInAttr, thrProc routine, void* arg);

/// 主动请求退出线程
/// \param [inout] imouos_thr thr 线程句柄
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32  imouos_thrCancel(imouos_thr thr);

/// 等待线程退出,回收资源
/// \param [inout] imouos_thr thr 线程句柄
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32 imouos_thrDestory(imouos_thr thr);

/// 获取当前线程的线程号
/// \return 当前线程ID
IMOUOS_API Uint32 imouos_thrGetCurTid(void);

/// 获取进程号
/// \return 当前线进程号
IMOUOS_API Uint32 imouos_thrGetCurPid(void);

/// 让调用线程阻塞一段时间
/// \param [in] milliSeconds 期望阻塞的毫秒数
IMOUOS_API void imouos_thrSleep(Uint32 milliSeconds);

/// 打印线程信息
IMOUOS_API void imouos_thrInfo(void);

/// 请求暂停线程
/// \param [inout] imouos_thr thr 线程句柄
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32  imouos_thrPause(imouos_thr thr);

/// 请求恢复线程
/// \param [inout] imouos_thr thr 线程句柄
/// \return IMOUOS_ErrCode_e
IMOUOS_API Int32  imouos_thrResume(imouos_thr thr);

#ifdef __cplusplus
}
#endif
#endif //  __IMOUOS_INFRA_THREAD_H__

