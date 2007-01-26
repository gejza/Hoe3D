
#include "StdAfx.h"
#include "../include/he/app.h"
#include "../include/he/editor.h"
//#include "becher.h"
#include <wx/arrimpl.cpp>
//#include "editor_tools.h"
//#include "plugin_editlevel.h"
//#include "id.h"
#include "../include/he/resources.h"
#include "../include/he/engview.h"
#include "../../HoeGame/include/hoe_engine.h"

namespace HoeEditor {

enum {
	IDB_BROWSE = 1,
	IDB_ADD,
	IDB_REMOVE,
	IDL_FILES,
	IDL_RESOURCES,
	IDB_ADDRES,
	IDB_REMOVERES,
	IDB_RESOK,
	IDB_RESCANCEL,
};

WX_DEFINE_OBJARRAY(ArrayResources);
WX_DEFINE_OBJARRAY(ArrayFiles);

wxString Resources::FindId(int id)
{
	return wxT("");
}

IHoeResource * Resources::MissingResource(int id)
{
	wxString findid = FindId(id);
	if (findid == "")
	{
		findid.Printf("%d",id);
	}
reply:
	for (uint i=0;i < m_res.GetCount();i++)
	{
		if(m_res[i].id == findid)
		{
			IHoeResource * res = NULL;
			if (HoeGame::GetHoeEngine())
				res = (IHoeResource*)HoeGame::GetHoeEngine()->Create(m_res[i].cmd.c_str());
			if (res)
				return res;
			else
			{
				wxString buf;
				buf.Printf(_("Failed command %s for resource %s."),m_res[i].cmd.c_str(),findid.c_str());
				wxMessageBox(buf);
				if (ShowDlg())
					goto reply;
				return NULL;
			}
		}
	}

	wxString buf;
	buf.Printf(_("Missing resource id %s"),findid.c_str());
	wxMessageBox(buf);

	if (ShowDlg())
		goto reply;

	return NULL;
}

bool Resources::ReqResource(const char * name)
{
	wxString buf;
	buf.Printf(_("Missing resource %s"), name);
	wxMessageBox(buf);

	return ShowDlg();
}

XHoeFile * Resources::GetFileResource(int index)
{
	if (m_files.Item(index).file == NULL)
	{
		m_files.Item(index).file = 
			Open(HoeUtils::fullpath(m_mainDir.c_str(), GetFilePath(index).c_str()).c_str());
	}
	return m_files.Item(index).file;
}

void Resources::AddFile(wxString file)
{
	FileItem fi;
	fi.file = NULL;
	fi.path = file;
	m_files.Add(fi);
}


bool Resources::ShowDlg()
{	
	SettingsDialog dlg(App::Get()->GetEditor(),this);
	if (dlg.ShowModal() == wxID_OK)
	{
		SetMainDir(dlg.GetMainDir());
		ClearAll();
		for (int i=0;i < dlg.GetFileList()->GetCount();i++)
		{
			AddFile(dlg.GetFileList()->GetString(i));
		}

		// resources
		for (size_t j=0;j < dlg.GetCountResources();j++)
		{
			GetResources()->Add(dlg.GetResource(j));
		}
		
		if (!this->m_mainDir.IsEmpty())
		{
			std::string predpath = HoeUtils::fullpath(m_mainDir.c_str(),"files.txt");
			this->SaveConf(predpath.c_str());
		}

		// ulozit do konfigu
		wxConfigBase *pConfig = wxConfigBase::Get();
		if ( pConfig )
			pConfig->Write(_T("/ResourceDir"), m_mainDir);

		return true;
	}

	return false;
}

bool GetFileLine(FILE * f, char * buff, int sizebuffer)
{
	if (fgets(buff,sizebuffer-1,f) == NULL)
		return false;
	// odebrani debilnich windows znaku na konci
	char * pb = buff;
	while (*pb) pb++;
	while (--pb >= buff && (*pb == 10 || *pb == 13 || *pb == ' ' || *pb == '\t')) *pb = 0;
	return true;
}

void Resources::AddResource(ResourceItem & ri)
{
	this->m_res.Add(ri);
}

bool ResourceConfigFile::LoadConf(const wxString path)
{
	char buff[1024];
	FILE * f = fopen(path.c_str(),"rt");
	if (!f)
		return false;
	//if (!fgets(buff,sizeof(buff)-1,f) || strcmp(buff,"#files")
	while (true)
	{
		if (!GetFileLine(f,buff,sizeof(buff)))
			goto fileerr;
		if (strcmp(buff,"#files") == 0)
			break;
	}
	ClearAll();
	while (true)
	{
		if (!GetFileLine(f,buff,sizeof(buff)))
			goto fileerr;
		if (strcmp(buff,"#resources") == 0)
			break;

		// nacteni souboru
		AddFile(buff);
	}
	while (true)
	{
		if (!GetFileLine(f,buff,sizeof(buff)))
			goto fileerr;
		if (strcmp(buff,"#end") == 0)
			break;
		
		// resource
		ResourceItem ri;
		const char * p = buff;
		// scan id
		while (*p == ' ' || *p == '\t') p++;
		while (*p != '=' && *p != ' ' && *p != '\t' && *p != 0)
		{
			ri.id.Append(*p);
			p++;
		}
		while (*p == ' ' || *p == '\t') p++;
		// scan cmd
		if (*p != '=')
			goto fileerr;
		p++;

		while (*p != '\"' && *p != 0) p++;
		if (*p != '\"')
			goto fileerr;
		p++;
		while (*p != '\"' && *p != 0)
		{
			ri.cmd.Append(*p);
			p++;
		}
		if (*p != '\"')
			goto fileerr;
		p++;
		while (*p == ' ' || *p == '\t') p++;
		// scan name
		if (*p != ';' && *p != 0)
			goto fileerr;

		if (*p == ';')
		{
			p++;
			while (*p == ' ' || *p == '\t') p++;
			while (*p != 0)
			{
				ri.name.Append(*p++);
			}
		}

		// add
		AddResource(ri);
	}
	fclose(f);
	return true;
fileerr:
	fclose(f);
	return false;
}

bool Resources::SaveConf(const wxString path)
{
	FILE * f = fopen(path.c_str(),"wt");
	if (!f)
		return false;

	fprintf(f, "#files\n");
	// print files
	for (uint i=0;i < GetFilesCount();i++)
	{
		fprintf(f,"%s\n",this->GetFilePath(i).c_str());
	}
	fprintf(f, "#resources\n");
	// print resouces
	for (uint i=0;i < this->GetResources()->GetCount();i++)
	{
		ResourceItem & ri = this->GetResources()->Item(i);
		fprintf(f,"%s=\"%s\"", ri.id.c_str(), ri.cmd.c_str());
		if (!ri.id.IsEmpty())
			fprintf(f, " ; %s", ri.name.c_str());
		fprintf(f, "\n");
	}
	fprintf(f, "#end\n");
	fclose(f);
	return true;
}

////////////////////////////
BEGIN_EVENT_TABLE(SettingsDialog, wxPropertySheetDialog)
	EVT_BUTTON(IDB_BROWSE, SettingsDialog::OnBrowse)
	EVT_BUTTON(IDB_ADD, SettingsDialog::OnAdd)
	EVT_BUTTON(IDB_REMOVE, SettingsDialog::OnRemove)
	EVT_BUTTON(IDB_ADDRES, SettingsDialog::OnAddRes)
	EVT_BUTTON(IDB_REMOVERES, SettingsDialog::OnRemoveRes)
	EVT_LISTBOX(IDL_FILES, SettingsDialog::OnUpdateButtons)
	EVT_LIST_ITEM_SELECTED(IDL_RESOURCES, SettingsDialog::OnUpdateButtonsRes)
	EVT_LIST_ITEM_DESELECTED(IDL_RESOURCES, SettingsDialog::OnUpdateButtonsRes)
	EVT_LIST_ITEM_ACTIVATED(IDL_RESOURCES, SettingsDialog::OnEditResource)
END_EVENT_TABLE()



SettingsDialog::SettingsDialog(wxWindow* win, Resources * pr)
{
	SetExtraStyle(/*wxDIALOG_EX_CONTEXTHELP|*/wxWS_EX_VALIDATE_RECURSIVELY);

    Create(win, -1, _("Becher Preferences"), wxDefaultPosition, wxSize(400,400));
    CreateButtons(wxOK|wxCANCEL);

    wxBookCtrlBase* notebook = GetBookCtrl();

    wxPanel* f = CreateFilesPage(notebook);
    wxPanel* r = CreateResourcesPage(notebook);

    notebook->AddPage(f, _("Files"));
    notebook->AddPage(r, _("Resources"));

	// fill data
	if (pr)
	{	
		unsigned int i;
		m_becherdir->SetValue(pr->GetMainDir());
		for (i=0;i < pr->GetFilesCount();i++)
		{
            m_listfiles->InsertItems(1, &pr->GetFilePath(i), INT_MAX);
		}

		for (i=0;i < pr->GetResources()->GetCount();i++)
		{
			ResourceItem * item = new ResourceItem;
			*item = pr->GetResources()->Item(i);;
			UpdateListItem(m_listresources->InsertItem(i, item->name),item);

		}
	}

    LayoutDialog(); 

	UpdateButtons();
	UpdateButtonsRes();
}

wxPanel* SettingsDialog::CreateFilesPage(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);
    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );

	//// hlavni cesta
	wxBoxSizer * siz = new wxBoxSizer(wxHORIZONTAL);
	siz->Add(new wxStaticText(panel,wxID_ANY,_("Location:"),wxDefaultPosition,wxSize(60,21)),
		wxSizerFlags().Align(wxALIGN_LEFT).Border(wxALL, 3));

	m_becherdir = new wxTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(300,21));

	siz->Add(m_becherdir,wxSizerFlags().Align(wxALIGN_CENTER_HORIZONTAL).Border(wxALL, 3));
	siz->Add(m_browse = new wxButton(panel,IDB_BROWSE,_("Browse..."),wxDefaultPosition,wxSize(75,25)),
		wxSizerFlags().Align(wxALIGN_RIGHT).Border(wxALL, 3));
	topSizer->Add(siz, 0, wxGROW|wxALL, 0);

    //// buttons
	wxBoxSizer * butt = new wxBoxSizer(wxHORIZONTAL);
	m_add = new wxButton(panel, IDB_ADD, _("Add.."));
	butt->Add(m_add,0, wxGROW|wxALL,0);
    m_remove = new wxButton(panel, IDB_REMOVE, _("Remove"));
	butt->Add(m_remove,0, wxGROW|wxALL,0);

	topSizer->Add(butt, 0, wxGROW|wxALL|wxALIGN_RIGHT, 0);

	//// list

    m_listfiles = new wxListBox( panel, IDL_FILES, wxDefaultPosition, wxSize(400,250), 0, NULL, wxLB_EXTENDED);
   	topSizer->Add(m_listfiles, 0, wxALIGN_BOTTOM|wxALL|wxEXPAND, 3);

    panel->SetSizer(topSizer);
	topSizer->SetSizeHints( panel );
    //topSizer->Fit(panel);
    
    return panel;
}

wxPanel* SettingsDialog::CreateResourcesPage(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );

    //// buttons
	wxBoxSizer * butt = new wxBoxSizer(wxHORIZONTAL);
	m_addres = new wxButton(panel, IDB_ADDRES, _("Add.."));
	butt->Add(m_addres,0, wxGROW|wxALL,0);
    m_removeres = new wxButton(panel, IDB_REMOVERES, _("Remove"));
	butt->Add(m_removeres,0, wxGROW|wxALL,0);
	topSizer->Add(butt, 0, wxGROW|wxALL, 3);

	m_listresources = new wxListCtrl(panel, IDL_RESOURCES, wxDefaultPosition, wxSize(400,260), wxLC_REPORT);
    wxListItem itemCol;
    itemCol.SetText(_("Name"));
    m_listresources->InsertColumn(0, itemCol);

    itemCol.SetText(_("ID"));
    m_listresources->InsertColumn(1, itemCol);

    itemCol.SetText(_("Command"));
    m_listresources->InsertColumn(2, itemCol);
    m_listresources->SetColumnWidth( 0, 150 );
    m_listresources->SetColumnWidth( 1, 100 );
    m_listresources->SetColumnWidth( 2, 200 );

	topSizer->Add(m_listresources, 0, wxGROW|wxLEFT|wxRIGHT, 3);

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);
    
    return panel;
}

void SettingsDialog::OnBrowse(wxCommandEvent& event)
{
    wxDirDialog dialog(this, _("Choose diRECTory"), m_becherdir->GetValue(),wxDD_DEFAULT_STYLE);

    if (dialog.ShowModal() == wxID_OK)
    {
		m_becherdir->SetValue(dialog.GetPath());
		// zjistovani zda obahuje script
		std::string predpath = HoeUtils::fullpath(dialog.GetPath().c_str(),"files.txt");
		if (HoeUtils::is_file(predpath.c_str()))
		{
			this->LoadConf(predpath.c_str());
		}
    }

	//m_tooltip.ShowHelp(m_becherdir);
}

bool Resources::LoadMainDir(const wxString dir)
{
	SetMainDir(dir);
	// zjistovani zda obahuje script
	std::string predpath = HoeUtils::fullpath(dir.c_str(),"files.txt");
	if (HoeUtils::is_file(predpath.c_str()))
	{
		return this->LoadConf(predpath.c_str());
	}
	return false;
}

void SettingsDialog::OnAdd(wxCommandEvent& event)
{
	wxString files = _("Hoe Resource Files(*.hx;*.hm)|*.hx;*.hm|All files(*.*)|*.*");
	wxFileDialog dialog(this,_("Add file..."),_T(""), _T(""), files, wxOPEN | wxFILE_MUST_EXIST | wxMULTIPLE);
	if (dialog.ShowModal() == wxID_OK)
	{
		wxArrayString paths;
		dialog.GetPaths(paths);
		for (unsigned int i=0;i < paths.Count();i++)
		{
			AddFile(HoeUtils::relativepath(m_becherdir->GetValue().c_str(),paths[i].c_str()).c_str());
		}

		UpdateButtons();
	}
}

void SettingsDialog::AddFile(wxString file)
{
	m_listfiles->InsertItems(1, &file, INT_MAX);
}

void SettingsDialog::OnRemove(wxCommandEvent& event)
{
	int i = 0;
	while (i < m_listfiles->GetCount())
	{
		if (m_listfiles->IsSelected(i))
			m_listfiles->Delete(i);
		else
			i++;
	}
	
	UpdateButtons();
}

void SettingsDialog::UpdateButtons()
{
	for (int i=0;i < m_listfiles->GetCount();i++)
	{
		if (m_listfiles->IsSelected(i))
		{
			m_remove->Enable(true);
			return;
		}
	}
	m_remove->Enable(false);
}

void SettingsDialog::OnUpdateButtons(wxCommandEvent& event)
{
	UpdateButtons();
}

void SettingsDialog::UpdateListItem(long item, ResourceItem * ri)
{
		m_listresources->SetItemData(item,(long)ri);
		m_listresources->SetItem(item,0,ri->name);
		m_listresources->SetItem(item,1,ri->id);
		m_listresources->SetItem(item,2,ri->cmd);
}

void SettingsDialog::OnAddRes(wxCommandEvent& event)
{
	ResourceDialog rd(this);
	if (rd.ShowModal())
	{
		ResourceItem * ri = new ResourceItem;
		rd.Get(ri);
		AddResource(*ri);
	}
}

void SettingsDialog::AddResource(ResourceItem & ri)
{
	ResourceItem *i = new ResourceItem(ri);
	UpdateListItem(m_listresources->InsertItem(m_listresources->GetItemCount(),ri.name),i);
}

void SettingsDialog::ClearAll()
{
	m_listfiles->Clear();
	m_listresources->DeleteAllItems();
}

void SettingsDialog::OnEditResource(wxListEvent& event)
{
	ResourceDialog rd(this);
	ResourceItem * ri = reinterpret_cast<ResourceItem *>(event.GetData());
	rd.Set(ri);
	if (rd.ShowModal())
	{
		rd.Get(ri);
		UpdateListItem(event.GetIndex(),ri);
	}
}

void SettingsDialog::OnRemoveRes(wxCommandEvent& event)
{
	int i;
	while ((i = m_listresources->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1)
	{
		m_listresources->DeleteItem(i);
	}
	
	UpdateButtons();
}

void SettingsDialog::UpdateButtonsRes()
{
	m_removeres->Enable(m_listresources->GetSelectedItemCount() > 0);
}

void SettingsDialog::OnUpdateButtonsRes(wxListEvent& event)
{
	UpdateButtonsRes();
}

const ResourceItem & SettingsDialog::GetResource(int index)
{
	return *(reinterpret_cast<ResourceItem*>(m_listresources->GetItemData(index)));
}

//////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(ResourceDialog, wxDialog)
    EVT_BUTTON(IDB_RESOK,ResourceDialog::OnOK)
    EVT_BUTTON(IDB_RESCANCEL,ResourceDialog::OnCancel)
END_EVENT_TABLE()

ResourceDialog::ResourceDialog(wxWindow* win)
{
	Create(win,-1,_("Edit resource"));

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );

	// name
    wxStaticBox* staticBox1 = new wxStaticBox(this, -1, _("Name:"));
    wxBoxSizer* itemSizer1 = new wxStaticBoxSizer( staticBox1, wxHORIZONTAL );
    nameCtrl = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition,
        wxSize(100, -1));
    itemSizer1->Add(nameCtrl, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    topSizer->Add(itemSizer1, 0, wxGROW|wxLEFT|wxRIGHT, 5);
	topSizer->AddSpacer(5);

	// id
    wxStaticBox* staticBox2 = new wxStaticBox(this, -1, _("ID:"));
    wxBoxSizer* itemSizer2 = new wxStaticBoxSizer( staticBox2, wxHORIZONTAL );
    idCtrl = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition,
        wxSize(100, -1));
    itemSizer2->Add(idCtrl, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    topSizer->Add(itemSizer2, 0, wxGROW|wxLEFT|wxRIGHT, 5);
	topSizer->AddSpacer(5);

	// command
    wxStaticBox* staticBox3 = new wxStaticBox(this, -1, _("Command:"));
    wxBoxSizer* itemSizer3 = new wxStaticBoxSizer( staticBox3, wxHORIZONTAL );
    cmdCtrl = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition,
        wxSize(250, -1));
    itemSizer3->Add(cmdCtrl, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    topSizer->Add(itemSizer3, 0, wxGROW|wxLEFT|wxRIGHT, 5);
	topSizer->AddSpacer(5);

	topSizer->Add(
		new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxSize(3,3), wxHORIZONTAL),
				wxSizerFlags().Expand());

	wxBoxSizer *button_box = new wxBoxSizer( wxHORIZONTAL );
	wxButton * okbutt = new wxButton( this, IDB_RESOK, _("OK") );
	okbutt->SetDefault();
	button_box->Add(okbutt,wxSizerFlags().Border(wxALL, 7));
	button_box->Add(
		new wxButton( this, IDB_RESCANCEL, _("Cancel") ),
		wxSizerFlags().Border(wxALL, 7));

	topSizer->Add(button_box, wxSizerFlags().Center());

    this->SetSizer(topSizer);
    topSizer->Fit(this);
}

void ResourceDialog::OnOK(wxCommandEvent& event)
{
	EndModal(1);
}

void ResourceDialog::OnCancel(wxCommandEvent& event)
{
	EndModal(0);
}

void ResourceDialog::Set(ResourceItem * rs)
{
	nameCtrl->SetValue(rs->name);
	idCtrl->SetValue(rs->id);
	cmdCtrl->SetValue(rs->cmd);
}

void ResourceDialog::Get(ResourceItem * rs)
{
	rs->name = nameCtrl->GetValue();
	rs->id = idCtrl->GetValue();
	rs->cmd = cmdCtrl->GetValue();
}

}; // namespace HoeEditor

//








