
#include "StdAfx.h"
#include "hoe3d.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "sound.h"
#include "config.h"
#include "texture_system.h"
#include "material_system.h"
#include "light_system.h"
#include "model_loader.h"
#include "camera.h"
#include "hoe_model.h"
#include "hoe_stream.h"
#include "camera.h"
#include "states.h"
#include "hoe_time.h"
#include "2d.h"
#include "hoe_map.h"
#include "hoe_map_part.h"
#include "hoe_info.h"
#include "hoe_input_winapi.h"
#include "hoe_input_xwin.h"
#include "hoe_input_di.h"
#include "cursor.h"
#include "hoe_font.h"
#include "physics.h"
#include "hoe.h"
#include "scene.h"
#include "hoe_picture.h"
#include "unicode.h"

// libgw32c.a libz.a  procinfo.lib jpeg_d.lib  flexlib.lib 
//#pragma comment (lib,"libjpeg.lib")
//#pragma comment (lib,"libfl.a")
//#pragma comment (lib,"freetype2110MT_D.lib")

Hoe3DEngine::Hoe3DEngine(int flags)
	: HoeEngine(flags), m_rt(HoeRenderTarget::eMain)
{
	Con_Print("-- HOE CREATED --");
}

Hoe3DEngine::~Hoe3DEngine()
{
	/*UNSET_SHARED_PTR(hoe3d);

	if (IsSoundLoaded())
	{
		delete ::GetSound();
	}

	if (IsInputLoaded())
	{
		delete ::GetInput();
	}

	delete ::GetPhysics();
	delete ::GetStates();
    
	delete ::GetRef();
	delete ::GetConfig();
	delete ::GetExec();
	delete ::GetCodePage();*/
}

/** Init funkce */
bool HOEAPI Hoe3DEngine::Init(THoeInitSettings * his)
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

#if 0

//#include "video.h"

//HoeVideoPlayer vp;

Hoe3D::Hoe3D(int flags) : m_rt(HoeRenderTarget::eMain)
{	
	SET_SHARED_PTR(hoe3d);
	m_active = NULL;
	// 
	new CmdExec();
	new CodePage();

	// config create
	new Config();

#ifdef _DEBUG
	Con_Print("%s ver. %.2f.%d DEBUG",::GetConfig()->GetHoeDesc(),::GetConfig()->GetHoeVersion() / 100.f,HOESDK_VERSION);
#else
	Con_Print("%s ver. %.2f.%d",::GetConfig()->GetHoeDesc(),::GetConfig()->GetHoeVersion() / 100.f,HOESDK_VERSION);
#endif

	char osdesc[100];
	::GetConfig()->GetOSDesc(osdesc);
	Con_Print("%s",osdesc);

	Con_Print("%s",::GetConfig()->GetBuild());

	this->SysFloatTime = ::SysFloatTime;

	new Ref;
	//m_sound = new SoundSystem;
	new InfoSystem();
	new ResourceMgr();
	new TextureSystem();
	new MaterialSystem();
	new LightSystem();
	new HoeStates();
	new Hoe2D();

	new Physics();

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

	Con_Print("-- HOE CREATED --");
}

#endif


/*HoeRenderTarget * GetRT()
{
	static HoeRenderTarget rt(HoeRenderTarget::eToTexture);
	return &rt;
}

class RenderQueBase
{
public:
	virtual void DrawScene(HoeBaseScene * scene) = 0;
};*/

bool Hoe3DEngine::Frame()
{
	// scene preprocess
	//if (m_active) m_active->Render();

	::GetInfo()->BeginFrame();
	::GetRef()->Begin();

	if (m_active)
	{
		// render to texture
		// vyrenderovani vseho do textur
		// nastaveni render parameters
		// prerender effect
		// postrender effect

		// render special scenes
		/*
		for pres vsechny preeffekty
		 preeffekt->drawscene(m_active);
		 // renderovat 1x za 5 snimku treba
		*/
			
		/*HoeRenderTarget * rt = ::GetRT();
		rt->Setup();
		::GetStates()->Reset();

		m_active->Render(0);

		rt->EndRender();*/
		
		
		// render normal
		m_rt.Setup();
		// render vysledku
		/*::Get2D()->Begin();
		HoePicture pic;
		pic.SetSource(rt->GetTexture());
		const float w=2,h=2;
		::Get2D()->SetRect(w,h);
		for (float i=0;i<w;i++)
			for (float j=0;j < h;j++)
				::Get2D()->BltFast(i,i+1,j,j+1,&pic);
		::Get2D()->End();*/
		
		::GetStates()->Reset();
		m_active->Render(0);

		// render video
		/*vp.NextFrame();
		::Get2D()->Begin();
		HoePicture pic;
		pic.SetSource(vp.GetTexture());
		const float w=4,h=3;
		//::Get2D()->SetRect(w,h);
		//for (float i=0;i<w;i++)
		//	for (float j=0;j < h;j++)
		//		::Get2D()->BltFast(i,i+1,j,j+1,&pic);
		::Get2D()->SetRect(40,30);
		//for (float i=0;i<w;i++)
		//	for (float j=0;j < h;j++)
				::Get2D()->BltFast(10,20,5,15,&pic);
		::Get2D()->End();*/

		// render user 2d
		::Get2D()->Begin();
		m_active->Paint2D();
		// render stats & logos
		::GetInfo()->Publish();
		::Get2D()->End(); 

		//HoeCursor::Draw();
		m_rt.EndRender();
	}
    else
    {
        // set barva
        ::GetRef()->SetBackgroundColor(0x80ff);
        ::GetRef()->ClearBuffers(true, true);
    }

	::GetInfo()->PreEndFrame();
	::GetRef()->End();
	::GetInfo()->EndFrame();
	
#ifdef _HOE_OPENGL_
	checkgl("sumary check");
#endif

	return true;
}

bool Hoe3DEngine::Resize(unsigned int width,unsigned int height)
{
	HoeCamera::SetView(width,height);
	return true;
}

void Hoe3DEngine::Destroy()
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

IHoeScene * Hoe3DEngine::CreateScene(HOE_TYPE_SCENE type)
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




