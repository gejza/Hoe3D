
#ifndef _CONSOLE_APP_H_
#define _CONSOLE_APP_H_

#include "../Common/tutorial.h"
#include <hoe_figure2d.h>

class Scene : public HoeGame::Hoe2DFigure , public XHoeKeyboard
{
	HoeGame::GuiConsole * con;
	IHoeFont * font;
public: 
	Scene(HoeGame::GuiConsole * c);
	void SetCon(HoeGame::GuiConsole * c) { con = c;}
	virtual void OnPaint();
	bool Load(IHoe3DEngine *);
	virtual void HOEAPI _KeyDown(int);
};

class ConApp : public HoeTutorial
{
	HoeGame::GuiConsole gcon;
	Scene scene;
public:
	ConApp(HOE_INSTANCE instance, HoeGame::Console * con);
	virtual const char * GetAppName() { return "ConApp"; }
	bool LoadScene();

	virtual void OnUpdate(float dtime);
};

#endif // _CONSOLE_APP_H_

