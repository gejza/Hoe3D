
#include "StdAfx.h"
#include <hoe3d.h>
#include "../include/hoe_wx.h"

BEGIN_HOEGAME
namespace wx {

HwndTable Dialog::s_hwnds;

HwndTable::HwndTable()
{
	memset(m_item,0,sizeof(m_item));
}

Dialog * HwndTable::Get(HWND hwnd)
{
	for (int i=0;i < 10;i++)
		if (m_item[i].hwnd == hwnd)
			return m_item[i].dlg;
	return NULL;
}

void HwndTable::Set(HWND hwnd, Dialog * dlg)
{
	for (int i=0;i < 10;i++)
		if (m_item[i].hwnd == 0) {
			m_item[i].hwnd=hwnd;
			m_item[i].dlg=dlg;
			return;
		}
	hoe_assert(0);
}

void HwndTable::Unset(HWND hwnd)
{
	for (int i=0;i < 10;i++)
		if (m_item[i].hwnd == hwnd) {
			m_item[i].hwnd=0;
			return;
		}
	hoe_assert(0);
}

// dialog
Dialog::Dialog(HINSTANCE hInst)
{
	m_hInst = hInst;
}

int Dialog::Show(const char *res)
{
	return DialogBoxParam(m_hInst,res, 
		GetDesktopWindow(), DialogProc, (LPARAM)this);
}

INT_PTR Dialog::End(int retcode)
{
	OnEnd();
	EndDialog(this->GetHWND(),retcode);
	s_hwnds.Unset(this->GetHWND());
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

