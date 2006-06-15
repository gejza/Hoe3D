
#include "system.h"
#include "ref.h"
#include "utils.h"
#include "../include/hoeinterfaces.h"
#include "hoe3d_math.h"

#ifdef _HOE_D3D9_
#include <dxerr9.h>
#pragma comment (lib,"dxerr9.lib")
#endif
#ifdef _WIN32
#include <dbghelp.h>
#include <shellapi.h>
#include <shlobj.h>
#pragma comment (lib,"dbghelp.lib")

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
		Con_Print("Failed open file %s for write dump. (%d)", szFileName,GetLastError());
		return 0;
	}

    ExpParam.ThreadId = GetCurrentThreadId();
    ExpParam.ExceptionPointers = pExceptionPointers;
    ExpParam.ClientPointers = TRUE;

    bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), 
                    hDumpFile, MiniDumpWithDataSegs, &ExpParam, NULL, NULL);

	if (bMiniDumpSuccessful)
		Con_Print("Core dump saved to: %s", szFileName);
	else
		Con_Print("Failed create core dump.");

    //return EXCEPTION_EXECUTE_HANDLER;
	return 0;
}
#endif

//////// Conzole /////////////
XHoeConsole * _Con;

void Con_Print(const char * szFormat,...)
{
	if (_Con)
	{
		static char szBuff[1024];

		va_list args;

		va_start(args, szFormat);
		vsnprintf( szBuff, 1024, szFormat, args );
		va_end(args);

		_Con->Con_Print(szBuff);
	}
}

void Con_Print(int dlevel, const char * szFormat,...)
{
	if (dlevel > 2) return;

	if (_Con)
	{
		static char szBuff[1024];

		va_list args;

		va_start(args, szFormat);
		vsnprintf( szBuff, 1024, szFormat, args );
		va_end(args);

		_Con->Con_Print(szBuff);
	}
}

void Con_Print(HoeLog * log, const char * szFormat,...)
{
	if (_Con)
	{
		static char szBuff[1024];

		va_list args;

		va_start(args, szFormat);
		vsnprintf( szBuff, 1024, szFormat, args );
		va_end(args);

		log->Log("%s",szBuff);
	}
}

void Con_Print(HoeMath::MATRIX &m)
{
	Con_Print("%f %f %f %f", m._11,m._12,m._13,m._14);
	Con_Print("%f %f %f %f", m._21,m._22,m._23,m._24);
	Con_Print("%f %f %f %f", m._31,m._32,m._33,m._34);
	Con_Print("%f %f %f %f", m._41,m._42,m._43,m._44);
}

int string_ex(const char * ex,const char * str)
{
	int i;
	for (i=0;ex[i];i++)
	{
		if (ex[i] < str[i])
			return 1;
		else if (ex[i] > str[i])
			return -1;
	}
	if (str[i] == '\0' || str[i] == ' ')
		return 0;
	else
		return 1;
}

void QuitGame(bool hard, int code)
{
#ifdef _WIN32
	if (!hard)
	{
		Con_Print("Call PostQuitMessage() with code: %d",code);
		PostQuitMessage(code);
	}
	else
#endif
	{
		Con_Print("Call exit() with code: %d",code);
		exit(code);
	}
}

void HoeLog::Log(const char * szFormat,...)
{
}

HoeLogConsole::HoeLogConsole(const char * name)
{
	strcpy(m_name,name);
}

void HoeLogConsole::Log(const char * szFormat,...)
{
	static char szBuff[1024];
	int lnsize;
	sprintf(szBuff,"%s: ",m_name);
	lnsize = strlen(szBuff);

	va_list args;

	va_start(args, szFormat);
	vsnprintf( szBuff + lnsize, 1024 - lnsize, szFormat, args );
	va_end(args);

	_Con->Con_Print(szBuff);
}

//////////////////////////////
#ifdef _HOE_D3D_


void d3derr(const char * file, dword line, const char * fnc, const char *ffnc,HRESULT hRes)
{
	if (FAILED(hRes))
	{
		char buff[2048];
#ifdef _HOE_D3D9_
		_snprintf(buff, sizeof(buff)-1,
			"File: %s\nLine: %d\nIn Function: %s\nFunction: %s\nHRESULT: %s\nProgram will exit..", file, line, ffnc,fnc, DXGetErrorString9(hRes));
#else
		_snprintf(buff, sizeof(buff)-1,
			"File: %s\nLine: %d\nIn Function: %s\nFunction: %s\nHRESULT: 0x%x\nProgram will exit..", file, line, ffnc,fnc, hRes);
#endif
		Con_Print("HRESULT failed!");
		Con_Print("%s",buff);

		MessageBox(GetActiveWindow(), buff, "HRESULT failed!", MB_OK);
		// call stack
		BEGIN_TRY
		__debugbreak();
		throw("aa");
		END_TRY(exit(1))
	}
}
#endif
//////////////////////////////
#ifdef _WIN32
#define snprintf _snprintf
#endif

#ifdef _HOE_OPENGL_
const char * glstrerror(int code)
{
	switch (code)
	{
	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION";
	case GL_STACK_OVERFLOW:
		return "GL_STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW:
		return "GL_STACK_UNDERFLOW";
	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY";
	default:
		return NULL;
	};
}

void glerr(const char * file, dword line, const char * fnc, const char *ffnc, int code)
{
	if (code!=GL_NO_ERROR)
	{
		char buff[2048];
		const char * strerr = glstrerror(code);
		if (strerr)
		{
		snprintf(buff, sizeof(buff)-1,
			"File: %s\nLine: %d\nIn Function: %s\nFunction: %s\nglGetError: %s\nProgram will exit..", file, line, ffnc,fnc, strerr);
		}
		else
		{
		snprintf(buff, sizeof(buff)-1,
			"File: %s\nLine: %d\nIn Function: %s\nFunction: %s\nglGetError: %x\nProgram will exit..", file, line, ffnc,fnc, (int)code);
		}
		Con_Print("glGetError return %x!", (int)code);
		Con_Print("%s",buff);
#ifdef _WIN32
		MessageBox(GetActiveWindow(), buff, "glGetError failed!", MB_OK);
		// call stack
		BEGIN_TRY
		__debugbreak();
		throw("aa");
		END_TRY(exit(1))
#else
		exit(1);
#endif
	}
}
#endif

#ifdef _WIN32
LONG WINAPI ExpFilter(EXCEPTION_POINTERS* pExp, DWORD dwExpCode)
{
	Con_Print("Error exception.");
   GenerateDump(pExp);
   return EXCEPTION_EXECUTE_HANDLER;
}
#endif

