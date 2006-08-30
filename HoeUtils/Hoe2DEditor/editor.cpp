
/**
   @file    editor.h
   @date    Aug 2006
   @version 1.0
   @brief   Soubor s definici hlavnich trid 2d editoru.
*/

#include "StdAfx.h"
#include "editor.h"
#include "figure.h"
#include "items.h"
#include "tools.h"

IMPLEMENT_APP(Hoe2DEditApp)

const char * EditorName = "Hoe2DEditor";

enum {
	ID_TREE = HoeEditor::ID_CUSTOMMENU_FIRST,
	ID_COLORRECT,
	ID_STATICITEM,

};

BEGIN_EVENT_TABLE(Hoe2DEdit, HoeEditor::BaseEditor)
	EVT_MENU(HoeEditor::ID_NEW, Hoe2DEdit::OnNewFile)
	EVT_TREE_SEL_CHANGED(ID_TREE, Hoe2DEdit::OnTreeSelect)
	//EVT_TREE_BEGIN_DRAG(ID_TREE, Hoe2DEdit::OnTreeDrag)
	EVT_MENU(ID_COLORRECT, Hoe2DEdit::OnNewItem)
	EVT_MENU(ID_STATICITEM, Hoe2DEdit::OnNewItem)

	//EVT_MENU_RANGE(ID_OBJECT, ID_OBJECT + EBO_Max, BecherEdit::OnNewObject)

END_EVENT_TABLE()


HoeEditor::BaseEditor * Hoe2DEditApp::CreateEditor()
{
    //wxImage::AddHandler(new wxPNGHandler);



	// load lang
	m_locale.Init(wxLANGUAGE_CZECH);
	wxLocale::AddCatalogLookupPathPrefix("resource");
	wxLocale::AddCatalogLookupPathPrefix("../Hoe/HoeEditor/lang");

	m_locale.AddCatalog("editor_cs");
	m_locale.AddCatalog("cs_CZ");

	Hoe2DEdit * e = new Hoe2DEdit();
	e->Create("Hoe2D Editor");
 //   wxBitmap bitmap;
 //   if (bitmap.LoadFile(_T("splash.bmp"), wxBITMAP_TYPE_BMP))
	//m_splash = new wxSplashScreen(bitmap,
 //           wxSPLASH_CENTRE_ON_PARENT/*|wxSPLASH_TIMEOUT*/,
 //           4000, e, wxID_ANY, wxDefaultPosition, wxDefaultSize,
 //           wxSIMPLE_BORDER|wxSTAY_ON_TOP|wxFRAME_NO_TASKBAR);
	//else 
		m_splash = NULL;
	wxYield(); 
	return e;
}

bool Hoe2DEditApp::OnPostInit()
{
	if (m_splash) delete m_splash;
	return false;
}

///////////////////////////
 Hoe2DEdit * Hoe2DEdit::s_actinstance = NULL;

Hoe2DEdit::Hoe2DEdit()
{
	assert(s_actinstance == NULL);
	s_actinstance = this;
	m_figure = NULL;
	m_select = NULL;
}

Hoe2DEdit::~Hoe2DEdit()
{
	assert(s_actinstance == this);
	s_actinstance = NULL;
}
#include "resource/new.xpm"
bool Hoe2DEdit::Create(const wxString & title)
{

	
/*	wxImage::AddHandler(new wxGIFHandler);
	// 
	wxHelpControllerHelpProvider* provider = new wxHelpControllerHelpProvider;
    wxHelpProvider::Set(provider);
	provider->SetHelpController(&m_help);
	if (!m_help.Initialize("BecherEditor"))
	{
		wxMessageBox(wxT("Error: Cannot initialize the help system."));
		return false;
	}
*/
	HoeEditor::BaseEditor::Create(title);

	// tool bar
	wxToolBar * tool = CreateToolBar();
	wxBitmap newt(new_xpm);
int w = newt.GetWidth(),
        h = newt.GetHeight(); 
	tool->SetToolBitmapSize(wxSize(w, h));

	tool->AddTool(HoeEditor::ID_NEW,"nic",newt,"hokus pokus");
	tool->AddSeparator();
	tool->AddTool(ID_COLORRECT, "Color Rect", newt, "Color Rect");
	tool->AddTool(ID_STATICITEM, "picture", newt, "Picture");
	tool->Realize();

	// vytvorit menu
    // file
    wxMenu * menuFile = new wxMenu;
	menuFile->Append(HoeEditor::ID_NEW, _("&New...\tCtrl+N"), _("Creates a new file or project."));
	menuFile->Append(HoeEditor::ID_OPEN, _("&Open...\tCtrl+O"), _("Open an existing file."));
	menuFile->AppendSeparator();
	menuFile->Append(HoeEditor::ID_SAVE, _("&Save\tCtrl+S"), _("Save file."));
	menuFile->Append(HoeEditor::ID_SAVEAS, _("Save &As..."), _("Save file."));
	menuFile->AppendSeparator();
	menuFile->Append(HoeEditor::ID_QUIT, _("E&xit\tAlt-F4"), _("Quit this program")); 

    wxMenu * menuView = new wxMenu;
	menuView->AppendCheckItem(HoeEditor::ID_VIEWFULLSCREEN, _("F&ull Screen\tF12"), _("Switch to fullscreen."));

    // now append the freshly created menu to the menu bar...
    m_menu = new wxMenuBar(/*wxMB_DOCKABLE*/);
    m_menu->Append(menuFile, _("&File"));
    m_menu->Append(menuView, _("&View"));
	
    // ... and attach this menu bar to the frame
    SetMenuBar(m_menu); 

	wxStatusBar *statbar = CreateStatusBar();
	statbar->SetFieldsCount(2);

	wxSplitterWindow * split = new wxSplitterWindow(this,10, wxDefaultPosition, wxDefaultSize,0);
	split->SetSashGravity(0);
	
	// seradit do layoutu

	m_leftpanel = new HoeEditor::PanelMgr();
	m_leftpanel->Create(split, -1);
	/*GetPanelMgr()->AddPanel(
		new ToolObjects(GetPanelMgr()), _("Tools"), true, true);*/
	/*GetPanelMgr()->AddPanel(
		new TerrainObject(GetPanelMgr()), _("Terrain"), true, true);*/
	// list view
	m_tc = new wxTreeCtrl(GetPanelMgr(), ID_TREE);

	GetPanelMgr()->AddPanel(
		m_tc, _("List"), false, true);
	m_prop = new HoeEditor::PropertyGrid();
	m_prop->Create(GetPanelMgr());
	GetPanelMgr()->AddPanel(
		m_prop, _("Properties"), false, true);

	m_engview = new HoeEditor::EngineView();
	m_engview->Create(split, -1, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);

	split->SplitVertically(m_leftpanel, m_engview, 150);

	m_fs.AddResourceFile("hud.hx");

	return true;
}

bool Hoe2DEdit::OnPostInit()
{
	LoadEngine("../../bin/Hoe3Dd_D3D9.dll");
	return false;
}

// updejt menu
void Hoe2DEdit::UpdateControls()
{
	/*if (m_menu)
	{
		// engine
		m_menu->Enable(HoeEditor::ID_NEW, this->GetEngineView()->IsLoaded());
		m_menu->Enable(HoeEditor::ID_OPEN, this->GetEngineView()->IsLoaded());
		// map
		m_menu->Enable(HoeEditor::ID_SAVE, m_map != NULL);
		m_menu->Enable(HoeEditor::ID_SAVEAS, m_map != NULL);
		m_menu->Enable(ID_TERRAINWIRE, m_map != NULL);
		m_menu->Enable(ID_OBJECTS, m_map != NULL);
		m_menu->Enable(ID_SYSOBJECTS, m_map != NULL);
		m_menu->Enable(ID_MAPSETTINGS, m_map != NULL);
	}*/
}

void Hoe2DEdit::OnNewFile(wxCommandEvent &)
{
	CloseFile();

	m_figure = new FigureEdit(m_tc);
	m_figure->Create(m_engview->GetEngine());

	UpdateControls();
}

void Hoe2DEdit::CloseFile()
{
	// unselect
	// dotaz na ulozeni

	if (m_figure)
		m_figure->Delete();
	m_figure = NULL;
}

/*
void BecherEdit::OnOpenFile(wxCommandEvent &)
{
	wxFileDialog dlg(this,_("Choose a map file..."),
		_T(""), _T(""), _("Becher Map(*.bm)|*.bm|All Files(*.*)|*.*"), 
		wxOPEN | wxHIDE_READONLY | wxFILE_MUST_EXIST); 
	if (dlg.ShowModal() == wxID_OK)
	{
		if (m_map == NULL)
			m_map = new EditorMap();
		m_map->SetFilePath(dlg.GetPath());
		if (!m_map->LoadMap(dlg.GetPath()))
		{
			wxLogMessage(_("Open map file %s failed."), dlg.GetPath().c_str());
			CloseMap();
			MenuUpdate();
		}
		else
		{
			//m_mapfilepath = dlg.GetPath();
			SetTitle(m_map->GetTitle());
			GetEngine()->SetActiveScene(m_map->GetScene());
			HoeGetRef(GetEngine())->SetBackgroundColor(0xffb060ff);
			MenuUpdate();
		}
	}
	
	UpdateControls();
}

void BecherEdit::OnSaveFile(wxCommandEvent &e)
{
	if (!m_map)
		return;

	if (m_map->GetFilePath().IsEmpty() || e.GetId() == HoeEditor::ID_SAVEAS)
	{
		wxFileDialog dlg( this, _("Save map file..."),
			_T(""), _T(""), _("Becher Map(*.bm)|*.bm"), wxSAVE | wxHIDE_READONLY);
nq:
		if (dlg.ShowModal() == wxID_OK)
		{
			// todo: prepsat?
			if (wxFile::Exists(dlg.GetPath()))
			{
				wxString str;
				str = wxString::Format(_("File %s exist, overwrite?"),dlg.GetPath().c_str());
				int q = wxMessageBox(str, _("Question"),
                              wxICON_QUESTION | wxYES_NO | wxCANCEL);
				if (q == wxNO)
					goto nq;
				if (q == wxCANCEL)
					return;
			}

			m_map->SetFilePath(dlg.GetPath());
		}
		else
			return;
	}

	if (m_map->SaveMap())
		SetTitle(m_map->GetTitle());
}

void BecherEdit::OnHelp(wxCommandEvent &)
{
	// help
	//m_msHtmlHelp.KeywordSearch("neco");
	m_help.DisplaySection(1);
}

void BecherEdit::OnAbout(wxCommandEvent &)
{
	AboutDlg dlg(this);
	dlg.ShowModal();
}*/

void Hoe2DEdit::OnNewItem(wxCommandEvent &e)
{
	if (m_figure)
	{
		switch (e.GetId())
		{
		case ID_STATICITEM:
			m_figure->AddItem(new StaticItem(), "static");
			break;
		case ID_COLORRECT:
			m_figure->AddItem(new ColorRectItem(), "rect");
			break;
		};
	}
}

void Hoe2DEdit::OnTreeSelect(wxTreeEvent &e)
{
	assert(m_figure);
	wxTreeItemData * d = m_tc->GetItemData(e.GetItem());
	if (!d)
	{
		m_prop->Begin(NULL);
		m_select = NULL;
		SetTool(NULL);
		return;
	}
	else
	{
		m_select = dynamic_cast<BaseItem*>(d);
		m_select->Select(m_prop);
		SetTool(new ToolItem(m_select));
	}
}

/*void BecherEdit::OnNewObject(wxCommandEvent &)
{
}

/////////////////////////////////////
void BecherEdit::UpdateControls()
{
	
}

void BecherEdit::CloseMap()
{
	SAFE_DELETE(m_map);
	//m_mapfilepath = wxT("");
}*/

/////////////////////////////////////
void Hoe2DEdit::KeyDown(wxKeyEvent& e)
{
	/*if (m_map)
	{
		if (e.ControlDown())
		{
			switch (e.GetKeyCode())
			{
			case WXK_LEFT:
				m_map->GetView()->Rotate(0.1f);
				break;
			case WXK_RIGHT:
				m_map->GetView()->Rotate(-0.1f);
				break;
			case WXK_UP:
				m_map->GetView()->Zoom(-3.7f);
				break;
			case WXK_DOWN:
				m_map->GetView()->Zoom(3.7f);
				break;

			};
		}
		else
		{
			switch (e.GetKeyCode())
			{
			case WXK_LEFT:
				m_map->GetView()->Move(0,-move_speed);
				break;
			case WXK_RIGHT:
				m_map->GetView()->Move(0,move_speed);
				break;
			case WXK_UP:
				m_map->GetView()->Move(move_speed,0);
				break;
			case WXK_DOWN:
				m_map->GetView()->Move(-move_speed,0);
				break;

			};
		}			
	}*/
}

void Hoe2DEdit::KeyUp(wxKeyEvent& e)
{

}

void Hoe2DEdit::SetStatus(const wxString &str)
{
	SetStatusText(str, 0);
}

//////////////////////////////////
/*EditBaseConsole * GetCon()
{
	return HoeEditor::App::Get()->GetConsole();
}

HoeEditor::PropertyGrid * GetProp()
{
	return reinterpret_cast<BecherEdit*>(HoeEditor::App::Get()->GetEditor())->GetProp();
}*/

