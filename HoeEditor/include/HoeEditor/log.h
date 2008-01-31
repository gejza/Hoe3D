
#ifndef _HOE_EDITOR_LOG_H_
#define _HOE_EDITOR_LOG_H_

namespace HoeEditor {

class LogDialog;

class Log : public wxLog
{
	FILE * filelog;
	bool filecreated;
	virtual bool LogFilter(const wxString & msg);
	bool querydialog;
	wxArrayString m_messages;      // the log message texts
    wxArrayInt    m_severity;      // one of wxLOG_XXX values
    wxArrayLong   m_times;         // the time of each message
    //bool          m_bErrors,        // do we have any errors?
    //              m_bWarnings,      // any warnings?
    //              m_bHasMessages;   // any messages at all?
	int           m_lastshowerror;
	//LogDialog * m_dialog;
public:
	Log();
	~Log();
    // implement sink function
    virtual void DoLogString(const wxChar *szString, time_t t);

	bool SetLogFile(FILE * flog);
	bool SetLogFile(const wxString & fname);

	bool SaveLog(wxFile & file, bool withtime);

	void ShowDialog();

	bool GetReqDialog();

    //DECLARE_NO_COPY_CLASS(Log) 
};

class LogDialog : public wxDialog
{
	wxListCtrl * m_listctrl;
	wxToolBar * m_tool;
	int lastitem;
	Log * m_log;
public:
	LogDialog(Log * log);
	~LogDialog();
	void AddMessage(int sev, time_t t, const wxString & msg);
	void OnSize(wxSizeEvent & e);
	void UpdateSize();
	int OpenLogFile(wxFile& file, wxString *pFilename);
	void OnSave(wxCommandEvent &);

	DECLARE_EVENT_TABLE()
};

};

#endif // _HOE_EDITOR_LOG_H_
