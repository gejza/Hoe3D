
#include "StdAfx.h"
#include "../include/hoe_engine.h"


BEGIN_HOEGAME

IHoeEngine * g_hoeengine = NULL;

HoeEngine::HoeEngine()
{
}

HoeEngine::~HoeEngine()
{
	Destroy();
}

void HoeEngine::Destroy()
{
	if (g_hoeengine)
		g_hoeengine->Destroy();

	g_hoeengine = NULL;
	m_loaded = false;
	// unload
}

// HoeEngineDLL
HoeEngineDLL::HoeEngineDLL()
{
	m_lib = NULL;
}

HoeEngineDLL::~HoeEngineDLL()
{

#ifdef _WIN32
	if (m_lib)
		FreeLibrary(m_lib);
#endif	

#ifdef _LINUX
	if (m_lib)
		dlclose(m_lib);
#endif

}

bool HoeEngineDLL::Load(Console * con, HoeRes::XResourceMgr * fs, int sdkver)
{
	HOE_FUNCCREATE GetEngineInterface;
	
	if (sdkver != HOESDK_VERSION)
	{
		con->Printf(T("Bad version for HoeGame interface (req: %.2f editor: %.2f)"),sdkver * 0.01f,HOESDK_VERSION * 0.01f);
		return false;
	}

#ifdef _WIN32
	m_lib = LoadLibrary(m_dllname);
	if (!m_lib)
	{
		con->Printf(T("Failed load library: %s"),m_dllname);
		return false;
	}

	GetEngineInterface = (HOE_FUNCCREATE)GetProcAddress(m_lib,"_CreateHoeEngine@24");
	if (!GetEngineInterface)
	{
		con->Printf(T("Nelze ziskat export knihovny."));
		return false;
	}
#endif

#ifdef _LINUX

	m_lib = dlopen (m_dllname, RTLD_LAZY);
	if (!m_lib) {
		//con->Printf("%s", "nepovedlo"/*dlerror()*/);
		printf("error: %s\n",dlerror());
		return false;
	}

	GetEngineInterface =(HOE_FUNCCREATE) dlsym(m_lib, "CreateHoeEngine");
	if (!GetEngineInterface)
	{
		con->Printf("%s",dlerror());
		return false;
	}
#endif
	g_hoeengine = GetEngineInterface(HOESDK_VERSION,con,fs, 0);
	if (g_hoeengine)
	{
		m_loaded = true;
		return true;
	}
	else
		return false;
}

//////////////////////////////////////////////
HoeEngineInfo::HoeEngineInfo()
{
	m_lib = 0;
	m_info = NULL;
}

HoeEngineInfo::~HoeEngineInfo()
{
	Unload();
}

bool HoeEngineInfo::Load(const char * dllname)
{
#ifdef _WIN32
	HOE_FUNCINFO func;
	Unload();
	m_lib = LoadLibraryW(L"dllname");
	if (!m_lib)
	{
		return false;
	}
#ifdef _WIN32_WCE
	func = (HOE_FUNCINFO)GetProcAddress(m_lib,L"_GetEngineInfo@4");
#else
	func = (HOE_FUNCINFO)GetProcAddress(m_lib,"_GetEngineInfo@4");
#endif
	if (func == NULL)
	{
		Unload();
		return false;
	}
	m_info = func(HOESDK_VERSION);
	if (m_info == NULL)
	{
		Unload();
		return false;
	}
#endif
	return true;
}

void HoeEngineInfo::Unload()
{
	if (m_info)
	{
		m_info->Release();
		m_info = NULL;
	}
#ifdef _WIN32
    if (m_lib)
		FreeLibrary(m_lib);
#endif
	m_lib = NULL;
	m_info = NULL;
}

END_HOEGAME

/*#ifndef STATIC_ENGINE
vfloat SysFloatTime()
{
	if (HoeGame::g_hoeengine)
    {
        hoe_assert(HoeGame::g_hoeengine->SysFloatTime != NULL);
	return HoeGame::g_hoeengine->SysFloatTime();
	}
	return 0.f;
}
#endif*/


