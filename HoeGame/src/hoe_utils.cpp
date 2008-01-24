
#include "StdAfx.h"
#include "../include/hoe_utils.h"
#include "../include/hoe_console.h"

BEGIN_HOEGAME

#ifdef _WIN32
#ifndef _WIN32_WCE
#include <dbghelp.h>
#pragma comment (lib,"dbghelp.lib")
#endif
#include <shellapi.h>
#include <shlobj.h>
#endif

#ifdef _LINUX
#define MAX_PATH 512
#endif

bool SetRootFromExe(const tchar * path)
{
	return HoeCore::SetRootDir(HoeCore::GetBaseDir(path));
}

void RemoveEndLine(char * line)
{
	char * p = line;
	while (*p) p++;
	p--;
	while (*p == 13 || *p == 10)
	{
		*p = '\0';
		p--;
		if (p < line)
			break;
	}
}

void RemoveWhiteEndLine(char * line)
{
	char * p = line;
	while (*p) p++;
	p--;
	while (*p == 13 || *p == 10 || *p == ' ' || *p == '\t')
	{
		*p = '\0';
		p--;
		if (p < line)
			break;
	}
}

#ifdef _WIN32_WCE
bool SetRootFromInstance(HINSTANCE hInstance)
{
	return false;
}
#endif

#ifdef _WIN32_WINNT

bool SetRootFromInstance(HINSTANCE hInstance)
{
	tchar moduleName[ MAX_PATH ];
	if ( !GetModuleFileName( hInstance, moduleName, MAX_PATH ) )
	{
		return false;
	}

	return SetRootFromExe(moduleName);
}

int GenerateDump(EXCEPTION_POINTERS* pExceptionPointers)
{
    BOOL bMiniDumpSuccessful;
    //CHAR szPath[MAX_PATH]; 
	HoeCore::String_s<MAX_PATH> szFileName; 
    CHAR* szAppName = "AppName";
    CHAR* szVersion = "v1.0";
    DWORD dwBufferSize = MAX_PATH;
    HANDLE hDumpFile;
    SYSTEMTIME stLocalTime;
    MINIDUMP_EXCEPTION_INFORMATION ExpParam;

    GetLocalTime( &stLocalTime );

	szFileName.printf(T("%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp"), 
               szVersion, 
               stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, 
               stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond, 
               GetCurrentProcessId(), GetCurrentThreadId());
    hDumpFile = CreateFile(szFileName, GENERIC_READ|GENERIC_WRITE, 
                FILE_SHARE_WRITE|FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
	if (hDumpFile==INVALID_HANDLE_VALUE)
	{
		BaseConsole::Printf("Failed open file %s for write dump. (%d)", szFileName,GetLastError());
		return 0;
	}

    ExpParam.ThreadId = GetCurrentThreadId();
    ExpParam.ExceptionPointers = pExceptionPointers;
    ExpParam.ClientPointers = TRUE;

    bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), 
                    hDumpFile, MiniDumpWithDataSegs, &ExpParam, NULL, NULL);

	if (bMiniDumpSuccessful)
		BaseConsole::Printf("Core dump saved to: %s", szFileName);
	else
		BaseConsole::Printf("Failed create core dump.");

    //return EXCEPTION_EXECUTE_HANDLER;
	return 0;
}

LONG WINAPI ExpFilter(EXCEPTION_POINTERS* pExp, DWORD dwExpCode)
{
   BaseConsole::Printf("Error exception.");
   GenerateDump(pExp);
   return EXCEPTION_EXECUTE_HANDLER;
}

#endif

END_HOEGAME
