
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
	p.Lex();
	HoeCore::String_s<200> t(p.GetText());
	p.Lex();
	if (t == T("picture"))
		return CreatePic(p.GetText());
	if (t == T("font"))
		return CreateFont(p.GetText());
	return NULL;
}

IHoeInterface * Hoe2DEngine::CreatePic(const tchar * name)
{
	HoeCore::ReadStream* rs = GetResMgr()->GetResource(name);
	if (!rs)
		return NULL;
	HoeRes::PictureLoader pl(rs);
	HoeRes::MediaStreamPic * ps = pl.GetData();
	//HoeRes::FormatConv cs(ps);
	//HoeRes::DoubleZoom dz(&cs);
	HoeRes::FormatConv dz(ps);

	HOECOLOR ck;
	bool setck;
    if (setck = pl.GetChunk(MAKE_FOURCC('C','K','E','Y'), ck))
    {
		byte aref = 0xa0;
		pl.GetChunk(MAKE_FOURCC('A','R','E','F'), aref);
        dz.SetColorKey(ck, aref);
    }

	THoeSizeu rect;
	dz.GetSize(&rect);
	//byte * buff = new byte[pic->GetPitch()]
	//for (uint 
	// create picture
	HoePicture * pic = new HoePicture();

	::GetRef()->CreateSurface(&pic->m_surf, rect.width, rect.height);
	RefSurface::LockRect l;
	pic->m_surf.Lock(&l);
	for (uint y=0;y < rect.height;y++)
	{
		dz.GetRow(l.ptr);
		l.ptr += l.pitch;
	}
    pic->m_surf.Unlock();

    if (setck)
    {
        pic->m_surf.SetColorKey(ck);
    }
	return pic;

}

IHoeInterface * Hoe2DEngine::CreateFont(const tchar *name)
{
	HoeCore::ReadStream* rs = GetResMgr()->GetResource(name);
	if (!rs)
		return NULL;
	HoeRes::FontLoader fl(rs);
	HoeFont* f = new HoeFont();
	byte* ptr;
	size_t s;
	if (fl.GetChunk(MAKE_FOURCC('D','E','F',' '), &ptr, &s))
	{
		// ptr -> pole
		int np = s / (sizeof(wchar_t)+sizeof(int32));

	}

	return NULL;
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
