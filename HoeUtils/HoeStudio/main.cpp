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
    // tell wxAuiManager to manage this frame
    m_mgr.SetManagedWindow(this);

    // min size for the frame itself isn't completely done.
    // see the end up wxAuiManager::Update() for the test
    // code. For now, just hard code a frame minimum size
    SetMinSize(wxSize(400,300));

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
    prop->End();
    
    // "commit" all changes made to wxAuiManager
    m_mgr.Update();

    view->SetEnginePath("/home/milan/heimdall/Hoe/bin/Hoe3D.so");
    view->Init(new HoeEditor::EditorFS());
}

MyFrame::~MyFrame()
{
    m_mgr.UnInit();
}

