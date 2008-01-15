
#ifndef _INPUT_APP_H_
#define _INPUT_APP_H_

#include "../Common/tutorial.h"

class InputApp : public HoeTutorial, public XHoeKeyboard, public XHoeMouse, public XHoe2DCallback
{
	IHoeFont * f;
	char msg[256];

public:
	InputApp(HOE_INSTANCE instance, HoeGame::Console * con);
	virtual const char * GetAppName() { return "InputApp"; }
	bool LoadScene();

	// keyb func
	virtual void HOEAPI _KeyDown(int);
	virtual void HOEAPI _KeyUp(int);
	virtual void HOEAPI _Wheel(long);
	virtual void HOEAPI _MouseMove(float X, float Y);
	virtual void HOEAPI _ButtonDown(int);
	virtual void HOEAPI _Paint(IHoe2D *);
};

#endif // _INPUT_APP_H_

