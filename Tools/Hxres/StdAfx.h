// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#ifndef _HXRES_STD_AFX_H_
#define _HXRES_STD_AFX_H_

//#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
//#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
//#endif						

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#endif
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdarg.h>
#include <new>
#include <math.h>

#include <zlib.h>

#include <assert.h>
//#include <tchar.h>
#include <HoeCore/hoe_types.h>
#include <HoeCore/hoe_format.h>
#include <HoeCore/hoe_string.h>
#include <HoeCore/hoe_structures.h>
#include <HoeCore/hoe_universal.h>
#include <HoeCore/hoe_file.h>
#include <HoeCore/hoe_unicode.h>
#include <HoeCore/hoe_platform.h>
#include <HoeCore/map.h>

#include <HoeResource/hoe_resfile.h>
#include <HoeResource/hoe_res.h>
#include <HoeResource/codec.h>
#include <HoeUtils/hoe_utils.h>
#include <HoeUtils/error.h>

#endif // _HXRES_STD_AFX_H_

