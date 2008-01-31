
#ifndef _HOE_GAME_LINUX_H_
#define _HOE_GAME_LINUX_H_

#include "hoe_game.h"
#include "hoe_baseapp.h"
#include "hoe_console.h"

BEGIN_HOEGAME 

class HoeLinux : public HoeBaseApp
{
protected:
	HOE_DISPLAY * dpy;
	int screen;
	HOE_WINDOW win;
	IHoeEngine * m_enginstance;
public:
	HoeLinux();
	virtual ~HoeLinux();
	
	virtual const tchar * GetAppName() = 0;

	virtual void HandleError();

	virtual int GetMsg(IHoeEngine *);
	virtual void Destroy();

	virtual void OnSize(int width, int height) = 0;
	virtual void OnPaint() = 0;

	virtual void SetTitle(const tchar * title);

	virtual void PaintConsole(Console *);

	virtual void UpdateConsole();

	virtual int ShowMsg(const tchar * caption, const tchar *text) {}
};

END_HOEGAME

#endif // _HOE_GAME_WIN32_H_

