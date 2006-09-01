
#ifndef _HOE_GUI_UTILS_H_
#define _HOE_GUI_UTILS_H_

#include "hoe_game.h"

BEGIN_HOEGAME

class BaseGui
{
public:
	virtual void Set(const char * prop, const char *value) = 0;
};

class TextDevice : public XHoeKeyboard
{
	int shift;
public:
	TextDevice();
	virtual void HOEAPI _KeyDown(int);
	virtual void HOEAPI _KeyUp(int);
	virtual void AddChar(char c) {};
	virtual void Back() {};
	virtual bool Key(int k) { return true; }
};

END_HOEGAME

#endif // _HOE_GUI_UTILS_H_

