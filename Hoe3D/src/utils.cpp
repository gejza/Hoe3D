
#include "StdAfx.h"
#include "ref.h"
#include "utils.h"
#include "../include/hoeinterfaces.h"

#ifdef _HOE_D3D9_
#include <dxerr9.h>
#pragma comment (lib,"dxerr9.lib")
#endif

//////// Conzole /////////////
XHoeConsole * _Con;

void Con_Print(const tchar * szFormat,...)
{
	if (_Con)
	{
		static tchar szBuff[1024];

		va_list args;

		va_start(args, szFormat);
        HoeCore::string::vsnprintf( szBuff, 1024, szFormat, args );
		va_end(args);

		_Con->Con_Print(szBuff);
	}
}

void Con_Print(int dlevel, const tchar * szFormat,...)
{
	if (dlevel > 2) return;

	if (_Con)
	{
		static tchar szBuff[1024];

		va_list args;

		va_start(args, szFormat);
        HoeCore::string::vsnprintf( szBuff, 1024, szFormat, args );
		va_end(args);

		_Con->Con_Print(szBuff);
	}
}

void Con_Print(HoeLog * log, const tchar * szFormat,...)
{
	if (log)
	{
		static tchar szBuff[1024];

		va_list args;

		va_start(args, szFormat);
        HoeCore::string::vsnprintf( szBuff, 1024, szFormat, args );
		va_end(args);

		log->Log("%s",szBuff);
	}
}

void Con_Print(HoeMath::Matrix4f &m)
{
	Con_Print(T("%f %f %f %f"), m._11,m._12,m._13,m._14);
	Con_Print(T("%f %f %f %f"), m._21,m._22,m._23,m._24);
	Con_Print(T("%f %f %f %f"), m._31,m._32,m._33,m._34);
	Con_Print(T("%f %f %f %f"), m._41,m._42,m._43,m._44);
}

int string_ex(const tchar * ex,const tchar * str)
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
		Con_Print(T("Call PostQuitMessage() with code: %d"),code);
		PostQuitMessage(code);
	}
	else
#endif
	{
		Con_Print(T("Call exit() with code: %d"),code);
		exit(code);
	}
}


#ifdef _WIN32
void HoeDebugBreak()
{
    __try 
    {
        __debugbreak();
    }
    __except(GetExceptionCode() == EXCEPTION_BREAKPOINT ? 
             EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) 
    {
        // No debugger is attached, so return FALSE 
        // and continue.
        return;
    }
}
#endif

//////////////////////////////
#if defined (_HOE_D3D_) || defined(_HOE_DD_) || defined(_HOE_D3DM_)

void dxerr(const tchar * file, dword line, const tchar * fnc, const tchar *ffnc,HRESULT hRes)
{
	if (FAILED(hRes))
	{
		HoeCore::String_s<2048> buff;
		buff.printf(T("File: %s\n")
			T("Line: %d\n")
			T("In Function: %s\n")
			T("Function: %s\n")
			T("HRESULT: %s\n")
			T("Program will exit.."), file, line, ffnc,fnc, Ref::GetErrorString(hRes));
		Con_Print(T("HRESULT failed!"));
		Con_Print(T("%s"),buff);

		MessageBox(GetActiveWindow(), buff, T("HRESULT failed!"), MB_OK);
		// call stack
		DebugBreak();
	}
}
#endif
//////////////////////////////
#ifdef _HOE_OPENGL_
const char * glstrerror(int code)
{
	switch (code)
	{
	case GL_INVALID_ENUM:
		return "INVALID_ENUM";
	case GL_INVALID_VALUE:
		return "INVALID_VALUE";
	case GL_INVALID_OPERATION:
		return "INVALID_OPERATION";
	case GL_STACK_OVERFLOW:
		return "STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW:
		return "STACK_UNDERFLOW";
	case GL_OUT_OF_MEMORY:
		return "OUT_OF_MEMORY";
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
	   DebugBreak();
#else
		exit(1);
#endif
	}
}
#endif



