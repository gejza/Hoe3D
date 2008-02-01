
#include "StdAfx.h"
#include "../include/HoeGame/hoe_mobile.h"
#include "../include/HoeGame/hoe_engine.h"

BEGIN_HOEGAME 


HoeMobile::HoeMobile(HOE_INSTANCE hInst)/* : HoeBaseApp()*/
{
	this->m_hInstance = hInst;
	m_hWnd = NULL;
	m_enginstance = NULL;
}

HoeMobile::~HoeMobile()
{
}


LRESULT CALLBACK HoeMobile::WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return GET_THIS(HoeMobile)->MsgProc(hwnd, message, wParam, lParam);
}

bool HoeMobile::RegisterApp()
{
     WNDCLASS     wndclass ;
     
     wndclass.style         = CS_HREDRAW | CS_VREDRAW; // | CS_OWNDCCS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = m_hInstance;
     wndclass.hIcon         = 0;//LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = 0;//LoadCursorW (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = this->GetAppName();
     
     return RegisterClass (&wndclass) != FALSE;
}

bool HoeMobile::CreateWin(const tchar * title, int width, int height, bool fs)
{
	if (fs)
	{
		m_hWnd = CreateWindowW (this->GetAppName(), title,
							/*WS_OVERLAPPEDWINDOW*/0,
							CW_USEDEFAULT, CW_USEDEFAULT,
							640,480,//CW_USEDEFAULT, CW_USEDEFAULT,
							NULL, NULL, m_hInstance, NULL) ;
	}
	else
	{
		m_hWnd = CreateWindowW (this->GetAppName(), title,
							/*WS_OVERLAPPEDWINDOW*/0,
							CW_USEDEFAULT, CW_USEDEFAULT,
							width,height,//CW_USEDEFAULT, CW_USEDEFAULT,
							NULL, NULL, m_hInstance, NULL) ;
	}

	if (!m_hWnd)
	{
		return false;
	}

	 ShowWindow (m_hWnd, SW_SHOWNORMAL) ;
	 //AnimateWindow(m_hWnd,200,AW_SLIDE|AW_HOR_POSITIVE);

	 UpdateWindow (m_hWnd) ;

     return true;
}

void HoeMobile::HandleError()
{
	MSG	msg ;

	UpdateConsole();

	while (GetMessageW( &msg, NULL, 0U, 0U ))
    {
		if (msg.message == WM_QUIT || (msg.message == WM_KEYUP && msg.wParam == VK_ESCAPE))
			return;

        TranslateMessage (&msg) ;
		DispatchMessageW (&msg) ;
	}
}

void HoeMobile::UpdateConsole()
{
	InvalidateRect(m_hWnd,NULL,TRUE);
}

void HoeMobile::SetTitle(const char * title)
{
	hoe_assert(m_hWnd);
	SetWindowTextW(m_hWnd, L"title");
}

int HoeMobile::GetMsg(IHoeEngine * eng)
{
	MSG	msg ;

	m_enginstance = eng;

	while (PeekMessageW( &msg, NULL, 0U, 0U, PM_NOREMOVE ))
    {
		if (!GetMessageW( &msg, NULL, 0U, 0U))
			return 0;
        TranslateMessage (&msg) ;
		DispatchMessageW (&msg) ;

		if (msg.message == WM_QUIT)
			return 0;
	}

	return 1;
}

int HoeMobile::ShowMsg(const tchar * cap, const tchar * msg)
{
	MessageBox(NULL,msg,cap, MB_OK);
	return 0;
}

void HoeMobile::Destroy()
{
	if (m_hWnd)
		DestroyWindow(m_hWnd);
	m_hWnd = NULL;
	
	UnregisterClassW( L"this->GetAppName()", m_hInstance );
}

void HoeMobile::PaintConsole(Console * con)
{
	HDC         hdc ;
     PAINTSTRUCT ps ;
	

		 /* if (Becher::GetState() != Becher::sInit)
		 {
			 
		 }*/
			  /*int numlines = (m_cyClient-5) / m_cyChar;
				if (numlines > con->GetLines().Count())
					numlines = con->GetLines().Count();

				int y = ((numlines-1) * m_cyChar) + 5;
				int n = 0;

			  hdc = BeginPaint (m_hWnd, &ps) ;

           // Nalezení hranic kreslení
			  
				while (y > 0)
				{
					const char * str = con->GetLines().GetLine(n++).GetText();
					TextOut (hdc, 5, y,	str,lstrlen (str)) ;
					y -= m_cyChar;
				}

	          EndPaint (m_hWnd, &ps) ;*/
}

LRESULT CALLBACK HoeMobile::MsgProc (HWND hwnd,UINT message, WPARAM wParam, LPARAM lParam)
{
     HDC         hdc ;
     TEXTMETRICW  tm ;
     
     switch (message)
     {
     case WM_SIZE:
		this->OnSize(LOWORD (lParam), HIWORD (lParam));
         break;
      case WM_PAINT :
		this->OnPaint();
		ValidateRect(hwnd,NULL);
          break ;
     case WM_DESTROY :
		 PostQuitMessage (0) ;
          return 0 ;
     }

	if(GetHoeEngine())
		GetHoeEngine()->WndProc(hwnd, message, wParam, lParam);

     return DefWindowProcW (hwnd, message, wParam, lParam) ;
}


END_HOEGAME

extern "C" {
// facked functions
int isatty(int fildes)
{
	return 0;
}

}


