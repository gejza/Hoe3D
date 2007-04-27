
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
	ID_FIRSTITEM,
	ID_COLORRECT,
	ID_STATICITEM,
	ID_BUTTONITEM,
	ID_INFOITEM,
	ID_DIGICOUNTERITEM,
	ID_FONTITEM,
	ID_LASTITEM,
	ID_SHOWRES,
	ID_BACKGROUND,
};

BEGIN_EVENT_TABLE(Hoe2DEdit, HoeEditor::BaseEditor)
	EVT_MENU(HoeEditor::ID_NEW, Hoe2DEdit::OnNewFile)
	EVT_MENU(HoeEditor::ID_SAVE, Hoe2DEdit::OnSaveFile)
	EVT_MENU(HoeEditor::ID_OPEN, Hoe2DEdit::OnOpenFile)
	EVT_MENU(ID_SHOWRES, Hoe2DEdit::OnResMgr)
	EVT_TREE_SEL_CHANGED(ID_TREE, Hoe2DEdit::OnTreeSelect)
	//EVT_TREE_BEGIN_DRAG(ID_TREE, Hoe2DEdit::OnTreeDrag)

	EVT_MENU_RANGE(ID_FIRSTITEM, ID_LASTITEM, Hoe2DEdit::OnNewItem)

END_EVENT_TABLE()


HoeEditor::BaseEditor * Hoe2DEditApp::CreateEditor()
{
    //wxImage::AddHandler(new wxPNGHandler);
    SetVendorName(_T("Hoe"));
    SetAppName(_T("Hoe2DEditor")); // not needed, it's the default value

    wxConfigBase *pConfig = wxConfigBase::Get();

    // uncomment this to force writing back of the defaults for all values
    // if they're not present in the config - this can give the user an idea
    // of all possible settings for this program
    pConfig->SetRecordDefaults();

	// load lang
	m_locale.Init(wxLANGUAGE_CZECH);
	wxLocale::AddCatalogLookupPathPrefix("resource");
	wxLocale::AddCatalogLookupPathPrefix("../Hoe/HoeEditor/lang");

	m_locale.AddCatalog("editor_cs");
	m_locale.AddCatalog("cs_CZ");

	Hoe2DEdit * e = new Hoe2DEdit();
	e->Create("Hoe2D Editor");
    wxBitmap bitmap;
    if (bitmap.LoadFile(_T("splash.bmp"), wxBITMAP_TYPE_BMP))
	m_splash = new wxSplashScreen(bitmap,
            wxSPLASH_CENTRE_ON_PARENT/*|wxSPLASH_TIMEOUT*/,
            4000, e, wxID_ANY, wxDefaultPosition, wxDefaultSize,
            wxSIMPLE_BORDER|wxSTAY_ON_TOP|wxFRAME_NO_TASKBAR);
	else 
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
#include "../../HoeEditor/resources/new.xpm"
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
    m_mgr.SetManagedWindow(this);
	// tool bar

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
	menuView->AppendCheckItem(ID_BACKGROUND, _("Change Background Image..."), _("Load background image."));

	wxMenu * menuTools = new wxMenu;
	menuTools->Append(ID_SHOWRES, _("&Resource Manager..."), _("Resource Manager"));
#ifndef HOE_STATIC
	menuTools->Append(HoeEditor::ID_ENGINE, _("&Load Engine...\tF10"), _("Load engine from library"));
#endif

	wxMenu * menuInsert = new wxMenu;
	menuInsert->Append(ID_STATICITEM, _("Picture Item"),_("New picture item."));
	menuInsert->Append(ID_COLORRECT, _("Color Rect Item"),_("New color Rect item."));
	menuInsert->Append(ID_INFOITEM, _("Info Item"),_("New info item."));
	menuInsert->Append(ID_BUTTONITEM, _("Button Item"),_("New button item."));
	menuInsert->Append(ID_DIGICOUNTERITEM, _("Digi counter Item"),_("New digi counter item."));
	menuInsert->Append(ID_FONTITEM, _("Font Item"),_("New font item."));

    // now append the freshly created menu to the menu bar...
    m_menu = new wxMenuBar(/*wxMB_DOCKABLE*/);
    m_menu->Append(menuFile, _("&File"));
    m_menu->Append(menuInsert, _("&Insert"));
    m_menu->Append(menuView, _("&View"));
    m_menu->Append(menuTools, _("&Tools"));
	
    // ... and attach this menu bar to the frame
    SetMenuBar(m_menu); 

	wxStatusBar *statbar = CreateStatusBar();
	statbar->SetFieldsCount(2);

	wxToolBar * tool = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                   wxTB_FLAT | wxTB_NODIVIDER | wxTB_HORIZONTAL);
	wxBitmap newt(new_xpm);
int w = newt.GetWidth(),
        h = newt.GetHeight(); 
	tool->SetToolBitmapSize(wxSize(w, h));

	tool->AddTool(HoeEditor::ID_NEW,"nic",newt,"hokus pokus");
	tool->AddSeparator();
	tool->AddTool(ID_COLORRECT, "Color Rect", newt, "Color Rect");
	tool->AddTool(ID_STATICITEM, "picture", newt, "Picture");
	tool->Realize();

	// seradit do layoutu

	/*GetPanelMgr()->AddPanel(
		new ToolObjects(GetPanelMgr()), _("Tools"), true, true);*/
	/*GetPanelMgr()->AddPanel(
		new TerrainObject(GetPanelMgr()), _("Terrain"), true, true);*/
	// list view
	m_tc = new wxTreeCtrl(this, ID_TREE);

	m_prop = new HoeEditor::PropertyGrid();
	m_prop->Create(this);

	m_engview = new HoeEditor::EngineView();
	m_engview->Create(this, -1, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);

    m_mgr.AddPane(m_tc, wxLEFT, wxT("List"));
    m_mgr.AddPane(m_prop, wxLEFT, wxT("Properties"));
    m_mgr.AddPane(m_engview, wxAuiPaneInfo().CenterPane());

    m_mgr.AddPane(tool, wxAuiPaneInfo().
                  Name(wxT("tb")).Caption(wxT("Sample Vertical Toolbar")).
                  ToolbarPane().Top().
                  Gripper());

	m_mgr.Update();

	return true;
}

bool Hoe2DEdit::OnPostInit()
{
	wxString eng = 	wxConfigBase::Get()->Read(wxT("/engine"),wxT(""));
	if (eng != "")
	{
		LoadEngine(eng, HOESDK_VERSION);
	}
	m_res.LoadMainDir(wxConfigBase::Get()->Read(wxT("/ResourceDir"),wxT("")));
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
	m_figure->Create(HoeGame::GetHoeEngine());

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


void Hoe2DEdit::OnOpenFile(wxCommandEvent &)
{
	wxFileDialog dlg(this,_("Choose a figure file..."),
		_T(""), _T(""), _("Figure file (*.*)|*.*"), 
		wxOPEN | wxFILE_MUST_EXIST); 
	if (dlg.ShowModal() == wxID_OK)
	{
		CloseFile();
		m_figure = new FigureEdit(m_tc);
		m_figure->Create(HoeGame::GetHoeEngine());
		m_figure->SetFilePath(dlg.GetPath());
		m_figure->Load(dlg.GetPath().c_str());
		SetTitle(dlg.GetFilename());
	}
	
	UpdateControls();
}

void Hoe2DEdit::OnSaveFile(wxCommandEvent &e)
{
	if (!m_figure)
		return;

	if (m_figure->GetFilePath().IsEmpty() || e.GetId() == HoeEditor::ID_SAVEAS)
	{
		wxFileDialog dlg( this, _("Save figure file..."),
			_T(""), _T(""), _("Figure file (*.*)|*.*"), wxSAVE);
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

			m_figure->SetFilePath(dlg.GetPath());
		}
		else
			return;
	}

	if (m_figure->SaveFile())
		SetTitle(m_figure->GetFilePath());
}
/*
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
		case ID_INFOITEM:
			m_figure->AddItem(new InfoItem(), "info");
			break;
		case ID_BUTTONITEM:
			m_figure->AddItem(new ButtonItem(), "button");
			break;
		case ID_DIGICOUNTERITEM:
			m_figure->AddItem(new DigiCounterItem(), "counter");
			break;
		case ID_FONTITEM:
			m_figure->AddItem(new FontItem(), "font");
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

void Hoe2DEdit::OnResMgr(wxCommandEvent &)
{
	m_res.ShowDlg();
}
/*
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

