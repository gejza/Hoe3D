
#include "StdAfx.h"
#include "../include/HoeEditor/panels.h"

namespace HoeEditor {

BEGIN_EVENT_TABLE(myDrag, wxWindow)
	EVT_ENTER_WINDOW( myDrag::OnEnterWindow )
	EVT_LEAVE_WINDOW( myDrag::OnLeaveWindow )
	EVT_MOTION(myDrag::OnMouseMove)
	EVT_LEFT_DOWN(myDrag::OnMouseDown)
	EVT_LEFT_UP(myDrag::OnMouseUp)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(myCaption, wxWindow)
	EVT_LEFT_UP(myCaption::OnMouseUp)
	EVT_ENTER_WINDOW( myCaption::OnEnterWindow )
	EVT_LEAVE_WINDOW( myCaption::OnLeaveWindow )
END_EVENT_TABLE()


myCaption::myCaption(PanelMgr * pm, wxString cap) : wxStaticText( pm, wxID_ANY, cap,wxDefaultPosition, wxSize(160,15), /*wxWANTS_CHARS | */wxALIGN_CENTRE | wxBORDER_SIMPLE | wxST_NO_AUTORESIZE)
{
	m_mgr = pm;
	m_show = true;
	m_expand = true;
	wxFont f = *wxNORMAL_FONT; 
	f.SetWeight(wxFONTWEIGHT_BOLD);
	SetFont(f);
}

void myCaption::SetItems(wxSizerItem * cap, wxSizerItem * wnd, wxSizerItem * drag)
{
	m_cap = cap;
	m_wnd = wnd;
	m_drag = drag;
}

void myCaption::OnMouseUp ( wxMouseEvent &event )
{
	Expand(!m_expand);
}

void myCaption::ShowPanel(bool show)
{
	if (show != m_show)
	{
		m_show = show;
		if (show)
		{
			m_cap->Show(true);
			Expand(m_expand, false);
		}
		else
		{
			m_cap->Show(false);
			m_wnd->Show(false);
			if (m_drag)
				m_drag->Show(false);
		}
		m_mgr->UpdateSize();
	}
}

void myCaption::Expand(bool expand, bool update)
{
	m_wnd->Show(expand && m_show);
	if (m_drag)
		m_drag->Show(expand && m_show);
	
	wxFont f = *wxNORMAL_FONT; 
	f.SetWeight(expand ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL);
	SetFont(f);
	if (update)
		m_mgr->UpdateSize();
	m_expand = expand;
}

void myCaption::OnEnterWindow ( wxMouseEvent &event )
{
	SetBackgroundColour(wxColor(200, 200, 200));
	Refresh();
}

void myCaption::OnLeaveWindow ( wxMouseEvent &event )
{
	SetBackgroundColour(wxColor(150, 150, 150));
	Refresh();
}


////////////////////////////////////////////////////////////////////////

myDrag::myDrag(PanelMgr * pm) : wxWindow( pm, wxID_ANY, wxDefaultPosition, wxSize(3,3))
{
	this->SetCursor(wxCursor(wxCURSOR_SIZENS));
	SetBackgroundColour(wxColor(45, 145, 1));
	m_drag = false;
	m_mgr = pm;
}

void myDrag::SetItem(wxSizerItem * wnd)
{
	m_dragWnd = wnd;
}

void myDrag::OnMouseMove ( wxMouseEvent &event )
{
	if (m_drag)
	{
		m_size.y += event.m_y - m_startDrag;
		m_dragWnd->SetMinSize(m_size);
		m_mgr->UpdateSize();		
	}
}

void myDrag::OnMouseDown ( wxMouseEvent &event )
{
	m_drag = true;
	m_startDrag = event.m_y;
	m_size = m_dragWnd->GetMinSizeWithBorder() ;
	CaptureMouse();
}

void myDrag::OnMouseUp ( wxMouseEvent &event )
{
	if (m_drag)
	{
		ReleaseMouse();
		m_drag = false;
		m_size.y += event.m_y - m_startDrag;
		m_dragWnd->SetMinSize(m_size);
		m_mgr->UpdateSize();
	}
}

void myDrag::OnEnterWindow ( wxMouseEvent &event )
{
	SetBackgroundColour(wxColor(45, 230, 1));
	Refresh();

}

void myDrag::OnLeaveWindow ( wxMouseEvent &event )
{
	SetBackgroundColour(wxColor(45, 145, 1));
	Refresh();
}


////////////////////////////////////////////////////////////////////////

PanelMgr::PanelMgr()
{
	m_sizer = new wxBoxSizer(wxVERTICAL); 
}

PanelMgr::~PanelMgr()
{
}

myCaption * PanelMgr::AddPanel(wxWindow * win, wxString caption, bool fixed, bool show)
{
	// caption
	myCaption * cap = new myCaption(this,caption);
	wxSizerItem * capi = m_sizer->Add(cap,wxSizerFlags().Align(wxALIGN_CENTER).Border(wxALL, 2));

	wxSizerItem * wnd = m_sizer->Add(win,wxSizerFlags().Expand().Border(wxALL,1));
	if (fixed)
	{
		cap->SetItems(capi,wnd,NULL);
	}
	else
	{
		myDrag * drag = new myDrag(this);
		drag->SetItem(wnd);
		wxSizerItem * dragi = m_sizer->Add(drag,wxSizerFlags().Expand());
		cap->SetItems(capi,wnd,dragi);
	}

	cap->ShowPanel(show);

	
	this->SetSizer(m_sizer);
	//this->SetSashGravity(0.6);
	//this->SplitHorizontally(m_tree.GetCtrl(), m_prop.m_property, pConfig->Read(_T("/MainFrame/leftsplit"), 300));
	m_sizer->SetSizeHints( this );
	m_sizer->RecalcSizes();

	return cap;
}
void PanelMgr::UpdateSize()
{
	m_sizer->RecalcSizes();
	this->Refresh();
}

}; // napespace HoeEditor

//

