#ifndef __IMOUOS_OSAPI_IPC_H__
#define __IMOUOS_OSAPI_IPC_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <sys/msg.h>

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
typedef imouos_handle imouos_ipc;

/********************************************************************
 *
 *                            函数声明区
 *
 *******************************************************************/
/// 创建共享区域, creates  and  opens a new, or opens an existing, POSIX shared memory object.
/// \param [out] imouos_ipc *pshm  shm句柄地址
/// \param [in] const char *name 共享内存mingz,成功后会生成文件/dev/shm/"name"
/// \param [in] Uint32 size, 共享大小
/// \return 返回虚拟地址,NULL为失败
IMOUOS_API void* imouos_ipcShmOpen(imouos_ipc *pshm, const char *name, Uint32 size);

/// 关闭共享区域, creates  and  opens a new, or opens an existing, POSIX shared memory object.
/// \param [out] imouos_ipc shm  shm句柄地址
/// \return 返回虚拟地址,NULL为失败
IMOUOS_API Int32 imouos_ipcShmClose(imouos_ipc shm);

#ifdef __cplusplus
}
#endif
#endif //  __IMOUOS_OSAPI_IPC_H__
