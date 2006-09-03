
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

/**
  @brief	Pomocná třída pro nahrávání Hoe enginu. 
 */

class HoeEngine
{
protected:
	static IHoe3DEngine * m_engine; /** < Ukazatel na engine */
	HOE_LIBRARY m_lib; /** < Handle library */
	bool m_loaded;
public:
	/** Kontruktor */
	HoeEngine();
	/** Destruktor */
	~HoeEngine();

	/**
	 * Metoda pro přístup k enginu.
	 */
	static IHoe3DEngine * GetInstance()
	{
		assert(m_engine);
		return m_engine;
	}

	/**
	 * Nahráni enginu.
	 * @param dllname Jméno knihovny
	 * @param con Konzole hry
	 */
	bool Load(const char * dllname, Console * con, XHoeFS * fs);

	/**
	 * Nahráni enginu staticke verze.
	 * @param con Konzole hry
	 */
	bool LoadStatic(Console * con);

	bool IsLoaded() { return m_loaded; }
	/**
	 * pokud je engine nahrán zavolá iHoe3Dengine->Destroy()
	 */
	void Destroy();
};

END_HOEGAME

#endif // _HOE_ENGINE_H_

