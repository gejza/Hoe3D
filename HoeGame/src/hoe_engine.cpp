
#include "StdAfx.h"
#include "../include/hoe_engine.h"


BEGIN_HOEGAME

IHoe3DEngine * g_hoeengine = NULL;

HoeEngine::HoeEngine()
{
	m_lib = NULL;
	m_loaded = false;
}

HoeEngine::~HoeEngine()
{
	Destroy();

#ifdef _WIN32
	if (m_lib)
		FreeLibrary(m_lib);
#endif	

#ifdef _LINUX
	if (m_lib)
		dlclose(m_lib);
#endif

}

bool HoeEngine::Load(const char * dllname, Console * con, XHoeFS * fs)
{
	HOE_FUNCCREATE GetEngineInterface;
	
#ifdef _WIN32
	m_lib = LoadLibrary(dllname);
	if (!m_lib)
	{
		con->Printf("Failed load library: %s",dllname);
		return false;
	}

	GetEngineInterface = (HOE_FUNCCREATE)GetProcAddress(m_lib,"_CreateHoeEngine@24");
	if (!GetEngineInterface)
	{
		con->Printf("Nelze ziskat export knihovny.");
		return false;
	}
#endif

#ifdef _LINUX

	m_lib = dlopen (dllname, RTLD_LAZY);
	if (!m_lib) {
		//con->Printf("%s", "nepovedlo"/*dlerror()*/);
		printf("chyba: %s\n",dlerror());
		return false;
	}

	GetEngineInterface =(HOE_FUNCCREATE) dlsym(m_lib, "CreateHoeEngine");
	if (!GetEngineInterface)
	{
		con->Printf("%s",dlerror());
		return false;
	}
#endif

	g_hoeengine = GetEngineInterface(HOESDK_VERSION,con,fs, NULL,0,0);

	if (g_hoeengine)
	{
		m_loaded = true;
		return true;
	}
	else
		return false;
}

bool HoeEngine::LoadStatic(Console * con)
{
	/*this->m_engine = CreateHoeEngine(con,NULL,0,HOESDK_VERSION);

	if (m_engine)
	{
		m_loaded = true;
		return true;
	}
	else*/
		return false;
}

void HoeEngine::Destroy()
{
	if (g_hoeengine)
		g_hoeengine->Destroy();

	g_hoeengine = NULL;
	m_loaded = false;
	// unload
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
	HOE_FUNCINFO func;
	Unload();
	m_lib = LoadLibrary(dllname);
	if (!m_lib)
	{
		return false;
	}

	func = (HOE_FUNCINFO)GetProcAddress(m_lib,"_GetEngineInfo@4");
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
	return true;
}

void HoeEngineInfo::Unload()
{
	if (m_lib)
		FreeLibrary(m_lib);
	m_lib = NULL;
	m_info = NULL;
}

END_HOEGAME

float SysFloatTime()
{
	if (HoeGame::g_hoeengine)
		return HoeGame::g_hoeengine->SysFloatTime();
	return 0.f;
}