
#include "StdAfx.h"
#include <hoe3d.h>
#include "../include/hoe_wx.h"

BEGIN_HOEGAME
namespace wx {

HwndTable Dialog::s_hwnds;

HwndTable::HwndTable()
{
	m_hwnd = 0;
	m_dlg = NULL;
}

Dialog * HwndTable::Get(HWND hwnd)
{
	return hwnd==m_hwnd ? m_dlg:NULL;
}

void HwndTable::Set(HWND hwnd, Dialog * dlg)
{
	hoe_assert(m_hwnd==NULL);
	m_hwnd=hwnd;
	m_dlg=dlg;
}

// dialog
Dialog::Dialog()
{
}

int Dialog::Show(HINSTANCE hInst, const char *res)
{
	return DialogBoxParam(hInst,res, 
		GetDesktopWindow(), DialogProc, (LPARAM)this);
}

INT_PTR Dialog::End(int retcode)
{
	OnEnd();
	EndDialog(this->GetHWND(),retcode);
	return TRUE;
}

INT_PTR Dialog::DialogProc(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return OnInit();
	case WM_COMMAND:
		return OnCommand(LOWORD(wParam));
	};
	return FALSE;
}

INT_PTR CALLBACK Dialog::DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	Dialog * dlg = NULL;
	if (uMsg==WM_INITDIALOG)
	{
		dlg=(Dialog*)lParam;
		dlg->m_hWnd = hwndDlg;
		s_hwnds.Set(hwndDlg, dlg);
	}
	else
		dlg=s_hwnds.Get(hwndDlg);
	if (dlg)
		return dlg->DialogProc(uMsg, wParam, lParam);
	return FALSE;
}

} // namespace wx
END_HOEGAME

