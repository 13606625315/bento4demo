/*******************************************************************************
* shm.h
*
* Copyright (C) 2022-2024 imou CO.,LTD.
*
* Author :  panjianguang <pan_jianguang@dahuatech.com>
* Version:  V1.0.0  2022-01-12 Create
* Description: Binder Bnbinder Interface
*
*       1.  ó2?t?μ?÷?￡
*
*       2.  3ìDò?á11?μ?÷?￡
*
*       3.  ê1ó??μ?÷?￡
*
*       4.  ???TD??μ?÷?￡
*
*       5.  ?????μ?÷?￡
*           ?T
*
* Modification:
*    Date    :
*    Revision:
*    Author  :
*    Contents:
*******************************************************************************/

#ifndef _IMOUOS_SHM_H_
#define _IMOUOS_SHM_H_

#include "OSA/ImousTypes.h"

IMOUOS_API int imouos_ashmem_valid(int fd);
IMOUOS_API int imouos_ashmem_create_region(const char *name, size_t size);
IMOUOS_API void* imouos_ashmem_mapfd(int fd, int32_t offset,size_t size);
IMOUOS_API int imouos_ashmem_set_prot_region(int32_t fd, int prot);
IMOUOS_API int imouos_ashmem_pin_region(int32_t fd, size_t offset, size_t len);
IMOUOS_API int imouos_ashmem_unpin_region(int32_t fd, size_t offset, size_t len);
IMOUOS_API int imouos_ashmem_get_size_region(int32_t fd);

#endif
