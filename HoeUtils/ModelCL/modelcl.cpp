
#ifdef _WIN32
#include <windows.h>
#endif
#include <vector>
#include <assert.h>
#include "../../include/hoe3d.h"
#include "../../utils/sdk/hoePlugin.h"
#include "modelcl.h"
#include "models_pack.xpm"

using namespace HoePlugin;

static int g_fileimage = -1;

ModelScript::ModelScript()
{
}

bool ModelScript::OpenFileName(std::string fname)
{
	this->item = HoeExtern::ITreeControl::Get()->AddItem(-1,HoeUtils::GetFileName(fname).c_str(),g_fileimage,this);

	m_fname = fname;

	m_buffer = HoeExtern::IEditor::Get()->CreateCodeBuffer(this);
	m_buffer->Load(fname.c_str());
	return true;
}

int ModelScript::Msg(EPluginMsg msg, int par1, void * par2)
{
	HoeExtern::IProperty * prop = HoeExtern::IProperty::Get();
	switch (msg)
	{
	case EPM_SelectItem:
		HoeExtern::IEditor::Get()->ActivateCodeEditorPage();
		m_buffer->Activate();
		prop->Begin(this);
		prop->AppendCategory(__("Output"));
		prop->AppendString( 1, "file", "", 0, "output file");
		prop->End();
		break;
	}
	return 0;

}


/////////////////////////////////////////////////////////////////////////////

ModelCL::ModelCL()
{
}

const char * ModelCL::GetIS()
{
	return "modelcompiler";
}

bool ModelCL::Init()
{
	HoeExtern::ITreeControl::Get();
	//g_fileimage = HoeExtern::ITreeControl::Get()->RegisterImage((const char **)(texture_pack_xpm));
	//m_teximage = HoeExtern::ITreeControl::Get()->RegisterImage((const char **)(exefile_xpm));
	//HoeExtern::IEditor::Get()->AddEnginePage(this);

	return true;
}

const char * ModelCL::GetExt(int i)
{
	assert(i==0);
	return "hms";
}

const char * ModelCL::GetLongDesc(int i)
{
	assert(i==0);
	return __("Hoe Model Script file");
}

const char * ModelCL::GetShortDesc(int i)
{
	assert(i==0);
	return __("Hoe Model Script");
}

bool ModelCL::OpenFile(const char * fpath)
{
	ModelScript * ts = new ModelScript();
	if (ts->OpenFileName(fpath))
	{
        m_files.push_back(ts);
		return true;
	}
	else
	{
		delete ts;
		return false;
	}
}

int ModelCL::Msg(EPluginMsg msg, int par1, void * par2)
{
	switch (msg)
	{
	case EPM_NewDlgInit:
		reinterpret_cast<HoeExtern::INewDialog*>(par2)->AddType("Model script",23,models_pack_xpm);
		break;
	case EPM_NewDlgCheck:
		if (par1 == 23)
		{
			char path[1024];
			strncpy(path, reinterpret_cast<HoeExtern::INewDialog*>(par2)->GetFullPath(), 1000);
			HoeUtils::change_ext(path,"hms");
			if (HoeUtils::is_file(path))
				return 1;
			else
				return 0;
		}
		break;
	case EPM_NewFile:
		if (par1 == 23)
		{
			char path[1024];
			strncpy(path, reinterpret_cast<HoeExtern::INewDialog*>(par2)->GetFullPath(), 1000);
			HoeUtils::change_ext(path,"hms");
			NewPack(path);
		}
		break;
	/*case EPM_ChangeProperty:
		OnChangeProperty((HoeExtern::IPropertyItem*)par2);
		break;
	case EPM_UnloadEngine:
		UnloadEngine();
		break;
	case EPM_ReloadEngine:
		ReloadEngine();
		break;
	case EPM_EngMouseMove:
		aa = true;
		break;
	case EPM_EngMouseLeave:
		aa = false;
		break;*/
	};
	return 0;
}

/*void TextureView::OnSelectItem(int item, void * data)
{
	TextureItem * ti = reinterpret_cast<TextureItem *>(data);
	ShowTexture(ti);

	HoeExtern::IProperty * prop = HoeExtern::IProperty::Get();
	prop->Begin(this);
	prop->AppendCategory("Texture Info");
	prop->AppendString( 0,"Name", ti->name.c_str(),PG_PROP_DISABLE, "Jmeno textury");
	prop->AppendString( 0,"Format", "JPEG", PG_PROP_DISABLE, "Format textury");
	prop->AppendCategory("View");
	prop->AppendSize( 1,"Size",width,height,(width == 100 && height == 100) ? 0:PG_PROP_MODIFIED,"velikost zobrazeni textury");
	prop->AppendColor(2, "Background", back);
	prop->End();
	UpdateBackground();
}

void TextureView::OnChangeProperty(HoeExtern::IPropertyItem * item)
{
	switch (item->GetID())
	{
	case 1:
		item->GetSize(&width,&height);
		break;
	case 2:
		back = item->GetColor();
		UpdateBackground();
		break;
	}
}*/

bool ModelCL::Save(HoeExtern::IProjectSave * ps)
{
	for (int i=0;i < m_files.size();i++)
	{
		ps->AddCategory("file");
		ps->AddName("path");
		ps->SetDataString(m_files[i]->GetFileName().c_str());
	}
	return true;
}

bool ModelCL::Load(HoeExtern::IProjectLoad * pl)
{
	do {
		if (strcmp(pl->GetCategoryName(),"file") != 0 || strcmp(pl->GetName(),"path") != 0)
			return false;
		OpenFile(pl->GetDataString());

	} while (pl->NextCategory());
	return true;
}

void ModelCL::NewPack(const char * fpath)
{
	// vytvoreni souboru
	{
		HoeUtils::File f;
		f.Open(fpath,HoeUtils::File::mWrite);
		const char str[] = "// texture pack script\n";
		f.Write(str,sizeof(str)-1);
	}
	OpenFile(fpath);
}

///////////////////////////////////////////////////////

#ifdef _WIN32

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}

#endif // _WIN32

static HoePlugin::PluginBase * CreateClass(int)
{
	return new ModelCL();				
}

static const HoePlugin::TPluginDesc plugin = 
{
	VERSION_HOEEDITOR,
	"gejza", // const char * author;
	"Model Compiler",      // const char * description;
	1,       // int numClasses;
	CreateClass,     // CREATEFUNC createfunc;
	NULL
};

PLUGIN_EXPORT_FUNC
{
	return &plugin;
}


