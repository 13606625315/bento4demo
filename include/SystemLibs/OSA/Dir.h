#ifndef __IMOUOS_OSAPI_DIR_H__
#define __IMOUOS_OSAPI_DIR_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"
#include "OSA/File.h"

typedef imouos_handle imouos_dir;

typedef struct ImousDirAttr
{
    char name[256];
    ImousFileAttr attr;
}ImousDirAttr;

/// 打开目录
/// \param [out] imouos_dir *pdir  目录句柄地址
/// \param [in] const char* path  目录名
/// \return           IMOUOS_ErrCode_e 返回码
IMOUOS_API Int32 imouos_dirOpen(imouos_dir *pdir, const char* path);

/// 关闭目录
/// \param [in] imouos_dir dir 目录句柄
/// \return           IMOUOS_ErrCode_e 返回码
IMOUOS_API Int32 imouos_dirClose(imouos_dir dir);

/// 显示目录信息
/// \param [in] imouos_dir dir 目录句柄
/// \return           IMOUOS_ErrCode_e 返回码
IMOUOS_API Int32 imouos_dirInfo(imouos_dir dir, ImousDirAttr* attr);

/// 读取下一个文件
/// \param [in] imouos_dir dir 目录句柄
/// \return           IMOUOS_ErrCode_e 返回码
IMOUOS_API Int32 imouos_dirNext(imouos_dir dir);

#ifdef __cplusplus
}
#endif
#endif
