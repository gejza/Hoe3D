
#include "StdAfx.h"
#include "../include/he/app.h"
#include "../include/he/editor.h"
#include "../include/he/engview.h"
#include "../../HoeGame/include/hoe_utils.h"

namespace HoeEditor {

BEGIN_EVENT_TABLE(EngineView, wxWindow)
	EVT_PAINT(EngineView::OnPaint)
	EVT_MOTION(EngineView::OnMouseMove)
	EVT_MOUSEWHEEL(EngineView::OnMouseWheel)
	EVT_LEFT_DOWN(EngineView::OnMouseDown)
	EVT_LEFT_UP(EngineView::OnMouseUp)
	EVT_RIGHT_DOWN(EngineView::OnMouseDown)
	EVT_RIGHT_UP(EngineView::OnMouseUp)
	EVT_KEY_DOWN(EngineView::OnKeyDown)
	EVT_KEY_UP(EngineView::OnKeyUp)
	EVT_CHAR(EngineView::OnChar)
	EVT_ENTER_WINDOW( EngineView::OnEnterWindow )
	EVT_LEAVE_WINDOW( EngineView::OnLeaveWindow )
	EVT_SET_FOCUS(EngineView::OnSetFocus)
	EVT_KILL_FOCUS( EngineView::OnKillFocus ) 
	EVT_SIZE(EngineView::OnSize)
END_EVENT_TABLE()

EngineView::EngineView()
{
	m_engine = NULL;
#ifndef HOE_STATIC_ENGINE
	m_dllpath[0] = '\0';
#endif
	m_loaded = false;
	SetBackgroundColour(wxColor(57,67,140));
}

#ifndef HOE_STATIC_ENGINE
void EngineView::SetEnginePath(const char * path)
{
	strncpy(m_dllpath, path, sizeof(m_dllpath) - 1);
}
#endif

#ifdef HOE_STATIC_ENGINE
bool EngineView::InitStatic(XHoeFS * hfs)
{
	wxLogMessage("* Load Engine *");
	this->m_engine = CreateHoeEngine(HOESDK_VERSION, App::Get()->GetConsole(),hfs,NULL,0,HOEF_NOINPUT);
#else
bool EngineView::Init(XHoeFS * hfs)
{
	bool ret;
	BEGIN_TRY
		ret = InitUntry(hfs);
	END_TRY(ret = false);
#ifndef HOE_STATIC_ENGINE
	if (!ret)
		m_lib.Unload();
#endif
	return ret;
}

bool EngineView::InitUntry(XHoeFS * hfs)
{
	Unload();

	assert(strlen(m_dllpath) > 0);
	
	HOE_FUNCCREATE GetEngineInterface;
	
	if (!m_lib.Load(m_dllpath))
	{
		wxLogMessage(_("Failed load library: %s"),m_dllpath);
		return false;
	}

	GetEngineInterface = (HOE_FUNCCREATE)m_lib.GetSymbol(HOE_FUNCCREATENAME);
	if (!GetEngineInterface)
	{
		wxLogMessage(_("Find symbol GetEngineInterface failed."));
		return false;
	}

	wxLogMessage("* Load Engine from %s *",m_dllpath);
	this->m_engine = GetEngineInterface(HOESDK_VERSION,App::Get()->GetConsole(),hfs,NULL,0,HOEF_NOINPUT);
#endif
	if (m_engine == NULL)
		return false; 

	{
	THoeInitSettings his;
	memset(&his,0,sizeof(THoeInitSettings));
	his.hInst = NULL;

#ifdef __WXGTK__
	// ftp://biolpc22.york.ac.uk/pub/contrib/glcanvas/glcanvs2.zip
	GdkWindow * bwin = GTK_PIZZA(m_wxwindow)->bin_window;
	his.dpy = GDK_WINDOW_XDISPLAY( bwin );
	his.win = GDK_WINDOW_XWINDOW( bwin );
	his.screen = DefaultScreen(his.dpy);//GDK_WINDOW_SCREEN( m_wxwindow->window);
	//his.screen = GDK_SCREEN_XSCREEN(m_wxwindow->screen);
#else
	his.dpy = NULL;
	his.win = (HOE_WINDOW)this->GetHandle();
#endif

	wxSize s = GetClientSize();

	his.width = s.GetWidth();
	his.height = s.GetHeight();
	his.fullscreen = false;
	his.forcewnd = true; 
	his.depth = 16;

	wxLogMessage("* Init Engine with %dx%dx%d *",his.width,his.height,his.depth);

	if (!this->m_engine->Init(&his))
		return false;
	}
	m_loaded = true;

	// arial
	HoeGetRef(m_engine)->SetBackgroundColor(0xff000000);
	
	return true;
}

static wxString str; /*!!!*/ /** @todo odstranit tuhleprasarnicku */

void EngineView::Frame(float dtime)
{
	if (!this->IsLoaded())
		return;


	BEGIN_TRY

	assert(m_engine);
	this->m_engine->Process(dtime);
	this->m_engine->Frame();

	// update fps
	// info

	//float fps = HoeGetInfo(m_engine)->GetFPS();
	str.Printf("FPS: %f", HoeGetInfo(m_engine)->GetFPS());
	App::Get()->GetEditor()->SetStatusText(str, 1);

	END_TRY(exit(0));
}


void EngineView::Unload()
{
	if (!this->IsLoaded())
		return;
	
	assert(m_engine);

	this->m_engine->Destroy();
	m_engine = NULL;
#ifndef HOE_STATIC_ENGINE
	m_lib.Unload();
#endif

	m_loaded = false;
}

void EngineView::OnPaint(wxPaintEvent& event)
{
 	wxPaintDC dc(this);
	if (IsLoaded())
	{
		this->Frame(0);
	}
	else
	{
		dc.SetTextForeground(*wxWHITE);
        dc.DrawLabel(_("Engine not loaded."), GetClientRect(), wxALIGN_CENTER);
	}
	event.Skip();
}

void EngineView::OnSize( wxSizeEvent& event)
{
	if (m_loaded)
	{
		assert(m_engine);
		int x = 0;
		int y = 0;
		GetClientSize( &x, &y );

		m_engine->Resize(x,y);
		Frame(0);
	}
	else
		Refresh();
}

void EngineView::OnEnterWindow( wxMouseEvent& event )
{
    SetFocus();
	App::Get()->GetEditor()->MouseEnter(event.GetX(), event.GetY());
    m_lastmouseevent = event;
	event.Skip();
}

void EngineView::OnLeaveWindow( wxMouseEvent& event )
{
	App::Get()->GetEditor()->MouseLeave();
	event.Skip();
}


void EngineView::OnMouseMove(wxMouseEvent& event)
{
	App::Get()->GetEditor()->MouseMove(event.GetX()-m_lastmouseevent.GetX(),event.GetY()-m_lastmouseevent.GetY(),
			event.GetX(),event.GetY(), event);
	/*if (m_active)
	{
	if (event.ControlDown())
			flag |= HoePlugin::MControl;
		if (event.MetaDown())
			flag |= HoePlugin::MMeta;
		if (event.AltDown())
			flag |= HoePlugin::MAlt;
		if (event.ShiftDown())
			flag |= HoePlugin::MShift;
		if (event.CmdDown())
			flag |= HoePlugin::MCmd;
		if (event.MiddleIsDown())
			flag |= HoePlugin::MMiddle;
		if (event.RightIsDown())
			flag |= HoePlugin::MRight;
		if (event.LeftIsDown())
			flag |= HoePlugin::MLeft;

		if (event.GetWheelRotation() != 0)
		{
			m[0] = event.GetWheelRotation();
        	m_active->Msg(HoePlugin::EPM_EngMouseWheel,flag,m);
		}

		m_active->Msg(HoePlugin::EPM_EngMouseMove,flag,m);

	}*/
	m_lastmouseevent = event;
	event.Skip();
}

void EngineView::OnMouseUp(wxMouseEvent& event)
{
	if (event.LeftUp())
		App::Get()->GetEditor()->MouseLeftUp(event.GetX(),event.GetY(),event);
	if (event.RightUp())
		App::Get()->GetEditor()->MouseRightUp(event.GetX(),event.GetY(),event);
	/*if (m_active)
	{
		float m[4] = {event.GetX()-m_lastmouseevent.GetX(),event.GetY()-m_lastmouseevent.GetY(),
			event.GetX(),event.GetY()};
		int flag = 0;;

		if (event.ControlDown())
			flag |= HoePlugin::MControl;
		if (event.MetaDown())
			flag |= HoePlugin::MMeta;
		if (event.AltDown())
			flag |= HoePlugin::MAlt;
		if (event.ShiftDown())
			flag |= HoePlugin::MShift;
		if (event.CmdDown())
			flag |= HoePlugin::MCmd;
		if (event.MiddleUp())
			flag |= HoePlugin::MMiddle;
		if (event.RightUp())
			flag |= HoePlugin::MRight;
		if (event.LeftUp())
			flag |= HoePlugin::MLeft;

		m_active->Msg(HoePlugin::EPM_EngMouseUp,flag,m);

	}*/
	m_lastmouseevent = event;
	event.Skip();
}

void EngineView::OnMouseDown(wxMouseEvent& event)
{
	if (event.LeftDown())
		App::Get()->GetEditor()->MouseLeftDown(event.GetX(),event.GetY(),event);
	if (event.RightDown())
		App::Get()->GetEditor()->MouseRightDown(event.GetX(),event.GetY(),event);

	/*if (m_active)
	{
		float m[4] = {event.GetX()-m_lastmouseevent.GetX(),event.GetY()-m_lastmouseevent.GetY(),
			event.GetX(),event.GetY()};
		int flag = 0;;

		if (event.ControlDown())
			flag |= HoePlugin::MControl;
		if (event.MetaDown())
			flag |= HoePlugin::MMeta;
		if (event.AltDown())
			flag |= HoePlugin::MAlt;
		if (event.ShiftDown())
			flag |= HoePlugin::MShift;
		if (event.CmdDown())
			flag |= HoePlugin::MCmd;
		if (event.MiddleDown())
			flag |= HoePlugin::MMiddle;
		if (event.RightDown())
			flag |= HoePlugin::MRight;
		if (event.LeftDown())
			flag |= HoePlugin::MLeft;

		m_active->Msg(HoePlugin::EPM_EngMouseDown,flag,m);

	}*/
	m_lastmouseevent = event;
	event.Skip();
}

void EngineView::OnMouseWheel(wxMouseEvent& event)
{
	App::Get()->GetEditor()->MouseWheel(event);
}

void EngineView::OnKeyDown(wxKeyEvent& event)
{
	App::Get()->GetEditor()->KeyDown(event);

/*
        case WXK_NUMPAD_DECIMAL: key = _T("NUMPAD_DECIMAL"); break;
        case WXK_NUMPAD_DIVIDE: key = _T("NUMPAD_DIVIDE"); break;

        default:
        {
            if ( keycode < 256 )
            {
                if ( keycode == 0 )
                    key.Printf(_T("NUL"));
                else if ( keycode < 27 )
                    key.Printf(_T("Ctrl-%c"),
                                (unsigned char)(_T('A') + keycode - 1));
                else
                    key.Printf(_T("'%c'"), (unsigned char)keycode);
            }
            else
            {
                key.Printf(_T("unknown (%ld)"), keycode);
            }
        } 
*/

	//App::Get()->GetEditor()->SetTitle();
	event.Skip();
}

void EngineView::OnKeyUp(wxKeyEvent& event)
{
	App::Get()->GetEditor()->KeyUp(event);
	event.Skip();
}

void EngineView::OnChar(wxKeyEvent& event)
{
	event.Skip();
}

void EngineView::OnSetFocus(wxFocusEvent &event)
{
    //GetApp()->GetMainFrame()->SetTitle("focused");
	event.Skip();
}


void EngineView::OnKillFocus(wxFocusEvent &even)
{
    //GetApp()->GetMainFrame()->SetTitle("unfocused");
    //event.Skip();
}

};




