
#include "StdAfx.h"
#include "hoe_app.h"

BEGIN_HOEGAME 

CVar HoeApp::m_width("width", 800, 0);
CVar HoeApp::m_height("height", 600, 0);
CVar HoeApp::m_fullscreen("fullscreen", false, 0);
CVar HoeApp::m_enginedll("engine", ENGINE_DLL, TVAR_SSTR);

#ifdef _WIN32
HoeApp::HoeApp(HOE_INSTANCE hInst, Console * con) : HoeWin32(hInst)
#endif

#ifdef _LINUX
HoeApp::HoeApp(HOE_INSTANCE, Console * con) : HoeLinux()
#endif
{
	this->m_con = con;
	m_lastError = NULL;

	//con->Printf("Powered on %s",HOEGAMESDK);
}

HoeApp::~HoeApp()
{
		m_engine.Destroy();

		if (m_lastError)
			ShowMsg(m_lastError);
}

#ifdef _WIN32

bool HoeApp::Init()
{
	THoeInitSettings his;

	if (!this->RegisterApp())
		return false;

	if (!CreateWin(this->GetTitle(),m_width.GetInt(),m_height.GetInt(),m_fullscreen.GetBool()))
		return false;

	m_con->SetCallback(this);

	if (!GetMsg(NULL))
		exit(0);

	if (!LoadEngine())
		return false;

	if (!GetMsg(NULL))
		exit(0);

	his.hInst = m_hInstance;
	his.win = m_hWnd;
	his.width = m_width.GetInt();
	his.height = m_height.GetInt();
	his.fullscreen = m_fullscreen.GetBool();
	his.forcewnd = false;

	if (!m_engine.GetInstance()->Init(&his))
		return false;

	m_con->SetCallback(NULL);

	if (!GetMsg(m_engine.GetInstance()))
		exit(0);

	return true;
}

#endif // _WIN32
#ifdef _LINUX

bool HoeApp::Init()
{
    Atom wmDelete;

	dpy = XOpenDisplay(0);
	if(!dpy) {
 		m_con->Printf("This program requires X Windows.");
  		return false;
	}
	screen = DefaultScreen(dpy);
	
        if (!LoadEngine())
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
	if (!m_engine.GetInstance()->Init(&his))
		return false;
	win = his.win;
    wmDelete = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(dpy, win, &wmDelete, 1);
    XSetStandardProperties(dpy, win, this->GetAppName(),
            this->GetAppName(), None, NULL, 0, NULL);
	
	return true;
}

#endif // _LINUX

bool HoeApp::LoadEngine()
{
	
	return m_engine.Load(m_enginedll.GetString(), m_con, &m_fs);
}

bool HoeApp::Frame(float time)
{
	m_engine.GetInstance()->Process(time);
	OnUpdate(time);
	return m_engine.GetInstance()->Frame();
}

void HoeApp::OnUpdate(float time)
{
}

const char * HoeApp::GetTitle()
{
	return "HoeGame!";
}

void HoeApp::Run()
{
	float t=GetEngine()->SysFloatTime();
	while (1)
	{
		float nt = GetEngine()->SysFloatTime();

		if (!GetMsg(m_engine.GetInstance()))
			break;

		this->Frame((nt-t) < 1.f ? (nt-t):1.f);
		t = nt;
	}

}

void HoeApp::OnSize(int width, int height)
{
   if (this->m_engine.IsLoaded())
	 this->m_engine.GetInstance()->Resize( width, height);
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

