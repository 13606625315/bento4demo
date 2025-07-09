//
//  "$Id$"
//
//  Copyright (c)1992-2016, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//  Description:
//  Revisions:      Year-Month-Day  SVN-Author  Modification
//

#ifndef __IMOUOS_TYPEDEF_TYPES_H__
#define __IMOUOS_TYPEDEF_TYPES_H__

#include <stdio.h>
#include <assert.h>

#define IMOUOS_VERSION_ENCODE(major, minor, revision)			(((major) * 1000000) + ((minor) * 1000) + (revision))

/**
* MSVC编译器探测
*/
#if !defined(_MSC_VER)
#	define IMOUOS_MSVC_VERSION_CHECK(major,minor,patch) (0)
#elif defined(_MSC_VER) && (_MSC_VER >= 1400)
#	define IMOUOS_MSVC_VERSION_CHECK(major,minor,patch)		(_MSC_FULL_VER >= ((major * 10000000) + (minor * 100000) + (patch)))
#elif defined(_MSC_VER) && (_MSC_VER >= 1200)
#	define IMOUOS_MSVC_VERSION_CHECK(major,minor,patch)		(_MSC_FULL_VER >= ((major * 1000000) + (minor * 10000) + (patch)))
#else
#	define IMOUOS_MSVC_VERSION_CHECK(major,minor,patch)		(_MSC_VER >= ((major * 100) + (minor)))
#endif

/**
* GNUC编译器探测
*/
#if defined(__GNUC__) && defined(__GNUC_PATCHLEVEL__)
#	define IMOUOS_GNUC_VERSION								IMOUOS_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#elif defined(__GNUC__)
#	define IMOUOS_GNUC_VERSION								IMOUOS_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, 0)
#endif
#if defined(IMOUOS_GNUC_VERSION)
#	define IMOUOS_GNUC_VERSION_CHECK(major,minor,patch)		(IMOUOS_GNUC_VERSION >= IMOUOS_VERSION_ENCODE(major, minor, patch))
#else
#	define IMOUOS_GNUC_VERSION_CHECK(major,minor,patch)		(0)
#endif

/**
* Intel编译器探测
*/
#if defined(__INTEL_COMPILER) && defined(__INTEL_COMPILER_UPDATE)
#	define IMOUOS_INTEL_VERSION								IMOUOS_VERSION_ENCODE(__INTEL_COMPILER / 100, __INTEL_COMPILER % 100, __INTEL_COMPILER_UPDATE)
#elif defined(__INTEL_COMPILER)
#	define IMOUOS_INTEL_VERSION								IMOUOS_VERSION_ENCODE(__INTEL_COMPILER / 100, __INTEL_COMPILER % 100, 0)
#endif
#if defined(IMOUOS_INTEL_VERSION)
#	define IMOUOS_INTEL_VERSION_CHECK(major,minor,patch)		(IMOUOS_INTEL_VERSION >= IMOUOS_VERSION_ENCODE(major, minor, patch))
#else
#	define IMOUOS_INTEL_VERSION_CHECK(major,minor,patch)		(0)
#endif

/**
* PGI编译器探测
*/
#if defined(__PGI) && defined(__PGIC__) && defined(__PGIC_MINOR__) && defined(__PGIC_PATCHLEVEL__)
#	define IMOUOS_PGI_VERSION									IMOUOS_VERSION_ENCODE(__PGIC__, __PGIC_MINOR__, __PGIC_PATCHLEVEL__)
#endif
#if defined(IMOUOS_PGI_VERSION)
#  define IMOUOS_PGI_VERSION_CHECK(major,minor,patch)			(IMOUOS_PGI_VERSION >= IMOUOS_VERSION_ENCODE(major, minor, patch))
#else
#  define IMOUOS_PGI_VERSION_CHECK(major,minor,patch)			(0)
#endif

/**
* ARM编译器探测
*/
#if defined(__CC_ARM) && defined(__ARMCOMPILER_VERSION)
#	define IMOUOS_ARM_VERSION									IMOUOS_VERSION_ENCODE(__ARMCOMPILER_VERSION / 1000000, (__ARMCOMPILER_VERSION % 1000000) / 10000, (__ARMCOMPILER_VERSION % 10000) / 100)
#elif defined(__CC_ARM) && defined(__ARMCC_VERSION)
#	define IMOUOS_ARM_VERSION									IMOUOS_VERSION_ENCODE(__ARMCC_VERSION / 1000000, (__ARMCC_VERSION % 1000000) / 10000, (__ARMCC_VERSION % 10000) / 100)
#endif
#if defined(IMOUOS_ARM_VERSION)
#	define IMOUOS_ARM_VERSION_CHECK(major,minor,patch)		(IMOUOS_ARM_VERSION >= IMOUOS_VERSION_ENCODE(major, minor, patch))
#else
#	define IMOUOS_ARM_VERSION_CHECK(major,minor,patch)		(0)
#endif

/**
* TI编译器探测
*/
#if defined(__TI_COMPILER_VERSION__) && (defined(__TMS470__) || defined(__TI_ARM__) || defined(__MSP430__) || defined(__TMS320C2000__))
#	if (__TI_COMPILER_VERSION__ >= 16000000)
#		define IMOUOS_TI_VERSION								IMOUOS_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#	endif
#endif
#if defined(IMOUOS_TI_VERSION)
#	define IMOUOS_TI_VERSION_CHECK(major,minor,patch)			(IMOUOS_TI_VERSION >= IMOUOS_VERSION_ENCODE(major, minor, patch))
#else
#	define IMOUOS_TI_VERSION_CHECK(major,minor,patch)			(0)
#endif

/**
* TI_ARMCL编译器探测
*/
#if defined(__TI_COMPILER_VERSION__) && (defined(__TMS470__) || defined(__TI_ARM__))
#	define IMOUOS_TI_ARMCL_VERSION							IMOUOS_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif
#if defined(IMOUOS_TI_ARMCL_VERSION)
#	define IMOUOS_TI_ARMCL_VERSION_CHECK(major,minor,patch)	(IMOUOS_TI_ARMCL_VERSION >= IMOUOS_VERSION_ENCODE(major, minor, patch))
#else
#	define IMOUOS_TI_ARMCL_VERSION_CHECK(major,minor,patch)	(0)
#endif

/**
* TI_CL430编译器探测
*/
#if defined(__TI_COMPILER_VERSION__) && defined(__MSP430__)
#	define IMOUOS_TI_CL430_VERSION							IMOUOS_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif
#if defined(IMOUOS_TI_CL430_VERSION)
#  define IMOUOS_TI_CL430_VERSION_CHECK(major,minor,patch)	(IMOUOS_TI_CL430_VERSION >= IMOUOS_VERSION_ENCODE(major, minor, patch))
#else
#  define IMOUOS_TI_CL430_VERSION_CHECK(major,minor,patch)	(0)
#endif

/**
* TI_CL2000编译器探测
*/
#if defined(__TI_COMPILER_VERSION__) && defined(__TMS320C2000__)
#	define IMOUOS_TI_CL2000_VERSION							IMOUOS_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif
#if defined(IMOUOS_TI_CL2000_VERSION)
#	define IMOUOS_TI_CL2000_VERSION_CHECK(major,minor,patch)	(IMOUOS_TI_CL2000_VERSION >= IMOUOS_VERSION_ENCODE(major, minor, patch))
#else
#	define IMOUOS_TI_CL2000_VERSION_CHECK(major,minor,patch)	(0)
#endif

/**
* TI_CL6X编译器探测
*/
#if defined(__TI_COMPILER_VERSION__) && defined(__TMS320C6X__)
#	define IMOUOS_TI_CL6X_VERSION								IMOUOS_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif
#if defined(IMOUOS_TI_CL6X_VERSION)
#	define IMOUOS_TI_CL6X_VERSION_CHECK(major,minor,patch)	(IMOUOS_TI_CL6X_VERSION >= IMOUOS_VERSION_ENCODE(major, minor, patch))
#else
#	define IMOUOS_TI_CL6X_VERSION_CHECK(major,minor,patch)	(0)
#endif

/**
* TI_CL7X编译器探测
*/
#if defined(__TI_COMPILER_VERSION__) && defined(__C7000__)
#	define IMOUOS_TI_CL7X_VERSION								IMOUOS_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif
#if defined(IMOUOS_TI_CL7X_VERSION)
#	define IMOUOS_TI_CL7X_VERSION_CHECK(major,minor,patch)	(IMOUOS_TI_CL7X_VERSION >= IMOUOS_VERSION_ENCODE(major, minor, patch))
#else
#	define IMOUOS_TI_CL7X_VERSION_CHECK(major,minor,patch)	(0)
#endif

/**
* TI_CLPRU编译器探测
*/
#if defined(__TI_COMPILER_VERSION__) && defined(__PRU__)
#	define IMOUOS_TI_CLPRU_VERSION							IMOUOS_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif
#if defined(IMOUOS_TI_CLPRU_VERSION)
#	define IMOUOS_TI_CLPRU_VERSION_CHECK(major,minor,patch)	(IMOUOS_TI_CLPRU_VERSION >= IMOUOS_VERSION_ENCODE(major, minor, patch))
#else
#	define IMOUOS_TI_CLPRU_VERSION_CHECK(major,minor,patch)	(0)
#endif

/**
* SUNPRO编译器探测
*/
#if defined(__SUNPRO_C) && (__SUNPRO_C > 0x1000)
#	define IMOUOS_SUNPRO_VERSION								IMOUOS_VERSION_ENCODE((((__SUNPRO_C >> 16) & 0xf) * 10) + ((__SUNPRO_C >> 12) & 0xf), (((__SUNPRO_C >> 8) & 0xf) * 10) + ((__SUNPRO_C >> 4) & 0xf), (__SUNPRO_C & 0xf) * 10)
#elif defined(__SUNPRO_C)
#	define IMOUOS_SUNPRO_VERSION								IMOUOS_VERSION_ENCODE((__SUNPRO_C >> 8) & 0xf, (__SUNPRO_C >> 4) & 0xf, (__SUNPRO_C) & 0xf)
#elif defined(__SUNPRO_CC) && (__SUNPRO_CC > 0x1000)
#	define IMOUOS_SUNPRO_VERSION								IMOUOS_VERSION_ENCODE((((__SUNPRO_CC >> 16) & 0xf) * 10) + ((__SUNPRO_CC >> 12) & 0xf), (((__SUNPRO_CC >> 8) & 0xf) * 10) + ((__SUNPRO_CC >> 4) & 0xf), (__SUNPRO_CC & 0xf) * 10)
#elif defined(__SUNPRO_CC)
#	define IMOUOS_SUNPRO_VERSION								IMOUOS_VERSION_ENCODE((__SUNPRO_CC >> 8) & 0xf, (__SUNPRO_CC >> 4) & 0xf, (__SUNPRO_CC) & 0xf)
#endif
#if defined(IMOUOS_SUNPRO_VERSION)
#  define IMOUOS_SUNPRO_VERSION_CHECK(major,minor,patch)		(IMOUOS_SUNPRO_VERSION >= IMOUOS_VERSION_ENCODE(major, minor, patch))
#else
#  define IMOUOS_SUNPRO_VERSION_CHECK(major,minor,patch)		(0)
#endif

/**
* PELLES编译器探测
*/
#if defined(__POCC__)
#	define IMOUOS_PELLES_VERSION								IMOUOS_VERSION_ENCODE(__POCC__ / 100, __POCC__ % 100, 0)
#endif
#if defined(IMOUOS_PELLES_VERSION)
#  define IMOUOS_PELLES_VERSION_CHECK(major,minor,patch)		(IMOUOS_PELLES_VERSION >= IMOUOS_VERSION_ENCODE(major, minor, patch))
#else
#  define IMOUOS_PELLES_VERSION_CHECK(major,minor,patch)		(0)
#endif

/**
* IAR编译器探测
*/
#if defined(__IAR_SYSTEMS_ICC__)
#	if __VER__ > 1000
#		define IMOUOS_IAR_VERSION								IMOUOS_VERSION_ENCODE((__VER__ / 1000000), ((__VER__ / 1000) % 1000), (__VER__ % 1000))
#	else
#		define IMOUOS_IAR_VERSION								IMOUOS_VERSION_ENCODE(VER / 100, __VER__ % 100, 0)
#	endif
#endif
#if defined(IMOUOS_IAR_VERSION)
#	define IMOUOS_IAR_VERSION_CHECK(major,minor,patch)		(IMOUOS_IAR_VERSION >= IMOUOS_VERSION_ENCODE(major, minor, patch))
#else
#	define IMOUOS_IAR_VERSION_CHECK(major,minor,patch)		(0)
#endif

/**
* GCC编译器探测
*/
#if defined(IMOUOS_GNUC_VERSION) && \
	!defined(__clang__) && \
	!defined(IMOUOS_INTEL_VERSION) && \
	!defined(IMOUOS_PGI_VERSION) && \
	!defined(IMOUOS_ARM_VERSION) && \
	!defined(IMOUOS_TI_VERSION) && \
	!defined(IMOUOS_TI_ARMCL_VERSION) && \
	!defined(IMOUOS_TI_CL430_VERSION) && \
	!defined(IMOUOS_TI_CL2000_VERSION) && \
	!defined(IMOUOS_TI_CL6X_VERSION) && \
	!defined(IMOUOS_TI_CL7X_VERSION) && \
	!defined(IMOUOS_TI_CLPRU_VERSION) && \
	!defined(__COMPCERT__)
#	define IMOUOS_GCC_VERSION									IMOUOS_GNUC_VERSION
#endif
#if defined(IMOUOS_GCC_VERSION)
#  define IMOUOS_GCC_VERSION_CHECK(major,minor,patch)			(IMOUOS_GCC_VERSION >= IMOUOS_VERSION_ENCODE(major, minor, patch))
#else
#  define IMOUOS_GCC_VERSION_CHECK(major,minor,patch)			(0)
#endif

#if defined(__has_extension)
#	define IMOUOS_HAS_EXTENSION(extension)					__has_extension(extension)
#else
#	define IMOUOS_HAS_EXTENSION(extension)					(0)
#endif

#if defined(__has_attribute)
#  define IMOUOS_HAS_ATTRIBUTE(attribute)						__has_attribute(attribute)
#else
#  define IMOUOS_HAS_ATTRIBUTE(attribute)						(0)
#endif

/**
* 通用函数导入导出前缀
*/
#ifdef DAHUA_IMOUOS_DLL_BUILD
#define  IMOUOS_API _declspec(dllexport)
#elif defined DAHUA_IMOUOS_USE
#define  IMOUOS_API _declspec(dllimport)
#else
#define IMOUOS_API
#endif

/**
* 通用内联前缀
*/
#if defined(_MSC_VER) && !defined(__cplusplus) && !defined(inline)
#define inline __inline
#endif

#if IMOUOS_HAS_EXTENSION(attribute_deprecated_with_message) || \
		IMOUOS_GCC_VERSION_CHECK(4,5,0) || \
		IMOUOS_INTEL_VERSION_CHECK(13, 0, 0) || \
		IMOUOS_ARM_VERSION_CHECK(5, 6, 0) || \
		IMOUOS_SUNPRO_VERSION_CHECK(5, 13, 0) || \
		IMOUOS_PGI_VERSION_CHECK(17, 10, 0) || \
		IMOUOS_TI_VERSION_CHECK(18, 1, 0) || \
		IMOUOS_TI_ARMCL_VERSION_CHECK(18, 1, 0) || \
		IMOUOS_TI_CL6X_VERSION_CHECK(8, 3, 0) || \
		IMOUOS_TI_CL7X_VERSION_CHECK(1, 2, 0) || \
		IMOUOS_TI_CLPRU_VERSION_CHECK(2, 3, 0)
#	define IMOUOS_DEPRECATED(since)							__attribute__((__deprecated__("since " #since)))
#	define IMOUOS_DEPRECATED_FOR(since, replacement)			__attribute__((__deprecated__("since " #since "; use " #replacement)))
#elif IMOUOS_HAS_ATTRIBUTE(deprecated) || \
		IMOUOS_GCC_VERSION_CHECK(3, 1, 0) || IMOUOS_ARM_VERSION_CHECK(4,1,0) || IMOUOS_TI_VERSION_CHECK(15,12,0) || \
		(IMOUOS_TI_ARMCL_VERSION_CHECK(4, 8, 0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
		IMOUOS_TI_ARMCL_VERSION_CHECK(5, 2, 0) || \
		(IMOUOS_TI_CL2000_VERSION_CHECK(6, 0, 0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
		IMOUOS_TI_CL2000_VERSION_CHECK(6, 4, 0) || \
		(IMOUOS_TI_CL430_VERSION_CHECK(4, 0, 0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
		IMOUOS_TI_CL430_VERSION_CHECK(4, 3, 0) || \
		(IMOUOS_TI_CL6X_VERSION_CHECK(7, 2, 0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
		IMOUOS_TI_CL6X_VERSION_CHECK(7, 5, 0) || \
		IMOUOS_TI_CL7X_VERSION_CHECK(1, 2, 0) || \
		IMOUOS_TI_CLPRU_VERSION_CHECK(2, 1, 0)
#	define IMOUOS_DEPRECATED(since)							__attribute__((__deprecated__))
#	define IMOUOS_DEPRECATED_FOR(since, replacement)			__attribute__((__deprecated__))
#elif IMOUOS_MSVC_VERSION_CHECK(14,0,0)
#	define IMOUOS_DEPRECATED(since)							__declspec(deprecated("since " #since))
#	define IMOUOS_DEPRECATED_FOR(since, replacement)			__declspec(deprecated("since " #since "; use " #replacement))
#elif IMOUOS_MSVC_VERSION_CHECK(13,10,0) || IMOUOS_PELLES_VERSION_CHECK(6,50,0)
#	define IMOUOS_DEPRECATED(since)							__declspec(deprecated)
#	define IMOUOS_DEPRECATED_FOR(since, replacement)			__declspec(deprecated)
#elif IMOUOS_IAR_VERSION_CHECK(8,0,0)
#	define IMOUOS_DEPRECATED(since)							_Pragma("deprecated")
#	define IMOUOS_DEPRECATED_FOR(since, replacement)			_Pragma("deprecated")
#else
#	define IMOUOS_DEPRECATED(since)
#	define IMOUOS_DEPRECATED_FOR(since, replacement)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
* 平台通用句柄类型
*/
typedef void*												imouos_handle;

/**
* 通用assert宏
*/
#define IMOUOS_ASSERT(x)										assert(x)

#ifdef __cplusplus
}
#endif

#ifndef __cplusplus
#ifndef NULL
#define NULL 0
#endif
#endif

#endif /* __IMOUOS_TYPEDEF_TYPES_H__ */
