
#include "StdAfx.h"
#include "../include/hoe_utils.h"
#include "../include/hoe_console.h"

BEGIN_HOEGAME

#ifdef _WIN32
#ifndef _WIN32_WCE
#include <dbghelp.h>
#endif
#include <shellapi.h>
#include <shlobj.h>
#pragma comment (lib,"dbghelp.lib")
#endif

#ifdef _LINUX
#define MAX_PATH 512
#endif

bool SetRootDir(const char * dir)
{
#ifdef _WIN32
	return (SetCurrentDirectory(dir) != FALSE);
#endif
#ifdef _LINUX
	return (chdir(dir) == 0);
#endif
}

const char * GetBaseDir(const char * path)
{
	static char basedir[ 1024 ];
	int j;

	strcpy( basedir, path );

	for (j=strlen(basedir)-1;j > 0;j--)
	{
 		if (basedir[j] == '/' || basedir[j] == '\\')
		{
			basedir[j] = '\0';
			return basedir;
		}
	}

	return ".";
}

bool SetRootFromExe(const char * path)
{
	return SetRootDir(GetBaseDir(path));
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

#ifdef _WIN32
bool SetRootFromInstance(HINSTANCE hInstance)
{
	char moduleName[ MAX_PATH ];
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
    CHAR szFileName[MAX_PATH]; 
    CHAR* szAppName = "AppName";
    CHAR* szVersion = "v1.0";
    DWORD dwBufferSize = MAX_PATH;
    HANDLE hDumpFile;
    SYSTEMTIME stLocalTime;
    MINIDUMP_EXCEPTION_INFORMATION ExpParam;

    GetLocalTime( &stLocalTime );

    _snprintf( szFileName, MAX_PATH, "%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp", 
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
