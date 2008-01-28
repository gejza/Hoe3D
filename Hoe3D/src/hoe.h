
#ifndef _HOE_CLASS_
#define _HOE_CLASS_

#include "../include/hoe3d.h"
class HoeBaseScene;

class HoeEngine : public IHoeEngine 
{
protected:
	HoeBaseScene * m_active;
public:
	HoeEngine(int flags);
	virtual ~HoeEngine();
	virtual int HOEAPI exec(const tchar *);
	virtual bool HOEAPI RegisterCmd(const tchar * cmd, HOE_CMDFUNC func, void * par);
	virtual bool HOEAPI RegisterVar(THoeVar * var);

	virtual void HOEAPI SetActiveScene(IHoeScene * scene);
	virtual IHoeScene * HOEAPI GetActiveScene();

	virtual void HOEAPI Process(const double dtime);
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

