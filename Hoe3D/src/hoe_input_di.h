
#ifndef _HOE_INPUT_DI_
#define _HOE_INPUT_DI_

#ifdef _USE_D3D9_
#define _INPUT_DI
#endif
#ifdef _USE_D3D8_
#define _INPUT_DI
#endif

#ifdef _INPUT_DI

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class HoeInputDI : public HoeInput
{
	HOE_WINDOW m_hwnd;
	HINSTANCE m_inst;
	LPDIRECTINPUT8  m_lpDI;
	LPDIRECTINPUTDEVICE8  m_lpDIKeyb;
	LPDIRECTINPUTDEVICE8  m_lpDIMous;
	MouseType m_type;
public:
	HoeInputDI();
	virtual bool Init(THoeInitSettings *);
	virtual const char * GetName();
	virtual bool UseKeyb();
	virtual bool InstallMouse(MouseType mt);
	virtual void UninstallMouse();
	virtual void Process(float time);
};

#endif // _INPUT_DI

#endif // _HOE_INPUT_DI_
