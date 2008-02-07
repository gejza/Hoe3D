
#include "StdAfx.h"
#include "shared.h"
#include "hoe2d.h"
#include "ref.h"
#include "scene_base.h"
#include "../include/hoefs.h"
#include "icreate.h"
#include "hoe_picture.h"
#include "hoe_input.h"

/** Init funkce */
bool HOEAPI Hoe2DEngine::Init(THoeInitSettings * his)
{
	if (!::GetRef()->Init(his))
		return false;

	return true;
}

RefSurface srf;

/** Funkce co vytvari interface tridy */
IHoeInterface * HOEAPI Hoe2DEngine::Create(const tchar * cmd)
{
	// create parser
	HoeCore::HoeFlexMem buff(cmd);
	ICParser p;
	p.Switch(buff);
	p.Lex(); p.Lex();
	HoeCore::ReadStream* rs = GetResMgr()->GetResource(p.GetText());
	if (!rs)
		return NULL;
	HoeRes::PictureLoader pl(rs);
	HoeRes::MediaStreamPic * ps = pl.GetData();
	HoeRes::FormatConv cs(ps);

	THoeSizeu rect;
	cs.GetSize(&rect);
	//byte * buff = new byte[pic->GetPitch()]
	//for (uint 
	// create picture
	HoePicture * pic = new HoePicture();

	::GetRef()->CreateSurface(&pic->m_surf, rect.width, rect.height);
	RefSurface::LockRect l;
	pic->m_surf.Lock(&l);
	for (uint y=0;y < rect.height;y++)
	{
		cs.GetRow(l.ptr);
		l.ptr += l.pitch;
	}
    pic->m_surf.Unlock();
#ifdef _HOE_DD_
    DDCOLORKEY              ddck;
	register byte r,g,b,a;
	r = cs.key.r & 0xF8;
	g = cs.key.g & 0xF8;
	b = cs.key.b & 0xF8;
	const short c = 1 << 8 | b >> 3 | g << 2 | r << 7;
    ddck.dwColorSpaceLowValue = 0;
	//byte * t = (byte*)(&ddck.dwColorSpaceLowValue);
	//t[1] = c >> 8;
	//t[0] = 0xFF & c;
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
	HRESULT hRes = pic->m_surf.m_srf->SetColorKey(DDCKEY_SRCBLT, &ddck);
	checkres(hRes,"SetColorKey");
#endif
	return pic;
}

/** Funkce pro pristup k systemum hoe */
IHoeSystem * HOEAPI Hoe2DEngine::GetSystem(HOESYSTEMS sys)
{
	switch (sys)
	{
	case HOE_SYS_REF:
		return ::GetRef();
	case HOE_SYS_INPUT:
		return (IsInputLoaded()) ? ::GetInput():NULL;
	default:
		Con_Print(T("error get system %d"),sys);
		return NULL;
	};
}

/** Vypocita kolize a input */
void HOEAPI Hoe2DEngine::Process(const double)
{
}

/** Vykresli snimek */
bool HOEAPI Hoe2DEngine::Frame()
{
	// scene preprocess
	//if (m_active) m_active->Render();

	/*::GetInfo()->BeginFrame();*/
	::GetRef()->Begin();

	if (m_active)
	{
		// render user 2d
		//::Get2D()->Begin();
		m_active->Paint2D();
		// render stats & logos
		//::GetInfo()->Publish();
		//::Get2D()->End(); 
	}
    else
    {
        // set barva
        //::GetRef()->SetBackgroundColor(0x80ff);
        //::GetRef()->ClearBuffers(true, true);
    }

	//::GetInfo()->PreEndFrame();
	::GetRef()->End();
	//::GetInfo()->EndFrame();
	
#ifdef _HOE_OPENGL_
	checkgl("sumary check");
#endif
	

	return true;
}

/** Pro aktualizaci okna */
bool HOEAPI Hoe2DEngine::Resize(unsigned int width,unsigned int height)
{
	return false;
}

IHoeScene * HOEAPI Hoe2DEngine::CreateScene(HOE_TYPE_SCENE type)
{

	return new Hoe2DScene();
}

/** Zrusi engine */
void HOEAPI Hoe2DEngine::Destroy()
{
	delete this;
}

#ifdef _WIN32
/** Metoda pro vstup pøes okenní zprávy. */
LRESULT CALLBACK HOEAPI Hoe2DEngine::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (IsInputLoaded())
		return ::GetInput()->WndProc(hwnd, message, wParam, lParam);
	return 0;
}

#endif // _WIN32
#ifdef _LINUX
/** Matoda pro zpravy Xwindows */
bool HOEAPI Hoe2DEngine::XProc(XEvent * event)
{
}
#endif // _LINUX
