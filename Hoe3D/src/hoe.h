
#ifndef _HOE_CLASS_
#define _HOE_CLASS_

#include "../include/hoe3d.h"
#include "render_target.h"

class Hoe3D : public IHoe3DEngine 
{
	class HoeBaseScene * m_active;
	HoeRenderTarget m_rt; ///< Hlavni RenderTarget
public:
	Hoe3D(int flags);
	~Hoe3D();
	bool HOEAPI Init(THoeInitSettings *);
	IHoeResource * HOEAPI Create(const tchar *);
	virtual IHoePicture * HOEAPI CreatePicture(int width, int height, dword * data);
	IHoeSystem * HOEAPI GetSystem(HOESYSTEMS sys);

	virtual int HOEAPI exec(const tchar *);
	virtual bool HOEAPI RegisterCmd(const tchar * cmd, HOE_CMDFUNC func, void * par);
	virtual bool HOEAPI RegisterVar(THoeVar * var);
	
	virtual void HOEAPI Process(const double);
	virtual bool HOEAPI Frame();
	virtual bool HOEAPI Resize(unsigned int width,unsigned int height);
	
	void HOEAPI Destroy();

	virtual IHoeScene * HOEAPI CreateScene(HOE_TYPE_SCENE type);
	virtual void HOEAPI SetActiveScene(IHoeScene * scene);
	virtual IHoeScene * HOEAPI GetActiveScene();
	
	#ifdef _WIN32
	/** Metoda pro vstup přes okenní zprávy. */
	virtual LRESULT CALLBACK HOEAPI WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	#endif // _WIN32
	#ifdef _LINUX
	/** Matoda pro zpravy Xwindows */
	virtual bool HOEAPI XProc(XEvent * event);	
	#endif // _LINUX
};

#endif // _HOE_CLASS_

