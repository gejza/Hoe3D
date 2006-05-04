#ifndef _HOE_INPUT_WIN_API_
#define _HOE_INPUT_WIN_API_

#include "hoe_input.h"

#ifdef _WIN32

class HoeInputWin : public HoeInput
{
	POINT m_lastp;
	int m_baseX;
	int m_baseY;
	HWND m_hwnd;
public:
	HoeInputWin();
	virtual bool Init(THoeInitSettings *);
	virtual const char * GetName();
	virtual void Process(float time);
	virtual bool InstallMouse(MouseType mt);
	virtual void UninstallMouse();

	static int VKeyToHKey(int key);

	virtual LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif // _WIN32

#endif // _HOE_INPUT_WIN_API_
