
#include "StdAfx.h"
#include "../include/HoeGame/hoe_input.h"

namespace HoeGame {

KeyboardStdInput::KeyboardStdInput()
{
	memset(m_keys,0,sizeof(m_keys));
}

void KeyboardStdInput::_KeyDown(int key)
{
	bool genmsg = !m_keys[key];
	m_keys[key] = true;
	if (genmsg)
        OnKeyDown(key);
}

void KeyboardStdInput::_KeyUp(int key)
{
	m_keys[key] = false;
}

MouseStdInput::MouseStdInput()
{
	m_mouseX = 0;
	m_mouseY = 0;
	m_mouseleftdown = false;
	m_mousemiddledown = false;
	m_mouserightdown = false;
}

void MouseStdInput::_MouseMove(float X, float Y)
{
	m_mouseX = X;
	m_mouseY = Y;
	OnMouseMove(X,Y);
}

void MouseStdInput::_Wheel(long par)
{
	OnWheel(par);
}

void MouseStdInput::_ButtonUp(int par)
{
	switch (par)
	{
	case 0:
		m_mouseleftdown = false;
		OnLeftButtonUp();
		break;
	case 2:
		m_mousemiddledown = false;
		OnMiddleButtonUp();
		break;
	case 1:
		m_mouserightdown = false;
		OnRightButtonUp();
		break;
	}
}

void MouseStdInput::_ButtonDown(int par)
{
	switch (par)
	{
	case 0:
		m_mouseleftdown = true;
		OnLeftButtonDown();
		break;
	case 2:
		m_mousemiddledown = true;
		OnMiddleButtonDown();
		break;
	case 1:
		m_mouserightdown = true;
		OnRightButtonDown();
		break;
	}
}

} // namespace HoeGame

