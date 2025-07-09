//
//  "$Id$"
//
//  Copyright (c)1992-2016, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//  Description:
//  Revisions:      Year-Month-Day  SVN-Author  Modification
//

#ifndef __IMOUOS_UTILS_TOOLS_H__
#define __IMOUOS_UTILS_TOOLS_H__

#include "OSA/IntTypes.h"
#include "OSA/ImousTypes.h"

/********************************************************************
 *
 *                            宏定义区
 *
 *******************************************************************/
#if defined(__GNUC__) && !defined(IMOUOS_DISABLE_COMPILE_OPTIMIZE)
#define imouos_likely(x)     __builtin_expect(!!(x),1)
#define imouos_unlikely(x)   __builtin_expect(!!(x),0)
#else
#define imouos_likely(c)     (x)
#define imouos_unlikely(c)   (x)
#endif

/* 得到当前函数返回地址，即此函数被别的函数调用 */
#ifndef IMOUOS_BULITIN_LR
#define IMOUOS_BULITIN_LR() (__builtin_return_address(0))
#endif

/* 返回x中最后一个为1的位是从后向前的第几位，如__builtin_ffs(0x789)=1 */
#ifndef IMOUS_BULITIN_FFS
#define IMOUS_BULITIN_FFS(x) (__builtin_ffs(x))
#endif

/* 返回x中1的个数 */
#ifndef IMOUS_BULITIN_POPCOUNT
#define IMOUS_BULITIN_POPCOUNT(x) (__builtin_popcount(x))
#endif

/* x末尾0的个数,x=0时结果未定义Uint32 x=0x7890返回4 */
#ifndef IMOUS_BULITIN_CTZ
#define IMOUS_BULITIN_CTZ(x) (__builtin_ctz(x))
#endif

/* x前导0的个数,x=0时结果未定义 Uint32 x=0x7890返回17 */
#ifndef IMOUS_BULITIN_CLZ
#define IMOUS_BULITIN_CLZ(x) (__builtin_clz(x))
#endif

/* x中1的奇偶性 Uint32 x=0x7890返回0*/
#ifndef IMOUS_BULITIN_PARITY
#define IMOUS_BULITIN_PARITY(x) (__builtin_parity(x))
#endif

/* 按字节翻转x，返回翻转后的结果 Uint32 x=0x78912345,返回 0x45239178 */
#ifndef IMOUS_BULITIN_BSWAP32
#define IMOUS_BULITIN_BSWAP32(x) (__builtin_bswap32(x))
#endif

/* 如果x的值在编译时能确定,那么该函数返回值为1,可以在代码中做一些优化来减少处理exp的复杂度*/
#ifndef IMOUS_BULITIN_CONSTANT
#define IMOUS_BULITIN_CONSTANT(x) (__builtin_constant_p(x))
#endif

/* 用来判断两个类型是否相同,如果type_a与 type_b相同的话,就会返回1,否则的话,返回0 */
#ifndef IMOUS_BULITIN_TYPE_COMPATIBLE
#define IMOUS_BULITIN_TYPE_COMPATIBLE(type1, type2) (__builtin_types_compatible_p(type1, type2))
#endif

/**
* 根据结构体成员变量地址找到结构体首地址
*/
#if defined(container_of)
#define CONTAINER_FOR(ptr, TYPE, member)    container_of(ptr, TYPE, member)
#else
#define CONTAINER_FOR(ptr, TYPE, member)    \
    ((TYPE*)((Uint8*)(ptr) - (Sizet)&((TYPE*)0)->member))
#endif

/**
* 计算一个结构体中某个字段的偏移量
* @param TYPE	结构体类型
* @param member	字段
* @return		偏移量(字节)
*/
#if defined(offsetof)
#define OFFSET_OF(TYPE, member)						(offsetof(TYPE, member))
#else
#define OFFSET_OF(TYPE, member)						((Sizet)&((TYPE*)0)->member)
#endif
#ifndef __force
#define __force
#endif

#ifndef barrier
#define barrier() __asm__ __volatile__("": : :"memory")
#endif

static inline void __write_once_size(volatile void *p, void *res, Int32 size)
{
    switch (size) {
    case 1: *(volatile Uint8 *)p = *(Uint8 *)res; break;
    case 2: *(volatile Uint16 *)p = *(Uint16 *)res; break;
    case 4: *(volatile Uint32 *)p = *(Uint32 *)res; break;
    default:
        barrier();
        __builtin_memcpy((void *)p, (const void *)res, size);
        barrier();
    }
}

#define __READ_ONCE_SIZE                        \
({                                  \
    switch (size) {                         \
    case 1: *(Uint8 *)res = *(volatile Uint8 *)p; break;        \
    case 2: *(Uint16 *)res = *(volatile Uint16 *)p; break;      \
    case 4: *(Uint32 *)res = *(volatile Uint32 *)p; break;      \
    default:                            \
        barrier();                      \
        __builtin_memcpy((void *)res, (const void *)p, size);   \
        barrier();                      \
    }                               \
})

static inline
void __read_once_size(const volatile void *p, void *res, Int32 size)
{
    __READ_ONCE_SIZE;
}

static inline
void __read_once_size_nocheck(const volatile void *p, void *res, Int32 size)
{
    __READ_ONCE_SIZE;
}

#define __READ_ONCE(x, check)                       \
({                                  \
    union { typeof(x) __val; char __c[1]; } __u;            \
    if (check)                          \
        __read_once_size(&(x), __u.__c, sizeof(x));     \
    else                                \
        __read_once_size_nocheck(&(x), __u.__c, sizeof(x)); \
    __u.__val;                          \
})
#define READ_ONCE(x) __READ_ONCE(x, 1)

/*
 * Use READ_ONCE_NOCHECK() instead of READ_ONCE() if you need
 * to hide memory access from KASAN.
 */
#define READ_ONCE_NOCHECK(x) __READ_ONCE(x, 0)

#define WRITE_ONCE(x, val) \
({                          \
    union { typeof(x) __val; char __c[1]; } __u =   \
        { .__val = (__force typeof(x)) (val) }; \
    __write_once_size(&(x), __u.__c, sizeof(x));    \
    __u.__val;                  \
})

/**
* 获取固定数组大小
*/
#define IMOUOS_ARRAY_SIZE(arr)    (sizeof(arr)/ sizeof(arr[0]))

/**
* 对指针进行对齐。此函数返回大于等于`addr`且能被`align`整除的地址
* `align`必须是2的n次幂
*/
#define IMOUOS_ALIGN_PTR(addr, align)		\
	(void*)IMOUOS_ALIGN_SIZE(addr, align)

/**
* 对数字进行倍数取整。此函数返回大于等于`size`且能被`align`整除的值
* `align`必须是2的n次幂
*/
#define IMOUOS_ALIGN_SIZE(size, align)	\
	(((uintptr_t)(size) + ((uintptr_t)(align) - 1)) & ~((uintptr_t)(align) - 1))


#define IMOUOS_ALIGN_FLOOR(value, align)   (( (value) / (align) ) * (align) )

#endif // __IMOUOS_UTILS_TOOLS_H__
