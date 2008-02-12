
#ifndef _HOE3D_STDAFX_H_
#define _HOE3D_STDAFX_H_

#ifdef _UNICODE
#define No unicode support
#endif

#if defined (_WIN32) //////////////////////////
//#define WINVER 0x0500
//#define _WIN32_WINNT 0x0500

#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <tchar.h>

#ifdef HOE_STATIC_ENGINE
#define LIB_EXTERN
#else
#define LIB_EXTERN extern "C" __declspec( dllexport )
#endif

#pragma warning( disable : 4996)

#elif defined(_LINUX) /////////////////////////

#include <X11/Xlib.h>
#include <dlfcn.h>

#ifdef HOE_STATIC_ENGINE
#define LIB_EXTERN
#else
#define LIB_EXTERN extern "C" 
#endif

#elif defined(_MACOSX)

#include <Carbon/Carbon.h>


#else
#error Platform not defined!
#endif 

#include <stdio.h> 
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include <wchar.h>

#include <HoeCore/hoe_types.h>
#include <HoeCore/hoe_format.h>
#include <HoeCore/hoe_math_ext.h>
#include <HoeCore/hoe_core.h>
#include <HoeCore/hoe_flex.h>
#include <HoeCore/hoe_platform.h>
#include <HoeCore/hoe_stream.h>
#include <HoeCore/hoe_structures.h>

#include <HoeResource/resmgr.h>
#include <HoeResource/hoe_res.h>
#include <HoeResource/color.h>

// Memory managment
//#include "mem_system.h"

#ifdef _USE_OPENGL_
#define _DFMT_OPENGL_
#else // _USE_DIRECTX9_
#define _DFMT_D3D_
#define _DFMT_D3D9_
#endif 


#endif // _HOE3D_STDAFX_H_
