
#include "StdAfx.h"
#include "../include/HoeEditor/log.h"
#include "../include/HoeEditor/app.h"
#include "../include/HoeEditor/editor.h"

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(HoeEditor::LogDialog, wxDialog)
	//EVT_MENU(ID_QUIT, HoeEditor::LogDialog::OnQuit)
	EVT_MENU(ID_SAVE, HoeEditor::LogDialog::OnSave)
	EVT_SIZE(HoeEditor::LogDialog::OnSize)
END_EVENT_TABLE()

#ifdef _LINUX
#define strnicmp strncasecmp
#endif

HoeEditor::Log::Log()
{
	filecreated = false;
	querydialog = false;
	//m_lastshowerror = -1;
}

HoeEditor::Log::~Log()
{
	SetLogFile((FILE*)NULL);
}


bool HoeEditor::Log::SetLogFile(FILE * flog)
{
	if (filecreated)
		fclose(filelog);
	filelog = flog;
	filecreated = false;
	return true;
}

bool HoeEditor::Log::SetLogFile(const wxString & fname)
{
	FILE * f = fopen(fname.c_str(), "wt");
	if (!f)
		return false;

	SetLogFile(f);
	filecreated = true;

	return true;
}


void HoeEditor::Log::DoLogString(const wxChar *szString, time_t t)
{
    wxString msg;
	this->m_messages.Add(szString);
	this->m_times.Add((long) t);

    {
		wxChar buf[256];
		wxStrftime(buf, WXSIZEOF(buf), wxT("%X"), localtime(&t));
		msg << buf << wxT(": ") << szString << wxT('\n');
	}


    //m_pTextCtrl->AppendText(msg);
	if (filelog)
	{
		fputs(msg.c_str(),filelog);
		fflush(filelog);
	}

	// logovani do dialogu
	// zobrazit pri chybe
	bool qf = LogFilter(szString);
	if (qf)
	{
		m_severity.Add(wxLOG_Error);
	}
	else
	{
		m_severity.Add(0);
	}

	if (!querydialog && qf)
	{
		querydialog = true;
	}

}

bool HoeEditor::Log::LogFilter(const wxString & msg)
{
	const char * ps = msg.c_str();
	while (*ps)
	{
		switch (*ps)
		{
		case 'E':
		case 'e':
			if (strnicmp(ps, "error", 5) == 0)
				return true;
			break;
		case 'F':
		case 'f':
			if (strnicmp(ps, "failed", 6) == 0)
				return true;
			break;
		};
		ps++;
	}
	return false;
}

void HoeEditor::Log::ShowDialog()
{
	LogDialog dlg(this);

	// add msg
	size_t count = m_messages.GetCount();
    for ( size_t n = 0; n < count; n++ )
    {
        dlg.AddMessage(m_severity[n], m_times[n], m_messages[n]);
    }

	if (dlg.ShowModal())
	{
	}
	querydialog = false;
}

bool HoeEditor::Log::GetReqDialog()
{
	
	if (querydialog)
	{
		querydialog = false;
		return true;
	}
	return false;
}

bool HoeEditor::Log::SaveLog(wxFile &file, bool time)
{
	size_t count = m_messages.GetCount();
    for ( size_t n = 0; n < count; n++ )
    {
		wxString str;
		if (time)
		{
			wxChar buff[256];
			time_t t = m_times[n];
			wxStrftime(buff, WXSIZEOF(buff), "%X", localtime(&t) );
			str << buff << wxT(": ");
		}
		str << m_messages[n] << wxT('\n');
		file.Write(str);
    }
	return true;
}

/////////////////////////////////////
// dialog

#include "../resources/save.xpm"

HoeEditor::LogDialog::LogDialog(Log * log)
{
	m_log = log;

	Create(App::Get()->GetEditor(), -1, _("Log..."), wxDefaultPosition, wxSize(500,450), wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);

	m_tool = new wxToolBar(/*style, ID_TOOLBAR*/);
	m_tool->Create(this, -1, wxPoint(0,0), wxSize(50,50), (/*wxBORDER_NONE | */wxTB_HORIZONTAL | wxTB_FLAT));
    //SetToolBar(tb);
	m_tool->SetToolBitmapSize(wxSize(24, 24));
	m_tool->AddTool(ID_SAVE, wxBitmap(save_xpm), wxString(_("Save log")), wxString(_("Save log to custom file.")));
	m_tool->AddSeparator();
	m_tool->Realize();

    m_listctrl = new wxListCtrl(this, wxID_ANY,
                                wxPoint(5,40), wxSize(480, 400),
                                wxSUNKEN_BORDER |
                                wxLC_REPORT |
                                wxLC_NO_HEADER |
                                wxLC_SINGLE_SEL);

    m_listctrl->InsertColumn(0, _T("Message"));
    m_listctrl->InsertColumn(1, _T("Time"), wxLIST_FORMAT_RIGHT);

    // prepare the imagelist
    static const int ICON_SIZE = 16;
    wxImageList *imageList = new wxImageList(ICON_SIZE, ICON_SIZE);

    // order should be the same as in the switch below!
    static const wxChar* icons[] =
    {
        wxART_ERROR,
        wxART_WARNING,
        wxART_INFORMATION
    };

	wxBitmap bmp;
	bmp = wxArtProvider::GetBitmap(wxART_ERROR, wxART_MESSAGE_BOX,
                                                wxSize(ICON_SIZE, ICON_SIZE));
	imageList->Add(bmp);

	bmp = wxArtProvider::GetBitmap(wxART_WARNING, wxART_MESSAGE_BOX,
                                                wxSize(ICON_SIZE, ICON_SIZE));
	imageList->Add(bmp);

    m_listctrl->SetImageList(imageList, wxIMAGE_LIST_SMALL);

	wxSize s = this->GetClientSize();
	wxRect rt = m_tool->GetRect();
	m_listctrl->SetSize(5, rt.GetBottom() + 5, s.GetWidth() - 10, s.GetHeight() - rt.GetBottom() - 10);

	lastitem = 0;
	Update();
	UpdateSize();
}

HoeEditor::LogDialog::~LogDialog()
{
	if ( m_listctrl )
    {
        delete m_listctrl->GetImageList(wxIMAGE_LIST_SMALL);
    }
}

void HoeEditor::LogDialog::OnSize(wxSizeEvent & e)
{
	UpdateSize();
}

void HoeEditor::LogDialog::UpdateSize()
{
	wxSize s = this->GetClientSize();
	wxRect rt = m_tool->GetRect();
	m_listctrl->SetSize(5, rt.GetBottom() + 5, s.GetWidth() - 10, s.GetHeight() - rt.GetBottom() - 10);
	rt = m_listctrl->GetClientRect();
    m_listctrl->SetColumnWidth(1, wxLIST_AUTOSIZE);
	rt.width -= m_listctrl->GetColumnWidth(1) + 10;
	m_listctrl->SetColumnWidth(0, rt.width);
}

void HoeEditor::LogDialog::AddMessage(int sev, time_t t, const wxString & msg)
{
	int image = -1;
	switch ( sev )
	{
	case wxLOG_Error:
		image = 0;
		break;
	case wxLOG_Warning:
		image = 1;
		break;
	}

	m_listctrl->InsertItem(lastitem, msg, image);

	wxChar buf[4096];
    wxStrftime(buf, WXSIZEOF(buf), "%X", localtime(&t) );

	m_listctrl->SetItem(lastitem, 1, buf);
	lastitem++;

	m_listctrl->ScrollLines(lastitem);
}

int HoeEditor::LogDialog::OpenLogFile(wxFile &file, wxString *pFilename)
{
    // get the file name
    // -----------------
	wxFileDialog dialog( this,_("Save log to file ..."),_T(""), _T("log.txt"), _("Txt Files (*.txt)|*.txt"), wxSAVE | wxOVERWRITE_PROMPT);
	if (dialog.ShowModal() != wxID_OK)
	{
		return -1;
	}

    wxString filename = dialog.GetPath();

    // open file
    // ---------
    bool bOk wxDUMMY_INITIALIZE(false);
    if ( wxFile::Exists(filename) ) {
        bool bAppend = false;
        wxString strMsg;
        strMsg.Printf(_("Append log to file '%s' (choosing [No] will overwrite it)?"),
                      filename.c_str());
        switch ( wxMessageBox(strMsg, _("Question"),
                              wxICON_QUESTION | wxYES_NO | wxCANCEL) ) {
            case wxYES:
                bAppend = true;
                break;

            case wxNO:
                bAppend = false;
                break;

            case wxCANCEL:
                return -1;

            default:
                wxFAIL_MSG(_("invalid message box return value"));
        }

        if ( bAppend ) {
            bOk = file.Open(filename, wxFile::write_append);
        }
        else {
            bOk = file.Create(filename, true /* overwrite */);
        }
    }
    else {
        bOk = file.Create(filename);
    }

    if ( pFilename )
        *pFilename = filename;

    return bOk;
}

void HoeEditor::LogDialog::OnSave(wxCommandEvent &)
{
	wxFile file;
	wxString filename;
	int rc = OpenLogFile(file, &filename);
	if (rc == -1)
		// cancel
		return;

    bool bOk = rc != 0;
	if ( bOk )
		m_log->SaveLog(file, true);

    if ( bOk )
        bOk = file.Close();

    if ( !bOk ) {
        wxLogError(_("Can't save log contents to file."));
    }
    else {
        wxLogStatus( _("Log saved to the file '%s'."), filename.c_str());
    }
}
