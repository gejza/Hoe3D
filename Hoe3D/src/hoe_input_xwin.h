#ifndef _HOE_INPUT_XWINDOW_
#define _HOE_INPUT_XWINDOW_

#ifdef _LINUX

struct THoeInitSettings;

class HoeInputXWin : public HoeInput
{
  	HOE_DISPLAY * m_disp;
	HOE_WINDOW m_win;
	bool m_diRECTMouse;
	bool m_xwinMouse;
	int m_mouse_accel_numerator;
	int m_mouse_accel_denominator;
	int m_mouse_threshold;
public:
	HoeInputXWin();
	bool Init(THoeInitSettings *);
	virtual bool InstallMouse(MouseType mt);
	virtual void UninstallMouse();
	int XKeyEventToHKey(XKeyEvent *ev);

	virtual void Process(float time);
	virtual bool XProc(XEvent * event);

};

#endif // _LINUX

#endif // _HOE_INPUT_XWINDOW_

