// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef _HCGEN_STD_AFX_H_
#define _HCGEN_STD_AFX_H_

#pragma once

/*#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif	*/					
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#endif
#include <stdio.h>
#include <wchar.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>
#include <new>
#include <stdarg.h>
#include <stdlib.h>

#include <HoeCore/hoe_types.h>
#include <HoeCore/hoe_format.h>
#include <HoeCore/hoe_string.h>
#include <HoeCore/hoe_structures.h>
#include <HoeCore/hoe_universal.h>
#include <HoeCore/hoe_file.h>
#include <HoeCore/hoe_unicode.h>
#include <HoeCore/hoe_platform.h>
#include <HoeCore/const_file.h>

#include <HoeUtils/hoe_utils.h>
#include <HoeUtils/error.h>
#include <HoeUtils/text.h>

#endif // _HCGEN_STD_AFX_H_

