
#include "StdAfx.h"
#include "utils.h"
#include "hoe_input.h"
#include "hoe_input_winapi.h"
#include "../include/hoe3d.h"

#ifdef _WIN32

#ifndef VK_OEM_1
#define VK_OEM_1          0xBA   // ';:' for US
#define VK_OEM_PLUS       0xBB   // '+' any country
#define VK_OEM_COMMA      0xBC   // ',' any country
#define VK_OEM_MINUS      0xBD   // '-' any country
#define VK_OEM_PERIOD     0xBE   // '.' any country
#define VK_OEM_2          0xBF   // '/?' for US
#define VK_OEM_3          0xC0   // '`~' for US
#define VK_OEM_4          0xDB  //  '[{' for US
#define VK_OEM_5          0xDC  //  '\|' for US
#define VK_OEM_6          0xDD  //  ']}' for US
#define VK_OEM_7          0xDE  //  ''"' for US
#define VK_OEM_8          0xDF
#define VK_OEM_102        0xE2  //  "<>" or "\|" on RT 102-key kbd.
#endif

HoeInputWin::HoeInputWin()
{
	m_baseX = 320;
	m_baseY = 240;
	m_lastp.x = -1;
	m_lastp.y = -1;

	Con_Print("Win32 Input system created.");
}

const char * HoeInputWin::GetName()
{
	return "Windows Messages Input System";
}

bool HoeInputWin::Init(THoeInitSettings * his)
{
	m_hwnd = his->win;
	RECT rect;
	GetClientRect(m_hwnd,&rect);
	m_baseX = rect.right / 2;
	m_baseY = rect.bottom / 2;

	return true;
}

int HoeInputWin::VKeyToHKey(int key)
{
#define HOE_KEY_DEFINE(hoecode,wincode, small, upper, text) case wincode: return hoecode;
	switch (key)
	{
#include "hoe_input_keymap.h"
	default:
		return 0;
	};
#undef HOE_KEY_DEFINE
}

bool HoeInputWin::InstallMouse(MouseType mt)
{
	if (mt == MT_Foreground)
		ShowCursor(FALSE);
	else
		ShowCursor(TRUE);

	if (!m_absolutMouse)
	{
		POINT p;
		p.x = m_baseX;
		p.y = m_baseY;
		ClientToScreen(m_hwnd, &p);
		SetCursorPos(p.x, p.y);
	}
	return true;
}

void HoeInputWin::UninstallMouse()
{

}

LRESULT CALLBACK HoeInputWin::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//float x,y;
	int key;

	switch (message)
	{
	case WM_KEYDOWN:
		key = VKeyToHKey(wParam);
		if (key && m_keyb)
			m_keyb->_KeyDown(key);
		break;
	case WM_KEYUP:
		key = VKeyToHKey(wParam);
		if (key && m_keyb)
			m_keyb->_KeyUp(key);
		break;
	case WM_LBUTTONUP:
		if (m_mouse)
			m_mouse->_ButtonUp(HMB_LEFT);
		break;
	case WM_RBUTTONUP:
		if (m_mouse)
			m_mouse->_ButtonUp(HMB_RIGHT);
		break;
	case WM_MBUTTONUP:
		if (m_mouse)
			m_mouse->_ButtonUp(HMB_MIDDLE);
		break;
	case WM_LBUTTONDOWN:
		if (m_mouse)
			m_mouse->_ButtonDown(HMB_LEFT);
		break;
	case WM_RBUTTONDOWN:
		if (m_mouse)
			m_mouse->_ButtonDown(HMB_RIGHT);
		break;
	case WM_MBUTTONDOWN:
		if (m_mouse)
			m_mouse->_ButtonDown(HMB_MIDDLE);
		break;
	case WM_MOUSEWHEEL:
		if (m_mouse)
			m_mouse->_Wheel((short) HIWORD(wParam));
		break;
	case WM_MOUSEMOVE:
		if (m_absolutMouse)
		{
			POINT point;
			GetCursorPos(&point);
			ScreenToClient(m_hwnd,&point);

			if (point.x != m_lastp.x || point.y != m_lastp.y)
			{
				MoveAbsolut((float)point.x, (float)point.y);
				m_lastp = point;
			}
		}
		break;
	}
	return 0;
}

void HoeInputWin::Process(float time)
{
	if (m_mouse && !m_absolutMouse)
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(m_hwnd,&point);

		if ((point.x - m_baseX) || (point.y - m_baseY))
		{
			m_mouse->_MouseMove(float(point.x - m_baseX), float(point.y - m_baseY));
			POINT p;
			p.x = m_baseX;
			p.y = m_baseY;
			ClientToScreen(m_hwnd, &p);
			SetCursorPos(p.x, p.y);
		}
	}
}

#endif


