
#include "StdAfx.h"
#include "shared.h"
#include "config.h"
#include "hoe_time.h"
#include "hoe.h"

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
class XHoeConsole;
class XHoeFS;
extern ::XHoeConsole * _Con;

////////////////
//DLL_EXPORT 
IHoe3DEngine * HOEAPI CreateHoeEngine(int sdk_ver, ::XHoeConsole * con, ::XHoeFS * _fs, void * p_memBase,int n_memSize, int flags)
{
	if (sdk_ver != HOESDK_VERSION)
	{
		char buff[256];
		sprintf(buff,"Bad version for interface (req: %.2f this: %.2f)",sdk_ver * 0.01f,HOESDK_VERSION * 0.01f);
		con->Con_Print(buff);
		return NULL;
	}

	IHoe3DEngine * ret;

	if (IfExistHoe3DInstance())
	{
		con->Con_Print("One instance of Hoe3D is loaded.");
		return NULL;
	}

	_Con = con;

	assert(shared::fs == NULL);
	shared::fs = _fs;

	ret = new Hoe3D(flags);
	ret->SysFloatTime = SysFloatTime;

	return ret;
}

//DLL_EXPORT 
IHoeEngineInfo * HOEAPI GetEngineInfo(int sdk_ver)
{
	if (sdk_ver != HOESDK_VERSION)
	{
		return NULL;
	}
	static IHoeEngineInfo info;
	return &info;
}

//DLL_EXPORT 
int HOEAPI GetSDKVersion()
{
	return HOESDK_VERSION;
}

#if defined (_WIN32) && !defined (HOE_STATIC_ENGINE) 

BOOL WINAPI DllMain(HINSTANCE,DWORD,LPVOID)
{
	return TRUE;
}

#endif // _WIN32
#ifdef _LINUX
int main()
{
    printf("Hoe3D engine\n");
}
#endif

