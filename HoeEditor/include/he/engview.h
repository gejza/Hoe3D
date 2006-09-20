
#ifndef _HOE_ENGINE_VIEW_H_
#define _HOE_ENGINE_VIEW_H_

#include <hoe3d.h>
#include <wx/dynlib.h>
//#include "engine_fs.h"

class XHoeFS;

namespace HoeEditor { 

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
	static EngineView * m_shared;
	bool InitUntry(XHoeFS * hfs);
public:
	void Unload();
	EngineView();
	virtual ~EngineView();
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

	static inline EngineView * Get() { return m_shared; }
private:

    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

}; // namespace HoeEditor

#endif // _HOE_ENGINE_VIEW_H_

