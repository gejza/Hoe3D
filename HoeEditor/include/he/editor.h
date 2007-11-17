

#ifndef _EDITOR_MAIN_FRAME_H_
#define _EDITOR_MAIN_FRAME_H_

/*#include "treectrl.h"
#include "properties.h"
#include "panels.h"*/
#include "engview.h"
#include <wx/frame.h>

class wxPanel;
class XHoeFS;

// musi byt nadefinovano v aplikaci
extern const char * EditorName; 

namespace HoeEditor {

enum {
	ID_QUIT = wxID_HIGHEST + 1,
	ID_ABOUT,
	ID_OPEN,
	ID_NEW,
	ID_CLOSE,
	ID_ENGINE,
	ID_SAVE,
	ID_SAVEAS,
	ID_SAVEALL,
	ID_VIEWFULLSCREEN,
	ID_SHOWLOG,

	ID_CUSTOMMENU_FIRST,
};

class EngineView;
class EditorTool;

class ToolEventHandler : public EngineEventHandler
{
protected:
	EditorTool * m_tool;
public:
	ToolEventHandler();
	virtual ~ToolEventHandler();
	virtual void MouseLeftDown(const int x, const int y, wxMouseEvent & e);
	virtual void MouseLeftUp(const int x, const int y, wxMouseEvent & e);
	virtual void MouseRightDown(const int x, const int y, wxMouseEvent & e);
	virtual void MouseRightUp(const int x, const int y, wxMouseEvent & e);
	virtual void MouseWheel(wxMouseEvent & e);
	virtual void MouseMove(int relX, int relY, int absX, int absY, const wxMouseEvent & ev);
	virtual void MouseEnter(int absX, int absY);
	virtual void MouseLeave();

	void SetTool(EditorTool * tool);
};

// Define a new frame type: this is going to be our main frame
class BaseEditor: public wxFrame
{
protected:
	wxMenuBar * m_menu;
public:
	BaseEditor();
	~BaseEditor();
	bool Create(const wxString & title);
	/**
	* Post init faze
	* @return vraci true pokud potrebuje dalsi fazi
	*/
	virtual bool OnPostInit() { return false; }
	/**
	* Funkce je zavolana pokud kterekoliv ovladaci prvky potrebuji update
	*/
	virtual void UpdateControls() {}

	virtual XHoeFS * GetFS() { return NULL; }

#ifndef HOE_STATIC_ENGINE
	void LoadEngine(wxString path, int sdkver);
	void OnEngine(wxCommandEvent& event);
#endif

	void ProcessFrame();

	// events
	void OnViewFullscreen(wxCommandEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnShowLog(wxCommandEvent& event);

	virtual void OnDefaultTool() {}

	// any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};


}; // napespace HoeEditor

#endif // _EDITOR_MAIN_FRAME_H_
