
#ifndef _OS_SYSTEM_H_
#define _OS_SYSTEM_H_

#if defined (_WIN32) //////////////////////////
//#define WINVER 0x0500
#define _WIN32_WINNT 0x0500

#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <tchar.h>

#define vsnprintf _vsntprintf

#ifdef HOE_STATIC_ENGINE
#define DLL_EXPORT
#else
#define DLL_EXPORT extern "C" __declspec( dllexport )
#endif

#define DEFAULT_HOEFS_NAME "hoefs.dll"

#pragma warning( disable : 4996)

#elif defined(_LINUX) /////////////////////////

#include <X11/Xlib.h>
#include <dlfcn.h>

#define DLL_EXPORT extern "C"
#define DEFAULT_HOEFS_NAME "hoefs.so"

#elif defined(_MACOSX)

#define DLL_EXPORT extern "C"
#define DEFAULT_HOEFS_NAME "hoefs.so"

#else
#error Platform not defined!
#endif 

#include <stdio.h> 
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include <hoe_types.h>
#include <hoe_math_ext.h>
#include <hoe_core.h>

// Memory managment
//#include "mem_system.h"

#ifdef _USE_OPENGL_
#define _DFMT_OPENGL_
#else // _USE_DIRECTX9_
#define _DFMT_D3D_
#define _DFMT_D3D9_
#endif 

#endif //_OS_SYSTEM_H_
