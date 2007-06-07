
#ifndef _STDAFX_H_
#define _STDAFX_H_

#ifdef _XBOX
#include <xtl.h>
#else
#ifdef _WIN32
//#define _WIN32_WINNT 0x0500
#include <windows.h>
#endif
#endif // _XBOX

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

#ifdef __cplusplus
#include <vector>
#endif


#ifdef _LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#endif

#ifdef _LINUX
#include <Carbon/Carbon.h>
#endif

#include <assert.h>
#ifdef __cplusplus 
#include <hoe_core.h>
#include <hoe_types.h>
#include <hoe_math.h>
#endif


#ifdef _WIN32
//#define _WIN32_WINNT 0x0500
#define snprintf _snprintf
#endif

#endif // _STDAFX_H_
