/*******************************************************************************
* IServiceManager.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  panjianguang <pan_jianguang@dahuatech.com>
* Version:  V1.0.0  2022-01-10 Create
* Description: Binder IService Interface
*
*       1.  Ó²¼þËµÃ÷¡£
*
*       2.  ³ÌÐò½á¹¹ËµÃ÷¡£
*
*       3.  Ê¹ÓÃËµÃ÷¡£
*
*       4.  ¾ÖÏÞÐÔËµÃ÷¡£
*
*       5.  ÆäËûËµÃ÷¡£
*           ÎÞ
*
* Modification:
*    Date    :
*    Revision:
*    Author  :
*    Contents:
*******************************************************************************/

#ifndef _ISERVICEMANAGER_H_
#define _ISERVICEMANAGER_H_

#include "Binder/bpbinder.h"
#include "OSA/ImousTypes.h"
#include "OSA/RbMap.h"

#define BI_PACK_CHARS(c1,c2,c3,c4) ((((c1) << 24)) | (((c2) << 16)) | (((c3) << 8)) | (c4))

enum {
    /* Must match definitions in IBinder.h and IServiceManager.h */
    PING_TRANSACTION  = BI_PACK_CHARS('_','P','N','G'),
    DUMP_TRANSACTION  = BI_PACK_CHARS('_','D','M','P'),
    SVC_MGR_GET_SERVICE = 1,
    SVC_MGR_CHECK_SERVICE,
    SVC_MGR_ADD_SERVICE,
    SVC_MGR_LIST_SERVICES,

    /*add user private define enum*/
    SVC_MGR_DO_CALLBACK = 0x12345678,
};

/// service注册函数，与serviceManager进程强绑定
/// \param [in] BnBinder imouos_bnbinder_create函数返回值
/// \return  设置服务参数的返回状态IMOUOS_ErrCode_e
IMOUOS_API int32_t  imouos_addService(BnBinder *binder);

/// service查询函数，与serviceManager进程强绑定
/// \param [in] serviceName 服务名字，名字作为唯一码存在
/// \return  BpBinder 返回binder代理句柄
IMOUOS_API BpBinder* imouos_getService(char* serviceName);

/// service注册服务查询，与serviceManager进程强绑定
/// \return  imouos_rbmap_root 返回注册的服务名，value为string名字
IMOUOS_API imouos_rbmap_root*  imouos_listService(void);

/// 回调bpbinder得到函数，与imouos_parcel_writeObject配合使用
/// \param [in] data parcel输入参数
/// \return  BpBinder 远端代理函数
IMOUOS_API BpBinder* imouos_getStrongBinder(Parcel *data);

#endif

