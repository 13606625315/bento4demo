#ifndef __IMOUOS_BROADCAST_H__
#define __IMOUOS_BROADCAST_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*imous_event_hander)(char* data, uint32_t len);

/**
* @brief		订阅消息
* @param[in]	obj	订阅的对象
* @param[in]	func 订阅的回调函数
* @param[in]	timeout	订阅超时时间
* @return  成功返回0，失败返回-1
*/
int imouos_subscribe( const char* obj, imous_event_hander func);

/**
* @brief		取消订阅消息
* @param[in]	obj	订阅的对象
* @param[in]	func 订阅的回调函数
* @param[in]	timeout	订阅超时时间
* @return  成功返回0，失败返回-1
*/
int imouos_unSubscribe( const char* obj, imous_event_hander func);

/**
* @brief		通知消息
* @param[in]	obj	通知的对象
* @param[in]	data 通知的数据
* @param[in]	len 通知的数据长度
* @param[in]	timeout	通知超时时间
* @return  成功返回0，失败返回-1
*/
int imouos_notify(const char *obj,const char* data, uint32_t dataLen);
#ifdef __cplusplus
}
#endif
#endif