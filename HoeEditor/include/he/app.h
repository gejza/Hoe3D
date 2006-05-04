
#ifndef _APP_H_
#define _APP_H_

#include <wx/app.h>
/*#include "plugins.h"
#include "filemgr.h"
#include "project.h"
#include "mainframe.h"*/
#include "base_console.h"

void CreateEditor();

namespace HoeEditor {

class BaseEditor;
class Log;

// Define a new application type, each program should derive a class from wxApp
class App : public wxApp
{
protected:
	BaseEditor * m_editor;
	EditBaseConsole m_con;
	/*PluginMgr m_plugins;
	FileMgr m_files;*/
	//wxLocale m_locale;
	Log * m_log;

	int m_state;
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();

	virtual int OnExit();
	//virtual int OnRun();
	virtual bool ProcessIdle();
	
	virtual BaseEditor * CreateEditor() = 0;
	virtual void OnPostInit() {}
	BaseEditor * GetEditor() { return m_editor; }
	/*FileMgr * GetFileMgr() { return &m_files; }
	MainFrame * GetMainFrame() { return m_frame; }
	EditorExport * GetExport() { return &m_export; }
	EditorProject * GetProject() { return &m_project; }*/
	void StateToEnd() { m_state = 100; }
	void Process();
	EditBaseConsole * GetConsole() { return &m_con; }
	Log * GetLog() { return m_log; }

	static inline App * Get() { return reinterpret_cast<App*>(wxTheApp); }

};

}; // namespace HoeEditor


#endif // _APP_H_



