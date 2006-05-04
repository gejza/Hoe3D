
#include <wx/wxprec.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>

#include <he/app.h>
#include <he/editor.h>
#include <he/engview.h>
#include <hoe3d.h>
#include <hoetypes.h>
#include <hfmt/hres_file.h>
#include <hfmt/htex_file.h>
#include <vector>
#include <assert.h>
#include "../lib/hoeUtils.h"
#include "textureview.h"
#include "deffile.xpm"
#include "exefile.xpm"

//using namespace HoeEditor;

IMPLEMENT_APP(TexViewApp)
const char * EditorName = "TextureEditor";

HoeEditor::BaseEditor * TexViewApp::CreateEditor()
{
	HoeEditor::BaseEditor * e = new TextureView();
	e->Create("Texture Editor");

	
	//m_split = new wxSplitterWindow(this,10, wxDefaultPosition, wxDefaultSize,0);
	//m_split->SetSashGravity(0.3);

	////////////// menu
	wxMenu * m_menuFile = new wxMenu;
	m_menuFile->Append(HoeEditor::ID_OPEN, _("&New...\tCtrl+N"), _("Creates a new file or project."));
	m_menuFile->Append(HoeEditor::ID_SHOWLOG, _("Show &Log...\tF2"), _("Show log dialog."));
	m_menuFile->Append(HoeEditor::ID_ENGINE, _("&Load Engine...\tF10"), _("Load engine from library"));
	
	wxMenuBar * m_menu = new wxMenuBar(/*wxMB_DOCKABLE*/);
    m_menu->Append(m_menuFile, _("&File"));

    // ... and attach this menu bar to the frame
    e->SetMenuBar(m_menu); 

	// left panel
	wxSplitterWindow * m_split = new wxSplitterWindow(e,10, wxDefaultPosition, wxDefaultSize,0);

	wxTreeCtrl * t1 = new wxTreeCtrl();
	t1->Create(m_split, -1, wxDefaultPosition, wxDefaultSize,wxBORDER_SIMPLE|wxTR_DEFAULT_STYLE);

	// engine view
	e->GetEngineView()->Create(m_split, -1);

	m_split->SetSashGravity(0.01);
	m_split->SplitVertically(t1,e->GetEngineView(),150);

	//


	return e;
}






