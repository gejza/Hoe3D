// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#ifndef _HXRES_STD_AFX_H_
#define _HXRES_STD_AFX_H_

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#endif
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdarg.h>

#include <assert.h>
//#include <tchar.h>
#include <hoe_types.h>
#include <hoe_string.h>
#include <hoe_structures.h>
#include <hoe_universal.h>
#include <hoe_file.h>
#include <hoe_platform.h>

#include <hoe_resfile.h>
#include <hoe_utils.h>

// TODO: reference additional headers your program requires here

#endif // _HXRES_STD_AFX_H_

