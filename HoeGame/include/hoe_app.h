
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

#ifdef _WIN32
#include "hoe_win32.h"
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
#ifdef _WIN32
class HoeApp : public HoeWin32
#endif
#ifdef _LINUX
class HoeApp : public HoeLinux
#endif
#ifdef _MACOSX
class HoeApp : public HoeMacOsX
#endif
{
protected:
	HoeEngine m_engine; 
	Console * m_con;
	HoeGame::FileSystem m_fs;

	static CVar m_width;
	static CVar m_height;
	static CVar m_fullscreen;
	static CVar m_enginedll;

	const char * m_lastError;

	virtual bool LoadEngine(int sdkver);
	virtual bool Frame(float time);
	virtual bool Frame();
public:
	/** 
	 * Kontruktor
	 * @param con Konzole hry
	 */
	HoeApp(HOE_INSTANCE,Console * con);
	/** Destruktor */
	virtual ~HoeApp();

	virtual bool Init(const char * title, int sdkver);
	virtual void Run();
	virtual void OnSize(int width, int height);
	virtual void OnPaint();

	virtual const char * GetTitle();

	static BaseConsole * GetConsole()
	{
		return GET_THIS(HoeApp)->m_con;
	}

	static IHoe3DEngine * GetEngine()
	{
		assert(HoeGame::GetHoeEngine());
		return HoeGame::GetHoeEngine();
	}

	static FileSystem * GetFS()
	{
		return &(GET_THIS(HoeApp)->m_fs);
	}

	virtual void OnUpdate(float time);

	void DestroyEngine();
};


END_HOEGAME

#endif // _HOE_GAME_APP_H_

