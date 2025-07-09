/*******************************************************************************
* SocketTcp.h
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

#ifndef __IMOUOS_SOCKETAPI_TCP_H__
#define __IMOUOS_SOCKETAPI_TCP_H__
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
* @param host    连接ip或域名
* @param port   连接端口
* @param timeout_ms    超时时间
* @return       fd套接字,无效为(Uint32)(-1)
*/
IMOUOS_API Int32 imouos_establishTcpClient(const char *host, Uint16 port, Uint32 timeout_ms);


/**
* 创建服务端套接字及连接
* @param port   连接端口
* @return       fd套接字,无效为(Uint32)(-1)
*/ 
IMOUOS_API Int32 imouos_establishTcpServer(Uint16 port);


/**
* (非阻塞带超时)服务端接收客户端连接
* @param fd   套接字
* @param timeout_ms   超时时间
* @return       fd套接字,无效为(Uint32)(-1)
*/ 
IMOUOS_API Int32 imouos_acceptTcp(Uint32 fd, Uint32 timeout_ms);

/**
* 关闭套接字
* @param fd   套接字
* @return     0成功；-1失败
*/ 
IMOUOS_API Int32 imouos_closeTcp(Uint32 fd);


/**
* (非阻塞带超时)发送数据
* @param fd   套接字
* @param buf  写数据
* @param len  写数据长度
* @param timeout_ms   超时时间
* @return 成功返回发送数据的大小, 失败返回-1
*/ 
IMOUOS_API Int32 imouos_writeTcp(Uint32 fd, const char *buf, Uint32 len, Uint32 timeout_ms);

 
/**
* (非阻塞带超时)接收数据
* @param fd   套接字
* @param buf  读数据buffer
* @param len  读数据buffer长度
* @param timeout_ms   超时时间
* @return 成功返回接收数据的大小, 失败返回-1
*/ 
IMOUOS_API Int32 imouos_readTcp(Uint32 fd, char *buf, Uint32 len, Uint32 timeout_ms);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __IMOUOS_SOCKETAPI_TCP_H__

