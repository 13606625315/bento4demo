#ifndef __IMOUOS_OSAPI_FILE_H__
#define __IMOUOS_OSAPI_FILE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"

typedef imouos_handle imouos_file;

/* 文件的操作模式*/
typedef enum
{
    EIMOUOS_FILEMODE_RDONLY  = 0, /* 只读, 若文件不存在将失败*/
    EIMOUOS_FILEMODE_WRONLY,      /* 只写,从文件头开始写,原有数据将被清掉, 若文件不存在将创建*/
    EIMOUOS_FILEMODE_RDWR,        /* 读写,从文件头开始写,原有数据将被清掉, 若文件不存在将创建*/
    EIMOUOS_FILEMODE_WRAPPEND,    /* 追加写,从文件尾部追加写, 若文件不存在将创建*/
    EIMOUOS_FILEMODE_RDWR_NOCLR,  /* 从文件读写打开,不清数据,若文件不存在将创建*/
    EIMOUOS_FILEMODE_FEXIST,      /* 测试文件是否存在 */
} EImousFileMode;

typedef enum ECompatFileAttr
{
    EIMOUOS_FILEATTR_NORMAL      = 0x00 << 0x00, ///< 普通文件
    EIMOUOS_FILEATTR_DIRECTORY   = 0x01 << 0x00, ///< 目录
    EIMOUOS_FILEATTR_READONLY    = 0x01 << 0x01, ///< 只读
    EIMOUOS_FILEATTR_HIDDEN      = 0x01 << 0x02, ///< 隐藏文件
}EImousFileAttr;

typedef enum ECompatFileWhence
{
    EIMOUOS_SEEK_SET   = SEEK_SET,    ///< 文件开始
    EIMOUOS_SEEK_CUR   = SEEK_CUR,    ///< 当前位置
    EIMOUOS_SEEK_END   = SEEK_END,    ///< 文件末尾
}EImousFileWhence;

typedef enum EImousFileAccessMode
{
    EIMOUOS_ACCESSMODE_EXIST     = F_OK, ///< 检查文件是否存在
    EIMOUOS_ACCESSMODE_EXECABLE  = X_OK, ///< 可执行
    EIMOUOS_ACCESSMODE_WRITEABLE = W_OK, ///< 可写
    EIMOUOS_ACCESSMODE_READABLE  = R_OK, ///< 可读
}EImousFileAccessMode;

typedef struct ImousFileSystemInfo
{
    Uint64    totalBytes;     ///< 总字节数
    Uint64    freeBytes;      ///< 剩余容量
    Uint64    availableBytes; ///< 用户可用容量
}ImousFileSystemInfo;

typedef struct ImousFileAttr
{
    Uint32    attributes;     ///< 属性集，见`ECompatFileAttr`
    Uint64    createTime;     ///< 创建时间
    Uint64    lastAccessTime; ///< 最后访问时间
    Uint64    lastWriteTime;  ///< 最后修改时间
    Uint64    fileSize;       ///< 文件大小
}ImousFileAttr;

/// 打开文件
/// \param [in]    file    文件句柄
/// \param [in]    path    文件路径
/// \param [in]    mode    打开文件模式,同标准c
/// \return 负数为错误, 正数为成功读出长度 （同标准c）
Int32 imouos_fileOpen(imouos_file *pfile, const char* fileName, EImousFileMode mode);

/// 关闭文件
/// \param [in]    file    文件句柄
/// \return （同标准c）
IMOUOS_API Int32 imouos_fileClose(imouos_file file);

/// 读文件
/// \param [in]    file    文件句柄
/// \param [in]    buffer  读内存存放
/// \param [in]    size    读大小
/// \return 负数为错误, 正数为成功读出长度 （同标准c）
IMOUOS_API Int32 imouos_fileRead(imouos_file file, void* buffer, Uint32 size);

/// 写文件
/// \param [in]    file    文件句柄
/// \param [in]    buffer  写内存存放
/// \param [in]    size    写大小
/// \return 负数为错误, 正数为成功写入长度 （同标准c）
IMOUOS_API Int32 imouos_fileWrite(imouos_file file, const void* buffer, Uint32 size);

/// 强迫将缓冲区内的数据写回参数file指定的文件中,
/// 如果参数 file 为NULL,fflush()会将所有打开的文件数据更新
/// \param [in]    file    文件句柄
/// \return 成功返回0,失败为负数（同标准c）
IMOUOS_API Int32 imouos_fileFlush(imouos_file file);

/// 设置流 file 的文件位置为给定的偏移 offset
/// \param [in]    file    文件句柄
/// \param [in]    offset 这是相对 whence 的偏移量，以字节为单位
/// \param [in]    whence 这是表示开始添加偏移 offset 的位置。同标准c: SEEK_SET, SEEK_CUR, SEEK_END
/// \return 如果成功，则该函数返回零，错误返回非零(同标准c)
IMOUOS_API Int32 imouos_fileSeek(imouos_file file, Int32 offset, EImousFileWhence whence);

/// 该函数返回位置标识符的当前值,如果发生错误,返回错误码(负数)
/// \param [in]    pfile    文件句柄
/// \return 大于等于0,当前位置偏移，错误返回非0（同标准c）
IMOUOS_API Int32 imouos_fileTell(imouos_file file);

/// 从指定的流 stream 读取一行，并把它存储在 str 所指向的字符串内。当读取 (len-1) 个字符时，
/// 或者读取到换行符时，或者到达文件末尾时，它会停止.
/// \param [in]    pfile    文件句柄地址
/// \param [in]  buffer     这是指向一个字符数组的指针，该数组存储了要读取的字符串
/// \param [in]  len     这是要读取的最大字符数（包括最后的空字符）。通常是使用以 buffer传递的数组长度
/// \return 成功返回0,失败为负数
IMOUOS_API Int32 imouos_fileGets(imouos_file file, char* buffer, Int32 len);

/// 把字符串写入到指定的流 stream 中，但不包括空字符
/// \param [in]    pfile    文件句柄地址
/// \param [in]    buffer    包含了要写入的以空字符终止的字符序列
/// \return 成功返回0,失败为负数
IMOUOS_API Int32 imouos_filePuts(imouos_file file, const char* buffer);

/// amode参数为0时表示检查文件的存在性，如果文件存在，返回0
/// \param [in]    pfile    文件句柄
/// \param [in]    path    文件路径
/// \param [in]    mode    检查文件属性,同标准c
/// \06     检查读写权限
/// \04     检查读权限
/// \02     检查写权限
/// \01     检查执行权限
/// \00     检查文件的存在性
/// \而这个就算这个文件没有读权限，也可以判断这个文件存在于否
/// \return 成功返回0,失败为负数（同标准c）
IMOUOS_API Int32 imouos_fileAccess(const char* path, EImousFileAccessMode mode);

/// 文件重命名
/// \param [in]    pfile    文件句柄
/// \param [in]    path    文件路径
/// \param [in]    mode    打开文件模式,同标准c
/// \return 成功返回0,失败为负数（同标准c）
IMOUOS_API Int32 imouos_fileRename(const char* oldname, const char* newname);

/// 删除文件
/// \param [in]    pfile    文件句柄
/// \param [in]    path    文件路径
/// \param [in]    mode    打开文件模式,同标准c
/// \return 成功返回0,失败为负数（同标准c）
IMOUOS_API Int32 imouos_fileRemove(const char* path);

/// 创建一个以pathname为名字的目录，读、写、执行权限
/// \param [in]    path    文件路径
/// \return 成功返回0,失败为负数（同标准c）
IMOUOS_API Int32 imouos_fileMkdir(const char* path);

/// 删除一个以pathname为名字的目录
/// \param [in]    path    文件路径
/// \return 成功返回0,失败为负数（同标准c）
IMOUOS_API Int32 imouos_fileRmdir(const char* path);

/// 获得硬盘使用情况
/// \param [in]    path    位于需要查询信息的文件系统的文件路径名
/// \param [in]    info    文件信息地址
/// \return 成功返回0,失败为负数（同标准c）
IMOUOS_API Int32 imouos_fileStatFS(const char* path, ImousFileSystemInfo* info);

/// 获取文件属性信息
/// \param [in]    path    文件路径
/// \param [in]    attr    文件属性
/// \return 成功返回0,失败为负数（同标准c）
IMOUOS_API Int32 imouos_fileAttributes(const char* path, ImousFileAttr* attr);

#ifdef __cplusplus
}
#endif
#endif
