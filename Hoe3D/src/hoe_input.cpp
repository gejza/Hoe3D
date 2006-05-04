

#include "system.h"
#include "shared.h"
#include "utils.h"
#include "camera.h"
#include "hoe_input.h"


HoeInput::HoeInput()
{
	SET_SHARED_PTR(input);

	m_absolutMouse = false;

	m_keyb = NULL;
	m_mouse = NULL;
}

HoeInput::~HoeInput()
{
	UNSET_SHARED_PTR(input);
}

XHoeKeyboard * HoeInput::RegisterKeyboard(XHoeKeyboard * keyb)
{
	if (!this->UseKeyb())
		return false;

	XHoeKeyboard * ret = m_keyb;
	m_keyb = keyb;
	return ret;
}

XHoeMouse * HoeInput::RegisterMouse(MouseType mt, XHoeMouse * mouse)
{
	if (!this->InstallMouse(mt))
		return false;

	XHoeMouse * ret = m_mouse;
	m_mouse = mouse;
	m_absolutMouse = mt != MT_Foreground;

	return ret;
}

void HoeInput::Process(float time)
{
}

void HoeInput::Destroy()
{
	UninstallMouse();
}

bool HoeInput::UseKeyb()
{

	return true;
}

bool HoeInput::InstallMouse(MouseType mt)
{
	 return false;
}

void HoeInput::UninstallMouse()
{
}

const char * HoeInput::GetKeyName(int key)
{
#define HOE_KEY_DEFINE(hkey,vkey,small, upper, text) case hkey: return text;
	switch (key)
	{
#include "hoe_input_keymap.h"
	default:
		return "unknown";
	};
#undef HOE_KEY_DEFINE
}

#ifdef _WIN32
LRESULT CALLBACK HoeInput::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

#endif // _WIN32

#ifdef _LINUX
bool HoeInput::XProc(XEvent * event)
{
	return true;
}
#endif // _LINUX





