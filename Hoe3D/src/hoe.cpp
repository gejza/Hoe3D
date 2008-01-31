
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "sound.h"
#include "config.h"
#include "camera.h"
#include <hoe_math.h>
#include "hoe_time.h"
#include "hoe_info.h"
#include "hoe_input_winapi.h"
#include "hoe_input_xwin.h"
#include "hoe_input_di.h"
//#include "hoe_font.h"
#include "hoe.h"
#include "unicode.h"
#include "scene_base.h"

HoeEngine::HoeEngine(int flags)
{	
	SET_SHARED_PTR(hoe);
	m_active = NULL;
	// 
	new CmdExec();
	new CodePage();

	// config create
	new Config();

#ifdef _DEBUG
	Con_Print(T("%s ver. %.2f.%d DEBUG"),::GetConfig()->GetHoeDesc(),::GetConfig()->GetHoeVersion() / 100.f,HOESDK_VERSION);
#else
	Con_Print(T("%s ver. %.2f.%d"),::GetConfig()->GetHoeDesc(),::GetConfig()->GetHoeVersion() / 100.f,HOESDK_VERSION);
#endif

	tchar osdesc[100];
	::GetConfig()->GetOSDesc(osdesc);
	Con_Print(T("%s"),osdesc);

	Con_Print(T("%s"),::GetConfig()->GetBuild());

	this->SysFloatTime = ::SysFloatTime;

	new Ref;
	//m_sound = new SoundSystem;
	new InfoSystem();


	if (!(flags & HOEF_NOSOUND))
	{
		new SoundSystem();
	}

	if (!(flags & HOEF_NOINPUT))
	{
#if defined (_INPUT_DI)
		new HoeInputDI();
#elif defined (_WIN32)
		new HoeInputWin();
#elif defined (_LINUX)
		new HoeInputXWin();
#endif
	}

	Con_Print(T("-- HOE CREATED --"));
}
HoeEngine::~HoeEngine()
{
	UNSET_SHARED_PTR(hoe);

	if (IsSoundLoaded())
	{
		delete ::GetSound();
	}

	if (IsInputLoaded())
	{
		delete ::GetInput();
	}

	delete ::GetRef();
	delete ::GetConfig();
	delete ::GetExec();
	delete ::GetCodePage();
}
#if 0
bool HoeEngine::Init(THoeInitSettings * his)
{
	int x,y;
	unsigned int width,height;
	if (!::GetConfig()->Check(his))
		return false;

	Con_Print("Init system ---");
	if (!::GetRef()->Init(his))
		return false;
	::GetConfig()->PostCheck();
	m_rt.InitMain();
#ifdef _WIN32
	if (::GetRef()->IsFullscreen())
	{
		x = y = 0;
		width = ::GetRef()->GetWidth();
		height = ::GetRef()->GetHeight();
	}
	else
	{
		RECT Rect;
		::GetClientRect(his->win,&Rect);
		width = Rect.right - Rect.left;
		height = Rect.bottom - Rect.top;
	}
#endif
#ifdef _LINUX
	Window winDummy;
    	unsigned int borderDummy;
	unsigned int depth;
	XGetGeometry( his->dpy, his->win, &winDummy, &x, &y, &width, &height, &borderDummy, &depth);	
#endif
	HoeCamera::SetView(width,height);

	if (IsSoundLoaded())
	{
		::GetSound()->Init(his);
		
        // TODO: predelat kontrolu
	}

	if (::GetInfo())
		::GetInfo()->Init();

	if (IsInputLoaded())
	{
		if (!::GetInput()->Init(his))
			return false;
	}

	return true;
}

#endif


bool HoeEngine::RegisterCmd(const tchar * cmd, HOE_CMDFUNC func, void * par)
{
	return ::GetExec()->Register(cmd,func,par);
}

bool HoeEngine::RegisterVar(THoeVar * var)
{
	return ::GetExec()->Register(var);
}

int HoeEngine::exec(const tchar * cmd)
{
	return ::GetExec()->exec(cmd);
}

void HoeEngine::Process(const double dtime)
{
	if (IsInputLoaded())
		::GetInput()->Process(float(dtime));

	if (m_active) m_active->Process(dtime);

}
#if 0
bool Hoe3D::Resize(unsigned int width,unsigned int height)
{
	HoeCamera::SetView(width,height);
	return true;
}

void Hoe3D::Destroy()
{
	if (IsInputLoaded())
	{
		::GetInput()->Destroy();
	}

	::GetRef()->Destroy();
	if (IsSoundLoaded())
	{
		::GetSound()->Destroy();
	}
	delete this;
}

IHoeScene * Hoe3D::CreateScene(HOE_TYPE_SCENE type)
{
	IHoeScene * scene = NULL;
	switch (type)
	{
	case HOETS_2D:
		scene = new Hoe2DScene();
		break;
	case HOETS_NORMAL:
		scene = new HoeScene();
		break;
	case HOETS_GRAPH:
		scene = new HoeGraphScene();
		break;
	default:
		assert(false && "neimpletentovano");
		return NULL;
	};
	return scene;
}
#endif

void HoeEngine::SetActiveScene(IHoeScene * scene)
{
	if (scene)
        m_active = dynamic_cast<HoeBaseScene*>(scene);
	else
		m_active = NULL;
}

IHoeScene * HoeEngine::GetActiveScene()
{
	return m_active;
}


#ifdef _LINUX
bool HoeEngine::XProc(XEvent * event)
{
	if (IsInputLoaded())
		return ::GetInput()->XProc(event);

	return true;
}
#endif // _LINUX

#ifdef _WIN32
LRESULT CALLBACK HoeEngine::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (IsInputLoaded())
		return ::GetInput()->WndProc(hwnd, message, wParam, lParam);

	return 0;
}
#endif // _WIN32


