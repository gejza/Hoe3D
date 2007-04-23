
#ifndef _HOE_ENGINE_VIEW_H_
#define _HOE_ENGINE_VIEW_H_

#include <hoe3d.h>
#include <wx/dynlib.h>
//#include "engine_fs.h"

class XHoeFS;

namespace HoeEditor { 

class EngineEventHandler
{
	static EngineEventHandler * _this;
public:
	EngineEventHandler();
	virtual ~EngineEventHandler();
	static inline EngineEventHandler * Get() 
	{ 
		if (_this) return _this; 
		static EngineEventHandler empty;
		return &empty;
	}
	virtual void KeyDown(wxKeyEvent& event) {};
	virtual void KeyUp(wxKeyEvent& event) {};
	virtual void MouseLeftDown(const int x, const int y, wxMouseEvent & e) {};
	virtual void MouseLeftUp(const int x, const int y, wxMouseEvent & e) {};
	virtual void MouseRightDown(const int x, const int y, wxMouseEvent & e) {};
	virtual void MouseRightUp(const int x, const int y, wxMouseEvent & e) {};
	virtual void MouseWheel(wxMouseEvent & e) {};
	virtual void MouseMove(int relX, int relY, int absX, int absY, const wxMouseEvent & ev) {};
	virtual void MouseEnter(int absX, int absY) {};
	virtual void MouseLeave() {};
};

class EngineView : public wxPanel
{
protected:
#ifndef HOE_STATIC_ENGINE
	wxDynamicLibrary m_lib;
	char m_dllpath[512];
#endif
	bool m_loaded;
	wxMouseEvent m_lastmouseevent;
	wxTimer m_infotimer;
	static EngineView * g_shared;
	bool InitUntry(XHoeFS * hfs, int sdkver);
public:
	void Unload();
	EngineView();
	virtual ~EngineView();
#ifdef HOE_STATIC_ENGINE
	bool InitStatic(XHoeFS * hfs, int sdkver);
#else
	void SetEnginePath(const char * path);
	bool Init(XHoeFS * hfs, int sdkver);
#endif
	void Frame(float dtime);
	bool IsLoaded() { return m_loaded; }
	void OnPaint(wxPaintEvent& WXUNUSED(event));
	void OnSize( wxSizeEvent& event );
	void OnSizing( wxSizeEvent& event );
	void OnMouseMove(wxMouseEvent& event); 
	void OnMouseWheel(wxMouseEvent& event); 
	void OnMouseUp(wxMouseEvent& event); 
	void OnMouseDown(wxMouseEvent& event); 
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnChar(wxKeyEvent& event);
	void OnEnterWindow( wxMouseEvent& );
	void OnLeaveWindow( wxMouseEvent& );
	void OnSetFocus(wxFocusEvent &event);
	void OnKillFocus(wxFocusEvent &event);

	static inline EngineView * Get() { return g_shared; }
private:

    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

}; // namespace HoeEditor

#endif // _HOE_ENGINE_VIEW_H_

