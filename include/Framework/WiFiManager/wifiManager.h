/********************************************************************************
 * Copyright (C) 2022 Hangzhou Huacheng Network Technology Co., Ltd.
 * @file    : wifiManager.h
 * @brief   :
 * @version : V1.0.0
 * @author  : Wang Kang <wang_kang7@dahuatech.com>
 * @date    : 2022-04-07 15:35:33
 ********************************************************************************/

#ifndef __WIFIMANAGER_H__
#define __WIFIMANAGER_H__

#include "OSA/ImousTypes.h"
#ifdef __cplusplus
extern "C" {
#endif

#define WIFIMANAGER_SERVICE_NAME 	"WifiManagerService"

#define WLAN_SSID_LENGTH		(32 + 1)
#define WLAN_PSK_LENGTH			(64 + 1)
#define WLAN_SCAN_MAX_NUM       (128)
#define WLAN_MAC_ADDR_LEN       (6)

#define WIFIMANAGER_RESULT_SUCCESS 0
#define WIFIMANAGER_RESULT_FAILED -1


enum
{
    WIFIMANAGER_REASON_NONE = 0,
	WIFIMANAGER_REASON_WAIT,            //等待连接
	WIFIMANAGER_REASON_CONNECTING,      //正在连接
    WIFIMANAGER_REASON_NOTSSID,         //连接失败是无此SSID
    WIFIMANAGER_REASON_PASSWORDERROR,   //连接失败是密码错误
    WIFIMANAGER_REASON_SUCCESS = 5,		//连接成功
    WIFIMANAGER_REASON_KICK ,           //断开连接是路由器繁忙等原因踢掉
    WIFIMANAGER_REASON_KICK_USER,       //断开连接是调用接口断开
    WIFIMANAGER_REASON_OTHERFAIL,       //断开连接是其他原因
    WIFIMANAGER_REASON_APMISSING,       //断开连接是AP消失
    WIFIMANAGER_REASON_DHCPERR = 10,    //连接失败是由于DHCP申请失败
    WIFIMANAGER_REASON_UNTENSITY_0 = 80,//连接信号强度<=-65db
    WIFIMANAGER_REASON_UNTENSITY_1 = 81,//连接信号强度-58~-65db
    WIFIMANAGER_REASON_UNTENSITY_2 = 82,//连接信号强度-52~-58db
    WIFIMANAGER_REASON_UNTENSITY_3 = 83,//连接信号强度-45~-52db
    WIFIMANAGER_REASON_UNTENSITY_4 = 84,//连接信号强度0~-45db
    WIFIMANAGER_REASON_ENABLEERROR = 100,//使能wifi失败
};

typedef int32_t (*wifiManager_callback)(int32_t reason);

typedef struct _wifiManagerListener
{
    wifiManager_callback connect;
    wifiManager_callback disconnect;
    wifiManager_callback opensoftAP;
    wifiManager_callback closesoftAP;
}WifiManagerListener;

/******************************************************************************/
/******************************** SOFTAP **************************************/
/******************************************************************************/
typedef enum
{
	E_WLAN_HOSTAPD_AUTH_OPEN = 0,
	E_WLAN_HOSTAPD_AUTH_WPAPSK,
	E_WLAN_HOSTAPD_AUTH_WPA2PSK,
	E_WLAN_HOSTAPD_AUTH_WEP
} E_WLAN_HOSTAPD_AUTHMODE;

typedef struct WLAN_SoftAPConfig
{
	char ssid[WLAN_SSID_LENGTH];
	char password[WLAN_PSK_LENGTH];
	int32_t channel;
	int32_t auth; //E_WLAN_HOSTAPD_AUTHMODE
	int32_t ignoreSsid; //是否为隐藏热点
} WLAN_SoftAPConfig;

/******************************************************************************/
/********************************** STA ***************************************/
/******************************************************************************/
typedef enum
{
	E_WLAN_AUTH_OPEN = 0,
	E_WLAN_AUTH_WEP,
	E_WLAN_AUTH_WPS,
	E_WLAN_AUTH_WPA,
	E_WLAN_AUTH_WPA2,
	E_WLAN_AUTH_WPA_WPA2,
	E_WLAN_AUTH_WPAPSK,
	E_WLAN_AUTH_WPA2PSK,
	E_WLAN_AUTH_WPAPSK_WPA2PSK,
	E_WLAN_AUTH_WPA3PSK,
	E_WLAN_AUTH_WPA2PSK_WPA3PSK,
	E_WLAN_AUTH_AUTO,
} E_WLAN_AUTH_MODE;

typedef enum
{
	E_WLAN_ENCR_UNKNOWN = 0,
	E_WLAN_ENCR_OPEN,
	E_WLAN_ENCR_WEP,
	E_WLAN_ENCR_TKIP,
	E_WLAN_ENCR_AES,
	E_WLAN_ENCR_TKIP_AES,
} E_WLAN_ENCR_ALGR;

typedef struct WLAN_StaConnConfig
{
    char ssid[WLAN_SSID_LENGTH];
	char password[WLAN_PSK_LENGTH];
	int32_t authMode;
    int32_t reserved[32];
}WLAN_StaConnectConfig;

typedef struct WlanScanAPConfig
{
	char ssid[WLAN_SSID_LENGTH];
    int32_t reserved[32];
}WLAN_ScanAPConfig;

typedef enum
{
	E_WLAN_BANDINFO_2_4G = 0x01,
	E_WLAN_BANDINFO_5G = 0x10,
	E_WLAN_BANDINFO_2_4G_5G = 0x11,
}E_WLAN_BANDINFO;

typedef struct WlanScanAPInfo
{
	char ssid[WLAN_SSID_LENGTH];
	int32_t encrFlag;   // 0:Open, 1:Encr E_WLAN_ENCR_ALGR
	int32_t auth;
	int32_t channel;
	int32_t signalLevel;
	int32_t bandType;   // 0x01:Only 2.4G, 0x10:Only 5G, 0x11:2.4G&5G  E_WLAN_BANDINFO
	int32_t rssi;
	char    bssid[WLAN_MAC_ADDR_LEN];
	char    resChar[2];
	int32_t reserved[29];
}WLAN_ScanAPInfo;

enum connectStatus
{
    CONNECT_STATUS_DISABLE = 0,				// 未使能
    CONNECT_STATUS_CONNECTING = 1,			// wifi连接
    CONNECT_STATUS_SUCCESS = 2,				// 连接成功
    CONNECT_STATUS_ERROR = 3,				// 断开连接/连接函数返回err
    CONNECT_STATUS_SINGLE_CONNECTING = 4,	// 单点wifi连接中
};

typedef struct WlanWorkState
{
	char ssid[WLAN_SSID_LENGTH];	///< 如果已连接，该值为连接的AP的bssid；未连接时为空
	int32_t mode;					///< AP or Sta  (0:AP,1:STA)
	int32_t connected;				///< 详见connectStatus
	int32_t  channel;				///< 信道
	int32_t  signalLevel;			///< 信号等级
	char bssid[WLAN_MAC_ADDR_LEN];  ///< MAC地址
	char    resChar[2];
	int32_t reserved[30];
}WLAN_LinkInfo;

typedef enum multiWifiIndex
{
	ELIST_WIFI0 = 0,
    ELIST_WIFI1 = 1,
    ELIST_WIFI2 = 2,
    ELIST_WIFI3 = 3,
    ELIST_WIFI4 = 4,
	ELIST_WIFI_NUM,
}multiWifiIndex;

typedef struct MultiWifInfo
{
	int8_t  ssid[WLAN_SSID_LENGTH];
	int8_t  password[WLAN_PSK_LENGTH];
	uint8_t authMode;
	int32_t isUsable;
	int32_t isInvalid;
	int32_t reserved[32];
}MultiWifInfo;

typedef struct MultiWifiListInfo
{
	MultiWifInfo wifiInfo[ELIST_WIFI_NUM];	// 多wifi
}MultiWifiListInfo;

/**
* wifi功能监听
* @param listener    监听回调函数
* @return 返回IMOUOS_ErrCode_e错误码
*/
IMOUOS_API int32_t imouos_wifiAddListener(WifiManagerListener *listener);

/**
* wifi功能开或关
* @param enable     使能位
* @return 返回IMOUOS_ErrCode_e错误码
*/
IMOUOS_API int32_t imouos_wifiEnable(int32_t enable);

/**
* wifi自动连接是否开启(开启后使能wifi后wifi会自动连接之前网络 关闭后wifi会断开)
* @param isAutoConnect     使能位
* @return 返回IMOUOS_ErrCode_e错误码
*/
IMOUOS_API int32_t imouos_wifiAutoConnectSet(int32_t isAutoConnect);

/**
* 获取多wifi列表
* @param mulWifiList    多wifi列表
* @param mulWifiNum     多wifi数量
* @return 返回IMOUOS_ErrCode_e错误码
*/
IMOUOS_API int32_t imouos_wifiMultiListGet(MultiWifiListInfo *mulWifiList, int32_t *mulWifiNum);

/**
* 从多wifi列表切换ssid
* @param ssid    需要切换的ssid
* @return 返回IMOUOS_ErrCode_e错误码
*/
IMOUOS_API int32_t imouos_wifiMultiListSwitchSsid(char *ssid);

/**
* 从多wifi列表删除ssid
* @param ssid    需要删除的ssid
* @return 返回IMOUOS_ErrCode_e错误码
*/
IMOUOS_API int32_t imouos_wifiMultiListDeleteSsid(char *ssid);

/**
* cloud云端登录成功通知
* @return 返回IMOUOS_ErrCode_e错误码
*/
IMOUOS_API void imouos_CloudLoginSuccess(void);

/**
* 软AP开启，此函数为异步函数，成功后回回调listerner
* @param softAPConfig     软AP配置
* @return 返回IMOUOS_ErrCode_e错误码
*/
IMOUOS_API int32_t imouos_wifiSoftapOpen(WLAN_SoftAPConfig *softAPConfig);

/**
* 软AP关闭
* @return 返回IMOUOS_ErrCode_e错误码
*/
IMOUOS_API int32_t imouos_wifiSoftapClose(void);

/**
* 扫描周围热点信息
* @param scanConfig     扫描选项配置  如果为空则返回所有扫描结果，不为空则仅返回相应ssid对应信息
* @param scanAPs		扫描AP详细配置信息
* @param apNum			扫描AP数量
* @return 返回IMOUOS_ErrCode_e错误码
*/
IMOUOS_API int32_t imouos_wifiScanAP(WLAN_ScanAPConfig *scanConfig, WLAN_ScanAPInfo *scanAPs, int32_t *apNum);

/**
* 连接热点，此函数为异步函数，成功后回回调listerner
* @param connectConfig	需要连接的热点配置
* @return 返回IMOUOS_ErrCode_e错误码
*/
IMOUOS_API int32_t imouos_wifiConnectAP(WLAN_StaConnectConfig* connectConfig);

/**
* 断连热点，此函数为异步函数，成功后回回调listerner
* @return 返回IMOUOS_ErrCode_e错误码
*/
IMOUOS_API int32_t imouos_wifiDisconnectAP(void);

/**
* 获取连接热点信息
* @param LinkInfo     连接的热点信息
* @return 返回IMOUOS_ErrCode_e错误码
*/
IMOUOS_API int32_t imouos_wifiLinkInfoGet(WLAN_LinkInfo* LinkInfo);

/**
* 设置芯片模式为纯wifi
* @return 返回IMOUOS_ErrCode_e错误码
*/
IMOUOS_API int32_t imouos_SetModeWifi();

/**
* 设置芯片模式为ble+wifi
* @return 返回IMOUOS_ErrCode_e错误码
*/
IMOUOS_API int32_t imouos_SetModeNimble();


#ifdef __cplusplus
}
#endif

#endif // __WIFIMANAGER_H__

