/*******************************************************************************
* SocketLocal.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  zhuyuchun <zhu_yuchun@dahuatech.com>
* Version:  V1.0.0  2022-01-17 Create
* Description: socket tpc Interface
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

#ifndef __IMOUOS_SOCKETAPI_LOCAL_H__
#define __IMOUOS_SOCKETAPI_LOCAL_H__
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


/********************************************************************
 *
 *                            函数声明区
 *
 *******************************************************************/


/**
* (非阻塞带超时)创建客户端套接字及连接
* @param name   本地文件路径
* @param type   socket类型
* @return fd套接字,无效为(Uint32)(-1)
*/
IMOUOS_API Int32 imouos_establishLocalClient(const char *name,Int32 type);


/**
* 创建服务端套接字及连接
* @param name  本地文件路径
* @param type  socket类型
* @return       fd套接字,无效为(Uint32)(-1)
*/ 
IMOUOS_API Int32 imouos_establishLocalServer(const char *name,Int32 type);


/**
* (阻塞)服务端接收客户端连接
* @param fd   套接字
* @return       fd套接字,无效为(Uint32)(-1)
*/ 
IMOUOS_API Int32 imouos_acceptLocal(Uint32 fd);


/**
* 关闭套接字
* @param fd   套接字
* @param name  本地文件路径
* @return     0成功；-1失败
*/ 
IMOUOS_API Int32 imouos_closeLocal(Uint32 fd);


/**
* (阻塞)发送数据
* @param fd   套接字
* @param buf  写数据
* @param len  写数据长度
* @return 成功返回发送数据的大小, 失败返回-1
*/ 
IMOUOS_API Int32 imouos_writeLocal(Uint32 fd, const char *buf, Uint32 len);

 
/**
* (阻塞)接收数据
* @param fd   套接字
* @param buf  读数据buffer
* @param len  读数据buffer长度
* @return 成功返回接收数据的大小, 失败返回-1
*/ 
IMOUOS_API Int32 imouos_readLocal(Uint32 fd, char *buf, Uint32 len);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __IMOUOS_SOCKETAPI_LOCAL_H__

