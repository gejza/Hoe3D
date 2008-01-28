
#include "StdAfx.h"
#include "hoe2d.h"
/*#include "shared.h"
#include "config.h"
#include "hoe_time.h"
#include "hoe.h"*/

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
class XHoeConsole;
class XHoeResMgr;
extern ::XHoeConsole * _Con;

////////////////
LIB_EXTERN IHoeEngine * HOEAPI CreateHoeEngine(int sdk_ver, ::XHoeConsole * con, ::XHoeResMgr * _fs, int flags)
{
	IHoeEngine * ret;
	/*if (sdk_ver != HOESDK_VERSION)
	{
		HoeCore::String_s<256> buff;
		buff.printf("Bad version for interface (req: %.2f this: %.2f)",sdk_ver * 0.01f,HOESDK_VERSION * 0.01f);
		con->Con_Print(buff);
		return NULL;
	}


	if (IfExistHoe3DInstance())
	{
		con->Con_Print(T("One instance of Hoe3D is loaded."));
		return NULL;
	}

	_Con = con;

	assert(shared::fs == NULL);
	shared::fs = _fs;

	ret = new Hoe3D(flags);*/
	ret = new Hoe2DEngine(flags);
	ret->SysFloatTime = SysFloatTime;

	return ret;
}

LIB_EXTERN IHoeEngineInfo * HOEAPI GetEngineInfo(int sdk_ver)
{
	/*if (sdk_ver != HOESDK_VERSION)
	{
		return NULL;
	}
	static EngineInfo info;
	return &info;
	*/
	return 0;
}

LIB_EXTERN int HOEAPI GetSDKVersion()
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

