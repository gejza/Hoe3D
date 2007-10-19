
#include "StdAfx.h"
#include "ref.h"
#include "utils.h"
#include "../include/hoeinterfaces.h"
#include <hoe_math.h>
#include <hoe_log.h>

#ifdef _HOE_D3D9_
#include <dxerr9.h>
#pragma comment (lib,"dxerr9.lib")
#endif

//////// Conzole /////////////
XHoeConsole * _Con;

void Con_Print(const wchar_t * szFormat,...)
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

void Con_Print(const char * szFormat,...)
{
	/*if (_Con)
	{
		static char szBuff[1024];

		va_list args;

		va_start(args, szFormat);
		vsnprintf( szBuff, 1024, szFormat, args );
		va_end(args);

		_Con->Con_Print(szBuff);
	}*/
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

void Con_Print(HoeLog * log, const char * szFormat,...)
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

void Con_Print(HoeMath::Matrix &m)
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
		__debugbreak();
	}
}
#endif
#ifdef _HOE_D3DM_

void d3derr(const char * file, dword line, const char * fnc, const char *ffnc,HRESULT hRes)
{
	if (FAILED(hRes))
	{
		HoeCore::String_s<2048> buff;
		buff.printf("File: %s\n"
			"Line: %d\n"
			"In Function: %s\n"
			"Function: %s\n"
			"HRESULT: %s\n"
			"Program will exit..", file, line, ffnc,fnc, Ref::GetErrorString(hRes));
		Con_Print("HRESULT failed!");
		Con_Print("%s",buff);

		MessageBox(GetActiveWindow(), buff, T("HRESULT failed!"), MB_OK);
		// call stack
		__debugbreak();
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
		__debugbreak();
#else
		exit(1);
#endif
	}
}
#endif



