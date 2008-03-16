
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

#ifdef _LINUX
#include <unistd.h>
#endif

#if defined (_MACOSX)
#include <Carbon/Carbon.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <new>
#include <time.h>

#include <wchar.h>
#ifndef _WIN32_WCE
#include <errno.h>
#endif

#define ENABLE_AUTOCONV_FUNCTIONS 1
#include "../include/HoeCore/hoe_types.h"

#endif // _STDAFX_H_

