//
//  "$Id: Defs.h 989762 2020-12-03 02:26:11Z 28137 $"
//
//  Copyright (c)1992-2007, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//


#ifndef __PAL_DEFS_H__
#define __PAL_DEFS_H__

#include <stdint.h>

#ifdef _WIN32                 //WIN32 Dynamic Link Library
#ifdef PAL_DLL_BUILD
#define  PAL_API _declspec(dllexport)
#elif defined PAL_DLL_USE
#define  PAL_API _declspec(dllimport)
#else
#define PAL_API
#endif
#elif defined __GNUC__        //__GNUC__
#if __GNUC__ >= 4
#define  PAL_API __attribute__ ((visibility ("default")))
#else
#define PAL_API
#endif
#else                         //others
#define PAL_API
#endif

#endif //__PAL_DEFS_H__

