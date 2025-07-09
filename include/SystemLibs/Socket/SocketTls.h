/*******************************************************************************
* SocketTls.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  zhuyuchun <zhu_yuchun@dahuatech.com>
* Version:  V1.0.0  2022-01-24 Create
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


#ifndef __IMOUOS_SOCKETAPI_TLS_H__
#define __IMOUOS_SOCKETAPI_TLS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"

#include "mbedtls/config.h"
#include "mbedtls/platform.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"
#include "mbedtls/error.h"


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

typedef struct IMOU_TLS_COMMON
{
    mbedtls_net_context sock_fd, acp_fd;

    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt cacert;
    mbedtls_pk_context pkey;

#if 0
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;
#endif

    Int32 exit_code;
}IMOU_TLS_COMMON;


/********************************************************************
 *
 *                            函数声明区
 *
 *******************************************************************/


/**
* 创建基于ssl通道的服务器
* @param port 连接端口
* @param ca_crt 可信证书列表（sdk）
* @param ca_crt_len 证书列表长度
* @param pkey 服务端私钥
* @param pkey_len 服务端私钥长度
* @return 非NULL，服务端ssl对象 IMOU_TLS_COMMON 指针；NULL失败
*/
IMOUOS_API void * imouos_ssl_establishTcpServer(Uint16 port, const char *ca_crt, Uint32 ca_crt_len, const char *pkey, Uint32 pkey_len);


 /**
* ssl接收客户端连接请求
* @param ssl 加密对象
* @param timeout_ms 超时时间
* @return 0成功 -1失败
*/
IMOUOS_API Int32 imouos_ssl_acceptTcp(void *ssl, Uint32 timeout_ms);


/**
* 创建基于ssl通道的客户端连接
* @param host 连接ip或域名
* @param port 连接端口
* @param timeout_ms 超时时间
* @param ca_crt 可信证书列表（sdk）
* @param ca_crt_len 证书列表长度
* @return ssl通道对象
*/
IMOUOS_API void * imouos_ssl_establishTcpClient(const char *host, Uint16 port, Uint32 timeout_ms, const char *ca_crt, Uint32 ca_crt_len);


/**
* (非阻塞带超时)发送数据
* @param ssl 加密对象
* @param buf 写数据
* @param len 写数据长度
* @param timeout_ms 超时时间
* @return 成功返回发送数据的大小, 失败返回-1
*/
IMOUOS_API Int32 imouos_ssl_writeTcp(void * ssl, const char *buf, Uint32 len, Uint32 timeout_ms);


/**
* (非阻塞带超时)接收数据
* @param ssl 加密对象
* @param buf 读数据buffer
* @param len 读数据buffer长度
* @param timeout_ms 超时时间
* @return 成功返回接收数据的大小, 失败返回-1
*/
IMOUOS_API Int32 imouos_ssl_readTcp(void * ssl, char *buf, Uint32 len, Uint32 timeout_ms);


/**
* 关闭ssl加密通道
* @param ssl 加密对象
* @return 0成功；-1失败
*/
IMOUOS_API Int32 imouos_ssl_closeTcp(void * ssl);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // __IMOUOS_SOCKETAPI_TLS_H__

