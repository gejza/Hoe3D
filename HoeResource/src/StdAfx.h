
#ifndef _STDAFX_H_
#define _STDAFX_H_

#if defined (_WIN32) //////////////////////////
//#define WINVER 0x0500
//#define _WIN32_WINNT 0x0500
//#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <tchar.h>
#include <math.h>
#endif

#if defined (_MACOSX)
#include <Carbon/Carbon.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include <wchar.h>
#include <new>

#include <HoeCore/hoe_types.h>
#include <HoeCore/hoe_format.h>
#include <HoeCore/hoe_string.h>
#include <HoeCore/hoe_stream.h>
#include <HoeCore/hoe_structures.h>
#include <HoeCore/hoe_platform.h>

#include <zlib.h>


#endif // _STDAFX_H_

