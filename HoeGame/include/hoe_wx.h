
#ifndef _HOE_WX_H_
#define _HOE_WX_H_

#include "hoe_game.h"

BEGIN_HOEGAME

namespace wx {

class Dialog;
class HwndTable
{
	Dialog * m_dlg;
	HWND m_hwnd;
public:
	HwndTable();
	Dialog * Get(HWND hwnd);
	void Set(HWND hwnd, Dialog * dlg);
};


class Dialog
{
	static HwndTable s_hwnds;
	static INT_PTR CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
	HWND m_hWnd;
public:
	Dialog();
	HWND GetHWND() { return m_hWnd; }
	int Show(HINSTANCE hInst, const char *res);
	virtual INT_PTR DialogProc(UINT uMsg,WPARAM wParam,LPARAM lParam);
	INT_PTR End(int retcode);

	virtual INT_PTR OnInit() { return FALSE; }
	virtual INT_PTR OnCommand(word cmd) { return FALSE; }
	virtual INT_PTR OnEnd() { return FALSE; }
};

} // namespace wx
END_HOEGAME

#endif // _HOE_WX_H_

