
#include "StdAfx.h"
#include "../include/he/app.h"
#include "../include/he/editor.h"
#include "../include/he/engview.h"
#include "../include/he/panels.h"
#include "../include/he/log.h"

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

	OnInitMenu();
	this->CreateStatusBar();

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
	if (GetEngineView()) {
#ifdef _WIN32
	wxFileDialog dialog(this,_("Choose a engine..."),_T(""), _T(""), _("Engine|*.dll"), wxOPEN | wxHIDE_READONLY | wxFILE_MUST_EXIST);
#else
    wxFileDialog dialog(this,_("Choose a engine..."),_T(""), _T(""), _("Engine|*.so"), wxOPEN | wxHIDE_READONLY | wxFILE_MUST_EXIST);
#endif
	if (dialog.ShowModal() == wxID_OK)
	{
		LoadEngine(dialog.GetPath());

		// select item
		//this->m_left->GetTree()->GetCtrl()->Reselect();
	}
	}
}

void HoeEditor::BaseEditor::LoadEngine(wxString path)
{
	if (GetEngineView())
	{
		GetEngineView()->SetEnginePath(path);
		if (GetEngineView()->Init(GetFS()))
		{
			wxConfigBase *pConfig = wxConfigBase::Get();
			if ( pConfig == NULL )
				return;
			pConfig->Write(_T("/engine"), path);
		}
	}
}
#endif

void HoeEditor::BaseEditor::ProcessFrame()
{
	if (GetEngineView())
		GetEngineView()->Frame(0.1f);
}

void HoeEditor::BaseEditor::OnShowLog(wxCommandEvent& event)
{
	App::Get()->GetLog()->ShowDialog();
}



