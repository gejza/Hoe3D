
#include "StdAfx.h"
#include "shared.h"
#include "hoe2d.h"
#include "ref.h"
#include "scene_base.h"
#include "../include/hoefs.h"
#include "icreate.h"

/** Init funkce */
bool HOEAPI Hoe2DEngine::Init(THoeInitSettings * his)
{
	if (!::GetRef()->Init(his))
		return false;

	return true;
}

IDirectDrawSurface7 * srf = NULL; 

class ResComp : public HoeRes::MediaStreamPic
{
	HoeRes::MediaStreamPic * m_stream;
	byte * m_buff;
public:
	ResComp(HoeRes::MediaStreamPic* stream)
		: m_stream(stream)
	{
		m_buff = new byte[stream->GetPitch()];
	}
	virtual ~ResComp()
	{
		delete [] m_buff;
	}
	virtual HOEFORMAT GetFormat() { return HOE_R5G6B5; }
	virtual uint GetPitch() 
	{
		THoeSizeu s;
		m_stream->GetSize(&s);
		return s.width * 2;
	}
	virtual void GetSize(THoeSizeu* size) { m_stream->GetSize(size); }
	virtual uint Close() { return 0; }
	virtual uint GetRow(byte* ptr)
	{
		uint w = m_stream->GetRow(m_buff);
		byte * f = m_buff;
		byte * t = ptr;
		for (uint x=0;x < w;x++)
		{
			const byte r = f[0] & 0xF8;
			const byte g = f[1] & 0xFC;
			const byte b = f[2] & 0xF8;
			const short c = b >> 3 | g << 3 | r << 8;
			t[1] = c >> 8;
			t[0] = 0xFF & c;
			f += 3; t += 2;
		}
		return w;
	}
};

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
	HoeRes::MediaStreamPic * pic = pl.GetData();
	THoeSizeu rect;
	pic->GetSize(&rect);
	//byte * buff = new byte[pic->GetPitch()]
	//for (uint 
	DDSURFACEDESC2 desc;
	memset(&desc,0,sizeof(desc));
	desc.dwSize = sizeof(desc);
	desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	desc.dwWidth = rect.width;
	desc.dwHeight = rect.height;

	HRESULT hRes = ::GetRef()->GetDD()->CreateSurface(&desc,&srf,0);
	checkres(hRes,"IDirectDraw7::CreateSurface");

    ZeroMemory(&desc, sizeof(desc));
    desc.dwSize = sizeof(desc);
	hRes = srf->Lock(NULL, &desc, DDLOCK_WRITEONLY|DDLOCK_WAIT, NULL);
	checkres(hRes,"IDirectDrawSurface7::Lock");
	
	Ref::GetFormat(desc.ddpfPixelFormat);
	ResComp c(pic);

	byte * ptr = (byte*)desc.lpSurface;
	for (uint y=0;y < rect.height;y++)
	{
		c.GetRow(ptr);
		ptr += desc.lPitch;
	}

    srf->Unlock(NULL);

	return NULL;
}

/** Funkce pro pristup k systemum hoe */
IHoeSystem * HOEAPI Hoe2DEngine::GetSystem(HOESYSTEMS sys)
{
	return 0;
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

	if (srf)
	{
		HRESULT hRes = ::GetRef()->GetSurface()->BltFast(0,0,srf,0,0);
		checkres(hRes, "IDirectDrawSurface7::BltFast");
	}

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
	return 0;
}

#endif // _WIN32
#ifdef _LINUX
/** Matoda pro zpravy Xwindows */
bool HOEAPI Hoe2DEngine::XProc(XEvent * event)
{
}
#endif // _LINUX
