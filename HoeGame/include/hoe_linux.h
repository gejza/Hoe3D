
#ifndef _HOE_GAME_LINUX_H_
#define _HOE_GAME_LINUX_H_

#include "hoe_game.h"
#include "hoe_baseapp.h"
#include "hoe_console.h"

BEGIN_HOEGAME 

class HoeLinux : public HoeBaseApp
{
protected:
	Display * dpy;
	int screen;
	Window win;
	IHoe3DEngine * m_enginstance;
public:
	HoeLinux();
	virtual ~HoeLinux();
	
	virtual const char * GetAppName() = 0;

	virtual void HandleError();

	virtual int GetMsg(IHoe3DEngine *);
	virtual void Destroy();

	virtual void OnSize(int width, int height) = 0;
	virtual void OnPaint() = 0;

	virtual void SetTitle(const char * title);

	virtual void PaintConsole(Console *);

	virtual void UpdateConsole();

	virtual int ShowMsg(const char *) {}
};

END_HOEGAME

#endif // _HOE_GAME_WIN32_H_

