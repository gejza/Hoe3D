
/**
   @file    hoe_app.h
   @date    Oct 2004
   @version $Revision: 1.11 $
   @brief   Hlavní aplikace
*/


#ifndef _HOE_GAME_APP_H_
#define _HOE_GAME_APP_H_

#include "hoe_vars.h"
#include "hoe_console.h"
#include "hoe_engine.h"
#include "hoe_fs.h"

#ifdef _WIN32_WINNT
#include "hoe_win32.h"
#endif

#ifdef _WIN32_WCE
#include "hoe_mobile.h"
#endif

#ifdef _LINUX
#include "hoe_linux.h"
#endif

#ifdef _MACOSX
#include "hoe_macosx.h"
#endif


BEGIN_HOEGAME 

/**
 * @brief Hlavní aplikace
 */
#ifdef _WIN32_WINNT
class HoeApp : public HoeWin32
#endif
#ifdef _WIN32_WCE
class HoeApp : public HoeMobile
#endif
#ifdef _LINUX
class HoeApp : public HoeLinux
#endif
#ifdef _MACOSX
class HoeApp : public HoeMacOsX
#endif
{
protected:
	Console * m_con;
	HoeGame::HoeEngine& m_engine;
	HoeRes::XResourceMgr * m_fs;

	static CVar m_width;
	static CVar m_height;
	static CVar m_fullscreen;

	const tchar * m_lastError;

	virtual bool LoadEngine(int sdkver);
	virtual bool Frame(float time);
	virtual bool Frame();
public:
	static CVar m_enginedll;
	/** 
	 * Kontruktor
	 * @param con Konzole hry
	 */
	HoeApp(HOE_INSTANCE,HoeEngine& engine, Console * con, HoeRes::XResourceMgr * fs);
	/** Destruktor */
	virtual ~HoeApp();

	virtual bool Init(const tchar * title, int sdkver);
	virtual void Run();
	virtual void OnSize(int width, int height);
	virtual void OnPaint();

	virtual const tchar * GetTitle();

	static BaseConsole * GetConsole()
	{
		return GET_THIS(HoeApp)->m_con;
	}

	static IHoeEngine * GetEngine()
	{
		assert(HoeGame::GetHoeEngine());
		return HoeGame::GetHoeEngine();
	}

	/*static FileSystem * GetFS()
	{
		return &(GET_THIS(HoeApp)->m_fs);
	}*/

	virtual void OnUpdate(float time);

	void DestroyEngine();
};


END_HOEGAME

#endif // _HOE_GAME_APP_H_

