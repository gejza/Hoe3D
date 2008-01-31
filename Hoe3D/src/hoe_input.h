
#ifndef _HOE_INPUT_
#define _HOE_INPUT_

#include "../include/hoeinterfaces.h"

struct THoeInitSettings;

class HoeInput : public IHoeInput
{
protected:
	XHoeKeyboard * m_keyb;
	XHoeMouse * m_mouse;
	bool m_absolutMouse;
	MouseType m_mousetype;
	float m_winwidth;
	float m_winheight;
public:
	HoeInput();
	~HoeInput();
	virtual bool Init(THoeInitSettings *) = 0;
	virtual XHoeKeyboard * HOEAPI RegisterKeyboard(XHoeKeyboard * keyb);
	virtual XHoeMouse * HOEAPI RegisterMouse(MouseType mt, XHoeMouse * mouse);
	virtual void HOEAPI SetWindowRect(float width, float height);
	virtual const tchar * HOEAPI GetKeyName(int key);

	virtual bool UseKeyb();
	virtual bool InstallMouse(MouseType mt);
	virtual void UninstallMouse();

	virtual void Process(float time);

	void MoveAbsolut(float x, float y);

	virtual void Destroy();

#ifdef _WIN32
	virtual LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif // _WIN32
#ifdef _LINUX
	virtual bool XProc(XEvent * event);
#endif // _LINUX
};

#endif // _HOE_INPUT_
