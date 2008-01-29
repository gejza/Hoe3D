
#include "StdAfx.h"
#include "shared.h"
#include "hoe2d.h"
#include "ref.h"
#include "scene_base.h"
#include "../include/hoefs.h"

/** Init funkce */
bool HOEAPI Hoe2DEngine::Init(THoeInitSettings * his)
{
	if (!::GetRef()->Init(his))
		return false;;

	return true;
}

/** Funkce co vytvari interface tridy */
IHoeInterface * HOEAPI Hoe2DEngine::Create(const tchar * cmd)
{
	// create parser
	GetResMgr()->GetResource(cmd);

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
