
#include "StdAfx.h"
#include "../include/hoe_app.h"

BEGIN_HOEGAME 

CVar HoeApp::m_width(T("width"), 240, 0);
CVar HoeApp::m_height(T("height"), 320, 0);
CVar HoeApp::m_fullscreen(T("fullscreen"), false, 0);
CVar HoeApp::m_enginedll(T("engine"), ENGINE_DLL, TVAR_SSTR);

#ifdef _WIN32_WINNT
HoeApp::HoeApp(HOE_INSTANCE hInst, HoeEngine& engine, Console * con) : HoeWin32(hInst)
#endif

#ifdef _WIN32_WCE
HoeApp::HoeApp(HOE_INSTANCE hInst, HoeEngine& engine, Console * con) : HoeMobile(hInst)
#endif

#ifdef _LINUX
HoeApp::HoeApp(HOE_INSTANCE, HoeEngine& engine, Console * con) : HoeLinux()
#endif

#ifdef _MACOSX
HoeApp::HoeApp(HOE_INSTANCE, HoeEngine& engine, Console * con) : HoeMacOsX()
#endif
 , m_engine(engine)
{
	this->m_con = con;
	m_lastError = NULL;

	//con->Printf("Powered on %s",HOEGAMESDK);
}

HoeApp::~HoeApp()
{
		m_engine.Destroy();

		if (m_lastError)
			ShowMsg(T("Error:"), m_lastError);
}

#ifdef _WIN32_WINNT

bool HoeApp::Init(const tchar * title, int sdkver)
{
	THoeInitSettings his;

	if (!this->RegisterApp())
		return false;

	if (!CreateWin(title,m_width.GetInt(),m_height.GetInt(),m_fullscreen.GetBool()))
		return false;

	if (m_con)
		m_con->SetCallback(this);

	if (!GetMsg(NULL))
		exit(0);

	if (!LoadEngine(sdkver))
		return false;

	if (!GetMsg(NULL))
		exit(0);

	his.hInst = m_hInstance;
	his.fullscreen = m_fullscreen.GetBool();
	his.win = m_hWnd;
	if (his.fullscreen)
	{
		his.width =  m_width.GetInt();
		his.height = m_height.GetInt();
	} else 
	{
		RECT Rect;
		GetClientRect(m_hWnd, &Rect);
		his.width = Rect.right - Rect.left;
		his.height = Rect.bottom - Rect.top;
	}
	his.forcewnd = false;

	if (!HoeGame::GetHoeEngine()->Init(&his))
		return false;

	m_con->SetCallback(NULL);

	if (!GetMsg(HoeGame::GetHoeEngine()))
		exit(0);

	return true;
}

#endif // _WIN32
#ifdef _WIN32_WCE

bool HoeApp::Init(const tchar * title, int sdkver)
{
	THoeInitSettings his;

	if (!this->RegisterApp())
		return false;

    int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (!CreateWin(title,iScreenWidth,iScreenHeight,true))
		return false;

	if (!LoadEngine(sdkver))
		return false;

	his.hInst = m_hInstance;
	his.fullscreen = m_fullscreen.GetBool();
	his.forcewnd = false;
	his.win = m_hWnd;
	his.width =  iScreenWidth;
	his.height = iScreenHeight;

	if (!HoeGame::GetHoeEngine()->Init(&his))
		return false;

	if (!GetMsg(HoeGame::GetHoeEngine()))
		exit(0);

	return true;
}

#endif // _WIN32
#ifdef _LINUX

bool HoeApp::Init(const tchar * title, int sdkver)
{
    Atom wmDelete;

	dpy = XOpenDisplay(0);
	if(!dpy) {
 		m_con->Printf("This program requires X Windows.");
  		return false;
	}
	screen = DefaultScreen(dpy);
	
        if (!LoadEngine(sdkver))
                return false;

	THoeInitSettings his;
	his.dpy = dpy;
	his.screen = screen;
	his.win = 0;
	his.depth = 16;
	his.width = m_width.GetInt();
	his.height = m_height.GetInt();
	his.forcewnd = false;
	his.fullscreen = m_fullscreen.GetBool();
	
	if (!HoeGame::GetHoeEngine()->Init(&his))
		return false;
	win = his.win;
    wmDelete = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(dpy, win, &wmDelete, 1);
    XSetStandardProperties(dpy, win, this->GetAppName(),
            this->GetAppName(), None, NULL, 0, NULL);
	
	return true;
}

#endif // _LINUX
#ifdef _MACOSX

bool HoeApp::Init(const char * title, int sdkver)
{
    OSStatus err;
	err = CreateNibReference(CFSTR("main"), &m_nibRef); 


    if (!LoadEngine(sdkver))
        return false;

	THoeInitSettings his;
	his.dpy = 0;
	his.screen = 0;
	his.win = 0;
	his.depth = 16;
	his.width = m_width.GetInt();
	his.height = m_height.GetInt();
	his.forcewnd = true;
	his.fullscreen = m_fullscreen.GetBool();

	TRACE;
	if (!HoeGame::GetHoeEngine()->Init(&his))
		return false;
	win = his.win;
	return true;
}

#endif // _MACOSX


bool HoeApp::LoadEngine(int sdkver)
{
	return m_engine.Load(m_con, NULL, sdkver);
}

void HoeApp::OnUpdate(float time)
{
}

const tchar * HoeApp::GetTitle()
{
	return T("HoeGame!");
}

void HoeApp::Run()
{
	if (!m_engine.IsLoaded())
	{
		while (GetMsg(GetHoeEngine()));
		return;
	}
	float t=GetEngine()->SysFloatTime();
	while (1)
	{
		if (!Frame())
			break;
	}
}

bool HoeApp::Frame()
{
	static float t=GetEngine()->SysFloatTime();
	float nt = GetEngine()->SysFloatTime();
	static int num = 1;
	if (!GetMsg(GetHoeEngine()))
		return false;


	//Frame((nt-t) < 1.f ? (nt-t):1.f);
	Frame((nt-t)/num); // todo
	//t = nt;
	num++;
	return true;
}

bool HoeApp::Frame(float time)
{
	assert(HoeGame::GetHoeEngine());
	HoeGame::GetHoeEngine()->Process(time);
	OnUpdate(time);
	return HoeGame::GetHoeEngine()->Frame();
}

void HoeApp::OnSize(int width, int height)
{
   if (this->m_engine.IsLoaded())
	 GetHoeEngine()->Resize( width, height);
}

void HoeApp::OnPaint()
{
	 PaintConsole(m_con);
}

void HoeApp::DestroyEngine()
{ 
	m_enginstance = NULL;
	m_engine.Destroy(); 
}

END_HOEGAME 

