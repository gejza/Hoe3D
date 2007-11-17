
#ifndef _HOE_WX_H_
#define _HOE_WX_H_

#include "hoe_game.h"

BEGIN_HOEGAME

namespace wx {

class Dialog;
class HwndTable
{
	struct Item
	{
		HWND hwnd;
		Dialog * dlg;
	} m_item[10];//!!!todo
public:
	HwndTable();
	Dialog * Get(HWND hwnd);
	void Set(HWND hwnd, Dialog * dlg);
	void Unset(HWND hwnd);
};


class Dialog
{
	static HwndTable s_hwnds;
	static INT_PTR CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
	HWND m_hWnd;
	HINSTANCE m_hInst;
public:
	Dialog(HINSTANCE hInst);
	HWND GetHWND() { return m_hWnd; }
	HINSTANCE GetInstance() { return m_hInst; }
	int Show(const tchar *res);
	virtual INT_PTR DialogProc(UINT uMsg,WPARAM wParam,LPARAM lParam);
	INT_PTR End(int retcode);

	virtual INT_PTR OnInit() { return FALSE; }
	virtual INT_PTR OnCommand(word cmd) { return FALSE; }
	virtual INT_PTR OnEnd() { return FALSE; }
};

} // namespace wx
END_HOEGAME

#endif // _HOE_WX_H_

