///////////////////////////////////////////////////////////////////////////////
// Name:        auidemo.cpp
// Purpose:     wxaui: wx advanced user interface - sample/test program
// Author:      Benjamin I. Williams
// Modified by:
// Created:     2005-10-03
// RCS-ID:      $Id: auidemo.cpp,v 1.44 2006/11/21 15:20:15 BIW Exp $
// Copyright:   (C) Copyright 2005, Kirix Corporation, All Rights Reserved.
// Licence:     wxWindows Library Licence, Version 3.1
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "StdAfx.h"
#include <he/properties.h>
#include <he/engview.h>
#include <he/fs.h>
#include <he/app.h>
#include <he/editor.h>

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

// -- application --

class Hoe2DEditApp : public HoeEditor::App
{
public:
	virtual HoeEditor::BaseEditor * CreateEditor();
	virtual bool OnPostInit();
};


DECLARE_APP(Hoe2DEditApp)
IMPLEMENT_APP(Hoe2DEditApp)


// -- frame --

class MyFrame : public HoeEditor::BaseEditor
{
public:
    MyFrame();

    ~MyFrame();

    wxAuiDockArt* GetDockArt();
    void DoUpdate();

private:
    wxAuiManager m_mgr;

    DECLARE_EVENT_TABLE()
};


HoeEditor::BaseEditor * Hoe2DEditApp::CreateEditor()
{
    MyFrame* frame = new MyFrame();
    SetTopWindow(frame);
    frame->Show();

    return frame;
}

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
END_EVENT_TABLE()

const char * EditorName = "Hoe2DEditor";

bool Hoe2DEditApp::OnPostInit()
{
	return false;
}


MyFrame::MyFrame()
        
{
	HoeEditor::BaseEditor::Create("Okynko");
    // tell wxAuiManager to manage this frame
    m_mgr.SetManagedWindow(this);

    // min size for the frame itself isn't completely done.
    // see the end up wxAuiManager::Update() for the test
    // code. For now, just hard code a frame minimum size
    SetMinSize(wxSize(400,300));

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

    // create some center panes
    wxTextCtrl* text1 = new wxTextCtrl(this, -1, _("Pane 1 - sample text"),
                     wxDefaultPosition, wxSize(200,150),
                     wxNO_BORDER | wxTE_MULTILINE);
                                       
    wxTextCtrl* text3 = new wxTextCtrl(this, -1, _("Main content window"),
                     wxDefaultPosition, wxSize(200,150),
                     wxNO_BORDER | wxTE_MULTILINE);
    HoeEditor::PropertyGrid * prop = new HoeEditor::PropertyGrid();
    prop->Create(this);
    HoeEditor::EngineView * view = new HoeEditor::EngineView();
    view->Create(this);
    //wx engine 
    // add the panes to the manager
    m_mgr.AddPane(text1, wxLEFT, wxT("Pane Number One"));
    m_mgr.AddPane(prop, wxLEFT, wxT("Properties"));
    m_mgr.AddPane(view, wxAuiPaneInfo().Centre());
    m_mgr.AddPane(text3, wxAuiPaneInfo().
                  Name(wxT("test7")).Caption(wxT("Client Size Reporter")).
                  CloseButton(true).MaximizeButton(true).MinimizeButton(true).
                  Bottom().PaneBorder(true));

    prop->Begin(NULL);
    prop->AppendCategory("Kategorie");
    prop->AppendString(0, "String", "hodnota", 0, "Nastavuje jen testovaci hodnotu");
	prop->AppendAngle(1, "Uhel", 2.1f, 0, "Nastaveni uhlu");
	prop->AppendColor(2, "Barva", 0xff00eecc, 0, "Vyber barvy");
	const char * list[] = {"A","B","C",NULL}; long values[] = {1,0,2,0};
	prop->AppendList(3, "Vyber", list, values, 0, 0, "Vyber z listu hodno (maskovane jako cisla)");
    prop->End();
    
    // "commit" all changes made to wxAuiManager
    m_mgr.Update();

    view->SetEnginePath("../../bin/Hoe3Dd_D3D9.dll");
    view->Init(new HoeEditor::EditorFS());
}

MyFrame::~MyFrame()
{
    m_mgr.UnInit();
}

