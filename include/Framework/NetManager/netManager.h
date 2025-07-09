#ifndef _NETMANAGER_H_
#define _NETMANAGER_H_

#include "OSA/IntTypes.h"

#define MAX_IP_CNT              16
#define INTF_NUM_MAX            8
#define MAX_IP_STRING_LENTH     16
#define MAX_SERVER_ADDR_LENTH   64
#define MAX_INTFNAME_LENTH      16

typedef struct 
{
    Int32   ipType;
    char    ipString[MAX_IP_STRING_LENTH];
} IpStringList; 

enum
{
    INTF_STATUS_DOWN    = 0,
    INTF_STATUS_UP      = 1,
};

enum
{
    INTF_PROTO_DHCP     = 0,
    INTF_PROTO_STATIC   = 1,
};

typedef struct
{
    char    intfname[MAX_INTFNAME_LENTH];
    Uint8   status;     /* 0：down，1：up */
    Uint8   proto;      /* 0：dhcp，1：static */
    char    ipAddr[MAX_IP_STRING_LENTH];
    char    netMask[MAX_IP_STRING_LENTH];
    char    gwAddr[MAX_IP_STRING_LENTH];
} IntfInfo;
#if 0
/**
* 获取NTP服务器地址
* @param masterAddr     主NTP服务器地址
* @param slaveAddr      备NTP服务器地址
* @return 成功返回0，失败返回错误码
*/
Int32 imouos_getNtpServerAddr(char *masterAddr, char *slaveAddr);

/**
* 设置NTP服务器地址
* @param masterAddr     主NTP服务器地址
* @param slaveAddr      备NTP服务器地址
* @return 成功返回0，失败返回错误码
*/
Int32 imouos_setNtpServerAddr(char *masterAddr, char *slaveAddr);
#endif
/**
* 获取DNS服务器地址
* @param masterAddr     主DNS服务器地址
* @param slaveAddr      备DNS服务器地址
* @return 成功返回0，失败返回错误码
*/
Int32 imouos_getDnsServerAddr(char *masterAddr, char *slaveAddr);

/**
* 设置DNS服务器地址
* @param masterAddr     主DNS服务器地址
* @param slaveAddr      备DNS服务器地址
* @return 成功返回0，失败返回错误码
*/
Int32 imouos_setDnsServerAddr(char *masterAddr, char *slaveAddr);

/**
* 通过域名获取ipv4地址
* @param domainName     域名
* @param dnsIpIist      IP地址列表
* @param ipCnt          IP地址数量，输入输出参数，输入最大获取IP地址数量， 输出已获取IP地址数量
* @return 成功返回0，失败返回错误码
*/
Int32 imouos_getHostByName(char *domainName, IpStringList *dnsIpIist, Int32 *ipCnt);

/**
* 打开网卡的dhcp服务
* @param intfName       网卡名
* @return 成功返回0，失败返回错误码
*/
Int32 imouos_startDHCP(char *intfName);

#if 0
/**
* 设置网卡信息
* @param intfName       网卡名
* @param proto          网卡协议，DHCP: INTF_PROTO_DHCP，STATIC：INTF_PROTO_STATIC
* @param ipAddr         网卡IP地址，proto为INTF_PROTO_STATIC有效
* @param netMask        网卡子网掩码，proto为INTF_PROTO_STATIC有效
* @param gwAddr         网卡网关地址，proto为INTF_PROTO_STATIC有效
* @return 成功返回0，失败返回错误码
*/
Int32 imouos_setIntfInfo(char *intfName, Uint8 proto, char *ipAddr, char *netMask, char *gwAddr);

/**
* 获取网卡信息
* @param intfName       网卡名，NULL或空字符串时获取所有网卡
* @param intfInfo       网卡信息
* @param intfCnt        网卡信息数量，输入输出参数，输入最大获取网卡数量， 输出已获取网卡信息数量
* @return 成功返回0，失败返回错误码
*/
Int32 imouos_getIntfInfo(char *intfName, IntfInfo *intfInfo, Uint32 *intfCnt);


/**
* 手动同步NTP时间
* @return 成功返回0，失败返回失败返回错误码
*/
Int32 imouos_syncNtpTime();
#endif

#endif /* _NETMANAGER_H_ */
