
#include "StdAfx.h"
#include "shared.h"
#include "config.h"
#include "hoe_time.h"
#include "hoe3d.h"

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
class XHoeConsole;
class XHoeFS;
extern ::XHoeConsole * _Con;

////////////////
DLL_EXPORT IHoeEngine * HOEAPI CreateHoeEngine(int sdk_ver, ::XHoeConsole * con, ::XHoeFS * _fs, void * p_memBase,int n_memSize, int flags)
{
	if (sdk_ver != HOESDK_VERSION)
	{
		HoeCore::String_s<256> buff;
		buff.printf("Bad version for interface (req: %.2f this: %.2f)",sdk_ver * 0.01f,HOESDK_VERSION * 0.01f);
		con->Con_Print(buff);
		return NULL;
	}

	IHoeEngine * ret;

	if (IfExistHoe3DInstance())
	{
		con->Con_Print(T("One instance of Hoe3D is loaded."));
		return NULL;
	}

	_Con = con;

	assert(shared::fs == NULL);
	shared::fs = _fs;

	ret = new Hoe3D(flags);
	ret->SysFloatTime = SysFloatTime;

	return ret;
}

class EngineInfo : public IHoeEngineInfo
{
	Ref m_ref;

public:
	/** Vypise popisek enginu */
	virtual const char * GetEngineName() { return Config::GetHoeDesc(); }
	virtual const char * GetEngineDescription() { return ""; }
	virtual const char * GetEngineVersion() { return Config::GetBuild(); }
	/** Ziska pocet moznych zarizeni */
	virtual const IHoeEngineInfo::InfoDevice * GetDevice(uint i)
	{
		if (i > 2)
			return NULL;
		static IHoeEngineInfo::InfoDevice dev;
		dev.name = "jmeno";
		dev.description = "desc";
		return &dev;
	}
	virtual const IHoeEngineInfo::InfoFormat * GetFormat(const InfoDevice * dev, uint i)
	{
		return NULL;
	}
	virtual const IHoeEngineInfo::InfoDisplay * GetDisplay(const InfoDevice * dev, const InfoFormat * fmt, uint i)
	{
		return NULL;
	}
};

DLL_EXPORT IHoeEngineInfo * HOEAPI GetEngineInfo(int sdk_ver)
{
	if (sdk_ver != HOESDK_VERSION)
	{
		return NULL;
	}
	static EngineInfo info;
	return &info;
}

DLL_EXPORT int HOEAPI GetSDKVersion()
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

