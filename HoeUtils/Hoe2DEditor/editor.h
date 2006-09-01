
/**
   @file    editor.h
   @date    Aug 2006
   @version 1.0
   @brief   Soubor s deklaraci hlavnich trid 2d editoru.
*/

#ifndef _HOE_2D_EDITOR_H_
#define _HOE_2D_EDITOR_H_

class FigureEdit;
class BaseItem;

class Hoe2DEdit : public HoeEditor::BaseEditor
{
	static Hoe2DEdit * s_actinstance;
protected:
	HoeEditor::PropertyGrid *m_prop;
	HoeEditor::EngineView *m_engview;
	HoeEditor::PanelMgr *m_leftpanel;
	wxTreeCtrl * m_tc;
	wxHelpController m_help;
	HoeEditor::UndoList m_undo;

	FigureEdit * m_figure;
	HoeEditor::Resources m_res;
	BaseItem * m_select;
public:
	Hoe2DEdit();
	virtual ~Hoe2DEdit();
	static inline Hoe2DEdit * Get() { assert(s_actinstance); return s_actinstance; }
	bool Create(const wxString & title);
	virtual bool OnPostInit();

	virtual XHoeFS * GetFS() { return &m_res; }
	HoeEditor::PropertyGrid * GetProp() { return m_prop; }
	virtual HoeEditor::EngineView * GetEngineView() { return m_engview; }
	virtual HoeEditor::PanelMgr * GetPanelMgr() { return m_leftpanel; }

	void OnNewObject(wxCommandEvent &);
	void OnResMgr(wxCommandEvent &);
	void OnNewFile(wxCommandEvent &);
	void OnOpenFile(wxCommandEvent &);
	void OnSaveFile(wxCommandEvent &);
	void OnHelp(wxCommandEvent &);
	void OnAbout(wxCommandEvent &);
	void OnTypeShow(wxCommandEvent &);
	void OnNewItem(wxCommandEvent &);

	void OnTreeSelect(wxTreeEvent &);
	void OnTreeDrag(wxTreeEvent & e);

	virtual void KeyDown(wxKeyEvent& event);
	virtual void KeyUp(wxKeyEvent& event);

	//void UpdateControls();
	void CloseFile();

	void SetStatus(const wxString & str);

	void AddUndo(HoeEditor::UndoAction * action);

    DECLARE_EVENT_TABLE()
protected:
	// updejt menu
	virtual void UpdateControls();
};

class Hoe2DEditApp : public HoeEditor::App
{
	wxSplashScreen *m_splash;
	wxLocale m_locale;
public:
	virtual HoeEditor::BaseEditor * CreateEditor();
	virtual bool OnPostInit();
};

#endif // _HOE_2D_EDITOR_H_

