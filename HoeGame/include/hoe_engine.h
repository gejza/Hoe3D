
/**
   @file    hoe_engine.h
   @date    Nov 2004
   @version $Revision: 1.4 $
   @brief   Pomocná třída pro nahrávání Hoe enginu.
*/
/*
	$Id: hoe_engine.h,v 1.4 2005/03/12 18:03:13 gejza Exp $
	$Author: gejza $
	$Date: 2005/03/12 18:03:13 $
	$Revision: 1.4 $
*/ 

#ifndef _HOE_ENGINE_H_
#define _HOE_ENGINE_H_

#include <hoe3d.h>
#include "hoe_game.h"
#include "hoe_console.h"

BEGIN_HOEGAME

extern IHoeEngine * g_hoeengine; /** < Ukazatel na engine */

inline IHoeEngine *GetHoeEngine()
{
	return g_hoeengine;
}

/**
  @brief	Pomocná třída pro nahrávání Hoe enginu. 
 */

class HoeEngine
{
protected:
	bool m_loaded;
public:
	/** Kontruktor */
	HoeEngine();
	/** Destruktor */
	~HoeEngine();

	static bool IsLoaded() { return GetHoeEngine() != NULL; }
	virtual bool Load(Console * con, HoeRes::XResourceMgr * fs, int sdkver) = 0;

	/**
	 * pokud je engine nahrán zavolá iHoe3Dengine->Destroy()
	 */
	void Destroy();
};

class HoeEngineDLL : public HoeEngine
{
protected:
	HOE_LIBRARY m_lib; /** < Handle library */
	const tchar* m_dllname;
public:
	HoeEngineDLL();
	virtual ~HoeEngineDLL();
	/**
	 * Nahráni enginu.
	 * @param dllname Jméno knihovny
	 * @param con Konzole hry
	 */
	virtual bool Load(Console * con, HoeRes::XResourceMgr * fs, int sdkver);
};

class HoeEngineStatic : public HoeEngine
{
public:
	virtual bool Load(Console * con, HoeRes::XResourceMgr * fs, int sdkver)
	{
		g_hoeengine = CreateHoeEngine(sdkver, con, fs, 0);
		if (g_hoeengine)
		{
			m_loaded = true;
			return true;
		}
		else
			return false;
	}
};

class HoeEngineInfo
{
	HOE_LIBRARY m_lib; /** < Handle library */
protected:
	IHoeEngineInfo * m_info;
public:
	HoeEngineInfo();
	virtual ~HoeEngineInfo();
	bool Load(const char * dllname);
	void Unload();
	IHoeEngineInfo * GetInfo() { return m_info; }
};

END_HOEGAME

#endif // _HOE_ENGINE_H_

