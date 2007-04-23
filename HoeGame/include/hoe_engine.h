
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

extern IHoe3DEngine * g_hoeengine; /** < Ukazatel na engine */

inline IHoe3DEngine *GetHoeEngine()
{
	return g_hoeengine;
}

/**
  @brief	Pomocná třída pro nahrávání Hoe enginu. 
 */

class HoeEngine
{
protected:
	HOE_LIBRARY m_lib; /** < Handle library */
	bool m_loaded;
public:
	/** Kontruktor */
	HoeEngine();
	/** Destruktor */
	~HoeEngine();

	/**
	 * Nahráni enginu.
	 * @param dllname Jméno knihovny
	 * @param con Konzole hry
	 */
	bool Load(const char * dllname, Console * con, XHoeFS * fs, int sdkver);

	/**
	 * Nahráni enginu staticke verze.
	 * @param con Konzole hry
	 */
	bool LoadStatic(Console * con);

	static bool IsLoaded() { return GetHoeEngine() != NULL; }
	/**
	 * pokud je engine nahrán zavolá iHoe3Dengine->Destroy()
	 */
	void Destroy();
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

