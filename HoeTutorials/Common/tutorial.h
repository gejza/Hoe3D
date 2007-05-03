
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
#endif

#include <assert.h>

#include <hoe_core.h>
#include <hoe3d.h>
#include <hoe_console.h>
#include <hoe_engine.h>
#include <hoe_config.h>
#include <hoe_app.h>
#include <hoe_utils.h>

extern const char * g_TutorialName;

class HoeTutorial : public HoeGame::HoeApp
{
public:
	HoeTutorial(HOE_INSTANCE instance, HoeGame::Console * con);
	virtual const char * GetAppName() { return g_TutorialName; }
	virtual bool InitTutorial(HOE_TYPE_SCENE ts);
};

#endif // _HOE_TUTORIAL_APP_H_

