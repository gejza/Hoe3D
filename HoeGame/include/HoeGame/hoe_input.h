
#ifndef _HOE_GAME_INPUT_H_
#define _HOE_GAME_INPUT_H_

#include "hoe_game.h"

BEGIN_HOEGAME

class KeyboardStdInput : public XHoeKeyboard
{
	bool m_keys[256];
public:
	KeyboardStdInput();
	virtual void HOEAPI _KeyDown(int);
	virtual void HOEAPI _KeyUp(int);

	bool IsKeyDown(int key) { return m_keys[key]; }
	virtual void OnKeyDown(int key) {};
	bool IsShiftKeyDown() { return IsKeyDown(HK_LSHIFT) || IsKeyDown(HK_RSHIFT); }
};

class MouseStdInput : public XHoeMouse
{
protected:
	float m_mouseX;
	float m_mouseY;
	bool m_mouseleftdown;
	bool m_mousemiddledown;
	bool m_mouserightdown;
public:
	MouseStdInput();
	virtual void HOEAPI _MouseMove(float X, float Y);
	virtual void OnMouseMove(float X, float Y) {}
	float GetMouseX() { return m_mouseX; }
	float GetMouseY() { return m_mouseY; }
	virtual void HOEAPI _Wheel(long);
	virtual void OnWheel(long) {}
	virtual void HOEAPI _ButtonUp(int);
	virtual void OnLeftButtonUp() {}
	virtual void OnMiddleButtonUp() {}
	virtual void OnRightButtonUp() {}
	virtual void HOEAPI _ButtonDown(int);
	virtual void OnLeftButtonDown() {}
	virtual void OnMiddleButtonDown() {}
	virtual void OnRightButtonDown() {}
	bool IsLeftButtonDown() { return m_mouseleftdown; }
	bool IsMiddleButtonDown() { return m_mousemiddledown; }
	bool IsRightButtonDown() { return m_mouserightdown; }
};

END_HOEGAME

#endif // _HOE_GAME_INPUT_H_

