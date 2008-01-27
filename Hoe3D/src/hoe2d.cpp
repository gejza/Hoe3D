
#include "StdAfx.h"
#include "hoe2d.h"

/** Init funkce */
bool HOEAPI Hoe2DEngine::Init(THoeInitSettings *)
{
	return false;
}

/** Funkce co vytvari interface tridy */
IHoeInterface * HOEAPI Hoe2DEngine::Create(const tchar *)
{
	return NULL;
}

IHoePicture * HOEAPI Hoe2DEngine::CreatePicture(int width, int height, dword * data)
{
	return 0;
}

/** Funkce pro pristup k systemum hoe */
IHoeSystem * HOEAPI Hoe2DEngine::GetSystem(HOESYSTEMS sys)
{
	return 0;
}

/** Slouzi ke spusteni prikazu */
int HOEAPI Hoe2DEngine::exec(const tchar *)
{
	return 0;
}

/** Zaregistrovani noveho prikazu */
bool HOEAPI Hoe2DEngine::RegisterCmd(const tchar * cmd, HOE_CMDFUNC func, void * par)
{
	return false;
}

/** Zaregistrovani var */
bool HOEAPI Hoe2DEngine::RegisterVar(THoeVar * var)
{
	return false;
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

	::GetInfo()->BeginFrame();
	::GetRef()->Begin();

	if (m_active)
	{
		// render normal
		m_rt.Setup();
		
		::GetStates()->Reset();
		m_active->Render(0);

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

/** Pro aktualizaci okna */
bool HOEAPI Hoe2DEngine::Resize(unsigned int width,unsigned int height)
{
	return false;
}

IHoeScene * HOEAPI Hoe2DEngine::CreateScene(HOE_TYPE_SCENE type)
{
	return 0;
}


void HOEAPI Hoe2DEngine::SetActiveScene(IHoeScene * scene)
{
}

IHoeScene * HOEAPI Hoe2DEngine::GetActiveScene()
{
	return 0;
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
