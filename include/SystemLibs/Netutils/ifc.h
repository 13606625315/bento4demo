/*******************************************************************************
* ifc.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  panjianguang <pan_jianguang@dahuatech.com>
* Version:  V1.0.0  2022-05-22 Create
* Description: net utils interface
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


#ifndef _IFC_H_
#define _IFC_H_

#include "OSA/ImousTypes.h"
#include <net/if.h>
#include <arpa/inet.h>

typedef enum
{
    IMOUOS_IFF_UP = IFF_UP,
    IMOUOS_IFF_RUNNING =    IFF_RUNNING,
    IMOUOS_IFF_MULTICAST =    IFF_MULTICAST,
    IMOUOS_IFF_BROADCAST = IFF_BROADCAST,
    IMOUOS_IFF_LOOPBACK = IFF_LOOPBACK,
    IMOUOS_IFF_POINTOPOINT = IFF_POINTOPOINT,
}NETFLAGS;

typedef struct _NetInterface
{
    char name[32];
    uint32_t reserved[8];
}NetInterface;

/// 获取网卡MTU大小
/// \param [in] ifname   网卡名
/// \param [in] size   MTU指针
/// \return  0:正确，其他linux通用错误码
IMOUOS_API int32_t imouos_ifcGetMTU(char *ifname, int32_t *size);

/// 设置网卡MTU大小
/// \param [in] ifname   网卡名
/// \param [in] size   MTU大小
/// \return  0:正确，其他linux通用错误码
IMOUOS_API int32_t imouos_ifcSetMTU(char *ifname, int32_t size);

/// 设置网卡IP地址和掩码，包括IPV4和IPV6
/// \param [in] ifname   网卡名
/// \param [in] ipaddr   IP地址
/// \param [in] prefixlen   掩码长度。
/// \return  0:正确，其他linux通用错误码
IMOUOS_API int32_t imouos_ifcSetHostIp(char *ifname,char *ipaddr,int32_t prefixlen);

/// 获取网卡状态
/// \param [in] ifname   网卡名
/// \param [in] flags   网卡flags，其值在NETFLAGS中。
/// \return  0:正确，其他linux通用错误码
IMOUOS_API int32_t imouos_ifcGetFlags(char *ifname,uint32_t *flags);

/// 获取网卡IP地址
/// \param [in] ifname   网卡名
/// \return 网卡ipaddr字符串
IMOUOS_API char* imouos_ifcGetIpaddr(char *ifname);

/// 获取网卡netmask
/// \param [in] ifname   网卡名
/// \return 网卡netmask字符串
IMOUOS_API char* imouos_ifcGetNetmask(char *ifname);

/// 使能网卡
/// \param [in] ifname   网卡名
/// \return  0:正确，其他linux通用错误码
IMOUOS_API int32_t imouos_ifcEnable(char *ifname);

/// 禁止网卡
/// \param [in] ifname   网卡名
/// \return  0:正确，其他linux通用错误码
IMOUOS_API int32_t imouos_ifcDisable(char *ifname);

/// 获取网卡MAC地址
/// \param [in] ifname   网卡名
/// \param [in] ptr   MAC地址，其传入长度需大于ETH_ALEN
/// \return  0:正确，其他linux通用错误码
IMOUOS_API int32_t imouos_ifcGetHwaddr(char *ifname,void *ptr);

/// 设置网卡MAC地址，不保存
/// \param [in] ifname   网卡名
/// \param [in] ptr   MAC地址
/// \return  0:正确，其他linux通用错误码
IMOUOS_API int32_t imouos_ifcSetHwaddr(char *ifname,void *ptr);

/// 获取网卡名称列表
/// \param [in] ifc  网卡数据结构体，含网卡名
/// \param [in] num  网卡数据结构最大数量
/// \return  >0 网卡数据结构真实数量，<0:异常码
IMOUOS_API int32_t imouos_ifcGetInterfaceList(NetInterface *ifc,uint32_t num);

/// 获取网卡Link状态
/// \param [in] ifname   网卡名
/// \return  0:down, 1:up, -1:other status
IMOUOS_API int32_t imouos_ifcGetNetlinkStatus(char *ifname);


/// 切换netmask to 切换prefixLength
/// \param [in] (char*)netmask
/// \return >0 int32_t PrefixLength <0:异常码
IMOUOS_API int32_t imouos_NetmaskStringToPrefixLength(char *string);

/// 设置default网关
/// \param [in] (char*)ifname 网卡名  [in]in_addr_t gateway 网关
/// \return  0:正确，其他linux通用错误码
IMOUOS_API int32_t imouos_ifc_set_default_route(char *ifname, in_addr_t gateway);

/// 获取default网关
/// \param [in] (char*)ifname 网卡名
/// \return  0:正确，其他linux通用错误码
IMOUOS_API int32_t imouos_ifc_get_default_route(char *ifname);

/// 删除default网关
/// \return  0:正确，其他linux通用错误码
IMOUOS_API int32_t imouos_ifc_remove_default_route();

/// 添加网关
/// \param [in] (char*)ifname 网卡名，(char*)dst ip地址，(int32_t) prefix_length,子网掩码长度，(char) *gw 网关
/// \return  0:正确，其他linux通用错误码
IMOUOS_API int32_t imouos_ifc_add_route(char *ifname, char *dst, int32_t prefix_length, char *gw);

/// 删除网关
/// \param [in] (char*)ifname 网卡名，(char*)dst ip地址，(int32_t) prefix_length,子网掩码长度，(char) *gw 网关
/// \return  0:正确，其他linux通用错误码
IMOUOS_API int32_t imouos_ifc_remove_route(char *ifname, char*dst, int32_t prefix_length, char *gw);

#endif

