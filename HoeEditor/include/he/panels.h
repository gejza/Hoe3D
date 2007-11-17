
#ifndef _HOE_EDITOR_PANELS_H_
#define _HOE_EDITOR_PANELS_H_

namespace HoeEditor {

class PanelMgr;

// caption
class myCaption : public wxStaticText
{
	wxSizerItem * m_cap;
	wxSizerItem * m_wnd;
	wxSizerItem * m_drag;
	PanelMgr * m_mgr;
	bool m_show;
	bool m_expand;
public:
	myCaption(PanelMgr * pm, wxString cap);
	void SetItems(wxSizerItem * cap, wxSizerItem * wnd, wxSizerItem * drag);
	void OnMouseUp ( wxMouseEvent &event );
	void OnEnterWindow ( wxMouseEvent &event );
	void OnLeaveWindow ( wxMouseEvent &event );
    virtual bool HasTransparentBackground() { return false; }
	void ShowPanel(bool show);
	void Expand(bool expand, bool update = true);

	DECLARE_EVENT_TABLE()
};

// drag
class myDrag : public wxWindow
{
	bool m_drag;
	int m_startDrag;
	wxSizerItem * m_dragWnd;
	wxSize m_size;
	PanelMgr * m_mgr;
public:
	myDrag(PanelMgr * pm);
	void SetItem(wxSizerItem * wnd);
	void OnMouseMove ( wxMouseEvent &event );
	void OnMouseUp ( wxMouseEvent &event );
	void OnMouseDown ( wxMouseEvent &event );
	void OnEnterWindow ( wxMouseEvent &event );
	void OnLeaveWindow ( wxMouseEvent &event );
	DECLARE_EVENT_TABLE()
};

class PanelMgr : public wxPanel
{
public:
	wxBoxSizer * m_sizer;
    PanelMgr();
	virtual ~PanelMgr();
	myCaption * AddPanel(wxWindow * win, wxString caption, bool fixed,bool show/* = false*/);
	void UpdateSize();
};

}; // napespace HoeEditor

#endif // _HOE_EDITOR_PANELS_H_

