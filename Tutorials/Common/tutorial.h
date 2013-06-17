
#ifndef _HOE_TUTORIAL_APP_H_
#define _HOE_TUTORIAL_APP_H_

#ifdef _WIN32
//#define _WIN32_WINNT 0x0500
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>


#ifdef _LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <string.h>
#include <wchar.h>
#endif

#include <assert.h>

#include <HoeCore/hoe_stream.h>
#include <HoeGame/hoe_app.h>
#include <HoeGame/hoe_config.h>
#include <HoeResource/resmgr.h>

extern const tchar * g_TutorialName;

class HoeTutorial : public HoeGame::HoeApp, public HoeRes::XResourceMgr
{
	HoeGame::HoeEngineDLL m_engine;
public:
	HoeTutorial(HOE_INSTANCE instance, HoeGame::Console * con);
	virtual const tchar * GetAppName() { return g_TutorialName; }
	virtual bool InitTutorial(HOE_TYPE_SCENE ts);
	virtual HoeCore::ReadStream * GetResource(const tchar* name, const tchar** ns = NULL)
	{
		return NULL;
	}
};

#endif // _HOE_TUTORIAL_APP_H_

