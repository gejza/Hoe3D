
#ifndef _HOE_GAME_MACOSX_H_
#define _HOE_GAME_MACOSX_H_

#include "hoe_game.h"
#include "hoe_baseapp.h"
#include "hoe_console.h"

namespace HoeGame { 

class HoeMacOsX : public HoeBaseApp
{
protected:
	HOE_DISPLAY * dpy;
	int screen;
	HOE_WINDOW win;
	IHoe3DEngine * m_enginstance;
    IBNibRef m_nibRef;
public:
	HoeMacOsX();
	virtual ~HoeMacOsX();
	
	virtual const char * GetAppName() = 0;

	virtual void HandleError();

	virtual int GetMsg(IHoe3DEngine *);
	virtual void Destroy();

	virtual void OnSize(int width, int height) = 0;
	virtual void OnPaint() = 0;

	virtual void SetTitle(const char * title);

	virtual void PaintConsole(Console *);

	virtual void UpdateConsole();

	virtual int ShowMsg(const char * caption, const char *text) {}
};

} // namespace HoeGame

#endif // _HOE_GAME_MACOSX_H_

