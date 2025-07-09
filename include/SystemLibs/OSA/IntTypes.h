//
//  "$Id$"
//
//  Copyright (c)1992-2016, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//  Description:
//  Revisions:      Year-Month-Day  SVN-Author  Modification
//

#ifndef __IMOUOS_TYPEDEF_INTTYPES_H__
#define __IMOUOS_TYPEDEF_INTTYPES_H__

////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* 无符号类型定义 */
#ifndef DEFINED_Uint32
#define DEFINED_Uint32
typedef unsigned int       Uint32;      /* 无符号32位整形数类型 */
#endif

#ifndef DEFINED_Int32
#define DEFINED_Int32
typedef int       Int32;                /* 有符号32位整形数类型 */
#endif

#ifndef DEFINED_Int16
#define DEFINED_Int16
typedef short     Int16;                 /* 有符号16位整形数类型 */
#endif

#ifndef DEFINED_Uint16
#define DEFINED_Uint16
typedef unsigned short     Uint16;      /* 无符号16位整形数类型 */
#endif

#ifndef DEFINED_Int8
#define DEFINED_Int8
typedef char      Int8;              /* 有符号8位整形数类型 */
#endif

#ifndef DEFINED_Uint8
#define DEFINED_Uint8
typedef unsigned char      Uint8;       /* 无符号8位整形数类型 */
#endif

#ifndef DEFINED_Int64
#define DEFINED_Int64
typedef long long          Int64;       /* 有符号64位整形数类型 */
#endif

#ifndef DEFINED_Bool16
#define DEFINED_Bool16
typedef unsigned short     Bool16;      /* 16位布尔类型 */
#endif

#ifndef DEFINED_Bool32
#define DEFINED_Bool32
typedef unsigned int       Bool32;      /* 32位布尔类型 */
#endif

#ifndef DEFINED_Int32L
#define DEFINED_Int32L
typedef long               Int32L;      /* 有符号32位长整形数类型 */
#endif

#ifndef DEFINED_Uint32L
#define DEFINED_Uint32L
typedef unsigned long      Uint32L;     /* 无符号32位长整形数类型 */
#endif

#ifndef DEFINED_Uint64
#define DEFINED_Uint64
typedef unsigned long long Uint64;      /* 无符号64位整形数类型 */
#endif

/* 浮点类型定义 */
#ifndef DEFINED_Float32
#define DEFINED_Float32
typedef float              Float32;		/* 32位浮点数类型 */
#endif

#ifndef DEFINED_Float64
#define DEFINED_Float64
typedef double             Float64;		/* 64位浮点数类型 */
#endif

/* 句柄类型 */
#ifndef DEFINED_Handle
#define DEFINED_Handle
typedef void *             Handle;      /* 统用句柄类型 */
typedef void *             Ptr;          /* 统用句柄类型 (兼容imousLittle)*/
#endif

/* 空类型定义 */
#ifndef DEFINED_Empty
#define DEFINED_Empty
typedef void               Empty;        /* 空类型 */
#endif

/* size_t类型定义 */
#ifndef DEFINED_Sizet
#define DEFINED_Sizet
typedef size_t             Sizet;       /* size_t类型 */
#endif

typedef enum {
    IMOUOS_FALSE = 0,
    IMOUOS_TRUE = 1,
}IMOUOS_BOOL;

#ifndef MAX
#define MAX(_val1, _val2) ((_val1)>=(_val2)?(_val1):(_val2))
#endif

#ifndef MIN
#define MIN(_val1, _val2) ((_val1)<=(_val2)?(_val1):(_val2))
#endif

#ifndef CLIP_MAX
#define CLIP_MAX(x,max)          ((x)>(max)?(max):(x))
#endif

#ifndef CLIP_MIN
#define CLIP_MIN(x,min)          ((x)<(min)?(min):(x))
#endif

#ifndef CLIP3
#define CLIP3(x,min,max)         ( (x)< (min) ? (min) : ((x)>(max)?(max):(x)) )
#endif

#ifndef IMOUOS_TIMEOUT_NONE
#define IMOUOS_TIMEOUT_NONE          (0)    /* 不等待，立即返回。*/
#endif

#ifndef IMOUOS_TIMEOUT_FOREVER
#define IMOUOS_TIMEOUT_FOREVER       (~0U)  /* 一直等待直到返回 */
#endif

#ifndef IMOUOS_NSPERSEC
#define IMOUOS_NSPERSEC              (1000000000)
#endif

#ifndef IMOUOS_USPERSEC
#define IMOUOS_USPERSEC              (1000000)
#endif

#ifndef IMOUOS_MSPERSEC
#define IMOUOS_MSPERSEC              (1000)
#endif

#endif// __IMOUOS_TYPEDEF_INTTYPES_H__

