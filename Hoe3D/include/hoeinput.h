
/**
   @file    hoeinterfaces.h
   @brief   Soubor s definici vstupu
*/

#ifndef _HOE_INPUT_INTERFACE_H_
#define _HOE_INPUT_INTERFACE_H_

#include "hoeinputkeys.h"

class XHoeMouse
{
public:
	virtual void HOEAPI _MouseMove(float X, float Y) {}
	virtual void HOEAPI _Wheel(long) {};
	virtual void HOEAPI _ButtonUp(int) {};
	virtual void HOEAPI _ButtonDown(int) {};
};

class XHoeKeyboard
{
public:
	virtual void HOEAPI _KeyDown(int) {};
	virtual void HOEAPI _KeyUp(int) {};
};

class XHoeJoypad
{
};

class IHoeInput : public IHoeSystem
{
public:
	enum MouseType {
		MT_Foreground = 1,
		MT_Background,
		MT_Cursored,
		MT_ForceDW = 0x7fffffff
	};

	virtual XHoeKeyboard * HOEAPI RegisterKeyboard(XHoeKeyboard * keyb) = 0;
	virtual XHoeMouse * HOEAPI RegisterMouse(MouseType mt, XHoeMouse * mouse) = 0;
	virtual const char * HOEAPI GetKeyName(int key) = 0;
};

#endif // _HOE_INPUT_INTERFACE_H_

