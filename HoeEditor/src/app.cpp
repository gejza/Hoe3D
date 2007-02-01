
#include "StdAfx.h"
#include "../include/he/editor.h"
#include "../include/he/app.h"
#include "../include/he/log.h"
#include "../include/he/engview.h"
//#include "mainframe.h"
//#include "engine_view.h"
//#include "code_edit.h"


// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)

// ============================================================================
// implementation
// ============================================================================

// `Main program' equivalent: the program execution "starts" here
bool HoeEditor::App::OnInit()
{
	m_state = 0;

	setlocale(LC_NUMERIC, "C");

	SetVendorName(wxT("Hoe"));
	SetAppName(EditorName); // not needed, it's the default value

	wxSystemOptions::SetOption(wxT("msw.remap"), 0);
	
	wxConfigBase *pConfig = wxConfigBase::Get();

	// parsovani parametru
	// nastaveni aktualniho adresare
	for (int i=0;i < argc;i++)
	{
		if (i==0)
		{
			wxSetWorkingDirectory(HoeUtils::GetFileDir(argv[0]).c_str());
			continue;
		}
	}

	m_editor = CreateEditor();

	//  zacatek logovani
	m_log = new Log();
	m_log->SetLogFile(stdout);
	wxLog::SetActiveTarget(m_log);

	/*g_filelog = fopen("console.log","wt");
	wxLogStderr prozlog(g_filelog);

	m_frame = new MainFrame();

	//assert(HoeUtilsTest());
	
	// uncomment this to force writing back of the defaults for all values
	// if they're not present in the config - this can give the user an idea
	// of all possible settings for this program
	//pConfig->SetRecordDefaults();
	
	// fs
	wxString fsname = pConfig->Read(wxT("/fspath"),wxT("hoefs.dll"));
	if (!this->GetMainFrame()->GetEngineView()->GetFS()->Init(fsname))
		return false;


	// lang
	const wxLanguageInfo * li = m_locale.GetLanguageInfo(wxLANGUAGE_CZECH);

	m_locale.Init(wxLANGUAGE_CZECH);
	m_locale.AddCatalog(wxT("lang/cs_CZ"));

	// 

	// inicializace systemu

    // Create the main application window
	m_frame->GetLeftPanel()->UpdateSize();
	m_frame->Create(_("Hoe World Editor"));*/


    m_editor->Show(false);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
	
    m_editor->Show(true);
	//m_frame->UpdateSize();
	m_editor->Refresh();
	m_editor->Update();

	return TRUE;
}

void HoeEditor::App::Process()
{
	switch (m_state)
	{
	case 0:
        if (m_editor != NULL)
            m_editor->Refresh();
		m_state = 10;
		break;
/*	case 10:
		{
#ifdef HOE_STATIC_ENGINE
			// init
			if (m_editor->GetEngineView())
			{
				if (!m_editor->GetEngineView()->InitStatic(m_editor->GetFS()))
				{
					// chyba nahrani
				}
			}
#else
			wxString eng = 	wxConfigBase::Get()->Read(wxT("/engine"),wxT(""));
			if (eng != "")
			{
				m_editor->LoadEngine(eng);
			}
#endif
			m_state = 20;
		} break;
	case 20:
		{
			// load last project
			wxString lp = wxConfigBase::Get()->Read(wxT("/lastproject"),wxT(""));
			if (lp != "")
			{
				//wxWindowDisabler disableAll;
				//wxBusyInfo info(_T("Working, please wait..."), m_frame);

				//if (m_project.LoadProject(lp,&m_plugins))
				//    m_frame->GetTreeCtrl()->SetWorkspace(GetApp()->GetProject()->GetProjectName());

			}
			m_state = 30;
			break;
		}
	case 30:
		OnPostInit();
		m_state = 40;
		break;*/
	case 10:
		if (!m_editor->OnPostInit())
			m_state = 20;
		break;
	case 20:
		if (!OnPostInit())
			m_state = 40;
		break;
	case 40:
		// tips
		m_state = 50;
		if (wxConfigBase::Get()->Read(wxT("/showtips"),(long)1))
		{
			static size_t s_index = (size_t)-1;

			if ( s_index == (size_t)-1 )
			{
				srand(time(NULL));

				// this is completely bogus, we don't know how many lines are there
				// in the file, but who cares, it's a demo only...
				s_index = rand() % 5;
			}

			wxTipProvider *tipProvider = wxCreateFileTipProvider(_T("tips.txt"), s_index);

			bool showAtStartup = wxShowTip(m_editor, tipProvider);

			wxConfigBase::Get()->Write(wxT("/showtips"),showAtStartup ? (long)1:(long)0);

			s_index = tipProvider->GetCurrentTip();
			delete tipProvider;
		}
		break;
	case 50:
		// log dialog
		if (m_log->GetReqDialog())
		{
		    wxCommandEvent event(wxEVT_COMMAND_TOOL_CLICKED, ID_SHOWLOG);
			event.SetEventObject(this);
			GetEditor()->GetEventHandler()->ProcessEvent(event);
		}

		m_editor->ProcessFrame();
		break;
	}
}

bool HoeEditor::App::ProcessIdle()
{
	wxApp::ProcessIdle();
	Process();
	return true;
}

int HoeEditor::App::OnExit()
{
	//m_state = 100;

	wxConfigBase *pConfig = wxConfigBase::Get();
	//pConfig->Write(_T("/lastproject"), m_project.GetProjectPath());


	//delete m_log;
  // clean up: Set() returns the active config object as Get() does, but unlike
  // Get() it doesn't try to create one if there is none (definitely not what
  // we want here!)
  delete wxConfigBase::Set((wxConfigBase *) NULL);

  return 0;
}

////////////////////////////////////////
