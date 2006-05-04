
#include <he/app.h>
#include <he/editor.h>
//#include <vector>
//#include <string>
#include <hoe3d.h>
#include "modelview.h"

IMPLEMENT_APP(ModelViewApp)
const char * EditorName = "ModelView";

HoeEditor::BaseEditor * ModelViewApp::CreateEditor()
{
	HoeEditor::BaseEditor * e = new ModelView();
	e->Create("Model View");

	//m_split = new wxSplitterWindow(this,10, wxDefaultPosition, wxDefaultSize,0);
	//m_split->SetSashGravity(0.3);


	return e;
}

/*
ModelView * g_viewer = NULL;

ModelObject::ModelObject(const char * name, FileItem * file)
{
	m_name = name;
	m_file = file;
	m_valid = false;
}

void ModelObject::Select()
{
	if (!m_valid)
	{
		Reload();
	}
	this->Show(true);
}

void ModelObject::Unselect()
{
	this->Show(false);
}

void ModelObject::Reload()
{
	if (!m_valid)
	{
 		g_viewer->GetScene()->RegisterObject(this);
		m_valid = true;
	}
	std::string cmd = "model " + m_name;
	this->LoadModel(cmd.c_str());
}

///////////////////////////////////////

ModelView::ModelView()
{
	assert(g_viewer == NULL);
	g_viewer = this;
	m_act = NULL;
	m_scene = NULL;
	m_backcolor = 0xff909090;
}

const char * ModelView::GetIS()
{
	return "modelviewer";
}

#include "models_pack.xpm"
#include "model_icon.xpm"

bool ModelView::Init()
{
	m_fileimage = HoeExtern::ITreeControl::Get()->RegisterImage((const char **)(models_pack_xpm));
	m_modelimage = HoeExtern::ITreeControl::Get()->RegisterImage((const char **)(model_icon_xpm));

	HoeExtern::IEditor::Get()->AddEnginePage(this);
	return true;
}

const char * ModelView::GetExt(int i)
{
	assert(i==0);
	return "hm";
}

const char * ModelView::GetLongDesc(int i)
{
	assert(i==0);
	return "Hoe Model Pack file";
}

const char * ModelView::GetShortDesc(int i)
{
	assert(i==0);
	return "Hoe Model";
}


bool ModelView::OpenFile(const char * fpath)
{
	int handle = HoeExtern::ITreeControl::Get()->AddItem(-1,HoeUtils::GetFileName(fpath).c_str(),m_fileimage,0);
	IHoe3DEngine * eng = HoeExtern::IEditor::Get()->GetEngine();
	HOE_HANDLE h = eng->AddResource(fpath);
	IHoeResourceMgr * rm = reinterpret_cast<IHoeResourceMgr *>(eng->GetSystem(HOE_SYS_RESMGR));
	IHoeModelFile * mf = rm->GetModelFile(h);
	if (!mf)
		return false;

	FileItem * fi = new FileItem;
	fi->fname = fpath;
	fi->item = handle;
	m_files.push_back(fi);

	for (unsigned i=0;i < mf->GetNumModels();i++)
	{
		char buff[200];
		if (mf->GetModelName(i,buff,sizeof(buff)))
		{
			ModelObject * i = new ModelObject(buff,fi);
			HoeExtern::ITreeControl::Get()->AddItem(handle,buff,m_modelimage,this,i);
			m_models.push_back(i);
		}
	}
	
	
	return true;
}

int ModelView::Msg(EPluginMsg msg, int par1, void * par2)
{
	switch (msg)
	{
	case EPM_SelectItem:
		OnSelectItem(par1,par2);
		break;
	case EPM_UnselectItem:

		break;
	case EPM_ChangeProperty:
		OnChangeProperty((HoeExtern::IPropertyItem*)par2);
		break;
	case EPM_UnloadEngine:
		UnloadEngine();
		break;
	case EPM_ReloadEngine:
		ReloadEngine();
		break;
	case EPM_EngMouseWheel:
		m_view.MoveDistance(((float*)par2)[0] * 0.1f);
		break;
	case EPM_EngMouseMove: 
		if (par1 & MRight)
		{
			float * m = (float*)par2;
			m_view.MoveDistance(m[1] * 1.f);
		}
		else if (par1 & MLeft) 
		{
			float * m = (float*)par2;
			if (m[0])
				m_view.Rotate(m[0] * 0.01f);
			if (m[1])
				m_view.RotateArc( - m[1] * 0.01f);

		} break;
	case HoePlugin::EPM_EngKeyDown:
		if (par1 == 0x139)
			m_view.MoveDistance(12.f);
		else if (par1 == 0x138)
			m_view.MoveDistance(-12.f);
		break;
	};
	return 0;
}

void ModelView::OnSelectItem(int item, void * data)
{
	ModelObject * mo = reinterpret_cast<ModelObject *>(data);
	ActivateModel(mo);

	HoeExtern::IProperty * prop = HoeExtern::IProperty::Get();
	prop->Begin(this);
	prop->AppendCategory("Model Info");
	prop->AppendString( 0,"Name", mo->GetName().c_str(),PG_PROP_DISABLE, "Jmeno Modelu");
	prop->AppendCategory(__("Model View"));
	prop->AppendColor(2, __("Background"), m_backcolor);
	prop->End();

	HoeExtern::IEditor::Get()->GetEngine()->SetActiveScene(GetScene());
	HoeExtern::IEditor::Get()->ActivateEnginePage(this);
	UpdateBackground();
}

void ModelView::OnUnselectItem(int item, void * data)
{
	ModelObject * mo = reinterpret_cast<ModelObject *>(data);
	mo->Unselect();
}

void ModelView::OnChangeProperty(HoeExtern::IPropertyItem * item)
{
	switch (item->GetID())
	{
	//case 1:
	//	item->GetSize(&width,&height);
	//	break;
	case 2:
		m_backcolor = item->GetColor();
		UpdateBackground();
		break;
	}
}

void ModelView::UpdateBackground()
{
	IHoe3DEngine * eng = HoeExtern::IEditor::Get()->GetEngine();
	IHoeRef * r = (IHoeRef*)eng->GetSystem(HOE_SYS_REF);
	r->SetBackgroundColor(m_backcolor);
}

void ModelView::UnloadEngine()
{
	m_scene = NULL;
	for (unsigned int i=0; i < m_models.size();i++)
	{
		m_models[i]->Unvalidate();
	}
}

void ModelView::ReloadEngine()
{
	IHoe3DEngine * eng = HoeExtern::IEditor::Get()->GetEngine();

	for (unsigned int i=0; i < m_files.size();i++)
	{
		eng->AddResource(m_files[i]->fname.c_str());
	}
}

bool ModelView::Save(HoeExtern::IProjectSave * ps)
{
	for (int i=0;i < m_files.size();i++)
	{
		ps->AddCategory("file");
		ps->AddName("path");
		ps->SetDataPath(m_files[i]->fname.c_str());
	}
	return true;
}

bool ModelView::Load(HoeExtern::IProjectLoad * pl)
{
	char fn[2048];
	do {
		if (strcmp(pl->GetCategoryName(),"file") != 0 || strcmp(pl->GetName(),"path") != 0)
			return false;
		if (pl->GetDataPath(fn,sizeof(fn)-1))
			OpenFile(fn);
	} while (pl->NextCategory());
	return true;
}

IHoeScene * ModelView::GetScene() 
{ 
	if (m_scene == NULL)
	{
		m_scene = HoeExtern::IEditor::Get()->GetEngine()->CreateScene(HOETS_TERRAIN);
		m_view.Init(m_scene);
		m_view.Update();
		assert(m_scene);
	}
	return m_scene;
}


void ModelView::ActivateModel(ModelObject * m)
{
	if (m_act)
		m_act->Unselect();
	if (m)
		m->Select();
	m_act = m;
}

*/

///////////////////////////////////////////////////////
