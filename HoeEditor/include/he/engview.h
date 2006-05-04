
#ifndef _HOE_ENGINE_VIEW_H_
#define _HOE_ENGINE_VIEW_H_

#include <hoe3d.h>
#include <wx/dynlib.h>
//#include "engine_fs.h"

class XHoeFS;

namespace HoeEditor { 

class EngineView : public wxPanel
{
#ifndef HOE_STATIC_ENGINE
	wxDynamicLibrary m_lib;
	char m_dllpath[512];
#endif
	IHoe3DEngine * m_engine;
	bool m_loaded;
	wxMouseEvent m_lastmouseevent;
	wxTimer m_infotimer;
public:
	void Unload();
	EngineView();
	IHoe3DEngine * GetEngine() { return m_engine; } 
#ifdef HOE_STATIC_ENGINE
	bool InitStatic(XHoeFS * hfs);
#else
	void SetEnginePath(const char * path);
	bool Init(XHoeFS * hfs);
#endif
	void Frame(float dtime);
	bool IsLoaded() { return m_loaded; }
	void OnPaint(wxPaintEvent& WXUNUSED(event));
	void OnSize( wxSizeEvent& event );
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
private:

    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

}; // namespace HoeEditor

#endif // _HOE_ENGINE_VIEW_H_

