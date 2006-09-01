
#ifndef _BECHER_EDITOR_RESOURCES_H_
#define _BECHER_EDITOR_RESOURCES_H_

//#include <wx/propdlg.h>
//#include <hoe_resource_mgr.h>
#include "fs.h"

class XHoeFile;

namespace HoeEditor {

struct ResourceItem
{
	wxString name;
	wxString id;
	wxString cmd;
};

class FileItem
{
public:
	wxString path;
	XHoeFile * file;
	FileItem()
	{
		file = NULL;
	}
	FileItem(const FileItem &item)
	{
		path = item.path;
		file = NULL;
	}	
	~FileItem()
	{
		if (file) delete file;//file->Release();
	}
};

WX_DECLARE_OBJARRAY(ResourceItem, ArrayResources);
WX_DECLARE_OBJARRAY(FileItem, ArrayFiles);

class ResourceConfigFile
{
public:
	bool LoadConf(const wxString path);
	virtual void AddFile(wxString file) = 0;
	virtual void AddResource(ResourceItem & ri) = 0;
	virtual void ClearAll() = 0;

};

//////////////////////////////////////////////////////////

class Resources : public HoeGame::ResourceMgr, public EditorFS, public ResourceConfigFile
{
protected:
	wxString m_mainDir;
	ArrayFiles m_files;
	ArrayResources m_res;
protected:
	virtual IHoeResource * MissingResource(int id);
	virtual bool ReqResource(const char * name);
public:
	Resources() { }
	~Resources() { }
	wxString GetMainDir() const { return m_mainDir; }
	void SetMainDir(wxString dir) { m_mainDir = dir; }
	virtual uint GetFilesCount() const { return m_files.GetCount(); } 
	wxString GetFilePath(int index) const { return m_files.Item(index).path; }
	virtual void ClearAll() { m_files.Clear();m_res.Clear(); }
	virtual void AddFile(wxString file);
	virtual void AddResource(ResourceItem & ri);
	ArrayResources * GetResources() { return &m_res; }
	bool ShowDlg();
	virtual wxString FindId(int id);
	// ukladani by melo patrit do konfig file, ale vzhledem k vyuziti staci kdyz bude zatim jen tady
	bool SaveConf(const wxString path);

	virtual XHoeFile * GetFileResource(int index);

};

class SettingsDialog : public wxPropertySheetDialog, public ResourceConfigFile
{
	wxTextCtrl* m_becherdir;
    wxButton* m_browse;
    wxButton* m_add;
    wxButton* m_remove;
    wxButton* m_addres;
    wxButton* m_removeres;
    wxListBox* m_listfiles;
    wxListCtrl* m_listresources;
public:
	SettingsDialog(wxWindow* win, Resources * pr);
	wxPanel* CreateFilesPage(wxWindow* parent);
	wxPanel* CreateResourcesPage(wxWindow* parent);
	void OnBrowse(wxCommandEvent& event);
	void OnAdd(wxCommandEvent& event);
	void OnRemove(wxCommandEvent& event);
	void UpdateButtons();
	void OnUpdateButtons(wxCommandEvent& event);
	void OnAddRes(wxCommandEvent& event);
	void OnRemoveRes(wxCommandEvent& event);
	void UpdateButtonsRes();
	void OnUpdateButtonsRes(wxListEvent& event);
	void OnEditResource(wxListEvent& event);

	wxString GetMainDir() { return m_becherdir->GetValue(); }
	wxListBox* GetFileList() { return m_listfiles; }
	unsigned int GetCountResources() { return m_listresources->GetItemCount(); }
	const ResourceItem & GetResource(int index);
	void UpdateListItem(long item, ResourceItem * rs);
	virtual void AddFile(wxString file);
	virtual void AddResource(ResourceItem & ri);
	virtual void ClearAll();

	DECLARE_EVENT_TABLE()
};

///////////////////////////////////////////////////////

class ResourceDialog : public wxDialog
{
	wxTextCtrl* nameCtrl;
	wxTextCtrl* idCtrl;
	wxTextCtrl* cmdCtrl;
public:
	ResourceDialog(wxWindow* win);
	void OnOK(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	void Set(ResourceItem * rs);
	void Get(ResourceItem * rs);


	DECLARE_EVENT_TABLE()
};

}; // namespace HoeEditor

#endif // _BECHER_EDITOR_RESOURCES_H_



