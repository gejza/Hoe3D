
#include "StdAfx.h"
#include "../include/he/app.h"
#include "../include/he/editor.h"
#include "../include/he/engview.h"
#include "../include/he/panels.h"
#include "../include/he/log.h"
#include "../include/he/utils.h"

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(HoeEditor::BaseEditor, wxFrame)
	EVT_MENU(ID_QUIT, HoeEditor::BaseEditor::OnQuit)
	/*EVT_MENU(ID_NEW, MainFrame::OnNew)
	EVT_MENU(ID_OPEN, MainFrame::OnOpen)
*/
#ifndef HOE_STATIC_ENGINE
	EVT_MENU(ID_ENGINE, HoeEditor::BaseEditor::OnEngine)
#endif
	EVT_MENU(ID_SHOWLOG, HoeEditor::BaseEditor::OnShowLog)
	/*/ view
	EVT_MENU(ID_VIEWENGINE, MainFrame::OnViewEngine)
	EVT_MENU(ID_VIEWCODE, MainFrame::OnViewCode)*/
	EVT_MENU(ID_VIEWFULLSCREEN, HoeEditor::BaseEditor::OnViewFullscreen)

	// adv
	/*EVT_MENU(ID_ADV_WHITESPACE, MainFrame::OnViewWhiteSpace)

	EVT_MENU(ID_PLUGINMGR, MainFrame::OnPluginDlg)
	EVT_MENU(ID_ABOUT, MainFrame::OnAbout)

	EVT_MENU_RANGE(ID_CUSTOMMENU_FIRST,ID_CUSTOMMENU_LAST,MainFrame::OnCustomMenu)*/
END_EVENT_TABLE()


HoeEditor::BaseEditor::BaseEditor()
{
	m_menu = NULL;
}

HoeEditor::BaseEditor::~BaseEditor()
{
	App::Get()->StateToEnd();
}

bool HoeEditor::BaseEditor::Create(const wxString & title)
{
	wxFrame::Create(NULL, -1, title, wxDefaultPosition, wxSize(800,600) );

	return true;
}

void HoeEditor::BaseEditor::OnViewFullscreen(wxCommandEvent& event)
{
	ShowFullScreen(m_menu->IsChecked(ID_VIEWFULLSCREEN));
}

void HoeEditor::BaseEditor::OnQuit(wxCommandEvent& event)
{
	this->Close();
}

#ifndef HOE_STATIC_ENGINE
void HoeEditor::BaseEditor::OnEngine(wxCommandEvent& event)
{
	if (EngineView::Get()) {
#ifdef _WIN32
	wxFileDialog dialog(this,_("Choose a engine..."),_T(""), _T(""), _("Engine|*.dll"), wxOPEN | wxFILE_MUST_EXIST);
#else
    wxFileDialog dialog(this,_("Choose a engine..."),_T(""), _T(""), _("Engine|*.so"), wxOPEN | wxFILE_MUST_EXIST);
#endif
	if (dialog.ShowModal() == wxID_OK)
	{
		LoadEngine(dialog.GetPath(), HOESDK_VERSION);
		// select item
		//this->m_left->GetTree()->GetCtrl()->Reselect();
	}
	}
}

void HoeEditor::BaseEditor::LoadEngine(wxString path, int sdkver)
{
	if (EngineView::Get())
	{
		EngineView::Get()->SetEnginePath(path);
		if (EngineView::Get()->Init(GetFS(), sdkver))
		{
			wxConfigBase *pConfig = wxConfigBase::Get();
			if ( pConfig )
				pConfig->Write(_T("/engine"), path);
			UpdateControls();
		}
	}
}
#endif

void HoeEditor::BaseEditor::ProcessFrame()
{
	if (EngineView::Get())
		EngineView::Get()->Frame(0.1f);
}

void HoeEditor::BaseEditor::OnShowLog(wxCommandEvent& event)
{
	App::Get()->GetLog()->ShowDialog();
}

/////////////////////////////////////
HoeEditor::ToolEventHandler::ToolEventHandler()
{
	m_tool = NULL;
}

HoeEditor::ToolEventHandler::~ToolEventHandler()
{
	if (m_tool)
		m_tool->Exit();
}

void HoeEditor::ToolEventHandler::SetTool(EditorTool *tool)
{
	if (m_tool)
		m_tool->Exit();
	//if (tool)
		m_tool = tool;
	//else
	//	m_tool = new ToolSelect();
	//if (!m_tool)
	//	OnDefaultTool();
}

void HoeEditor::ToolEventHandler::MouseEnter(int absX, int absY)
{
	if (m_tool)
		m_tool->Enter(absX, absY);
}

void HoeEditor::ToolEventHandler::MouseLeave()
{
	if (m_tool)
		m_tool->Leave();
}

void HoeEditor::ToolEventHandler::MouseMove(int relX, int relY, int absX, int absY, const wxMouseEvent & ev)
{
	if (m_tool)
		m_tool->Move(relX, relY, absX, absY, ev);
}

void HoeEditor::ToolEventHandler::MouseLeftDown(const int x, const int y, wxMouseEvent & e)
{
	if (m_tool)
		m_tool->LeftDown(x,y,e);
}

void HoeEditor::ToolEventHandler::MouseLeftUp(const int x, const int y, wxMouseEvent & e)
{
	if (m_tool)
		m_tool->LeftUp(x,y,e);
}

void HoeEditor::ToolEventHandler::MouseRightDown(const int x, const int y, wxMouseEvent & e)
{
	if (m_tool)
		m_tool->RightDown(x,y,e);
}

void HoeEditor::ToolEventHandler::MouseRightUp(const int x, const int y, wxMouseEvent & e)
{
	if (m_tool)
		m_tool->RightUp(x,y,e);
}

void HoeEditor::ToolEventHandler::MouseWheel(wxMouseEvent & e)
{
	//if (m_map)
	//{
	//	if (e.ControlDown() && m_tool)
	//	{
	if (m_tool)
			m_tool->Wheel(e);
	//	}
	//	else if (e.ShiftDown())
	//		m_map->GetView()->Zoom(e.GetWheelRotation() / 20.f);
	//	else
	//		m_map->GetView()->Rotate(e.GetWheelRotation() / 400.f);
	//}
		
}


