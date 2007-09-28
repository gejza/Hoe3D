
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "texture_system.h"
#include "material_system.h"

MaterialSystem::MaterialSystem()
{
	SET_SHARED_PTR(material);
}

MaterialSystem::~MaterialSystem()
{
	UNSET_SHARED_PTR(material);
}

void HOEAPI MaterialSystem::SetMaterialDir(const char * dir)
{
	m_dir = dir;
}

HoeMaterial * MaterialSystem::GetMaterial(const char * name)
{
	HoeMaterial * mat = new HoeMaterial();
	char path[1024];
	if (m_dir)
	{
		m_dir.Export(path, sizeof(path));
		size_t l = m_dir.Length();
		if (path[l-1] != '/' && path[l-1] != '\\')
		{
			strcat(path,"/");
		}
	}
	else
	{
		path[0] = '\0';
	}
	strcat(path,name);
	strcat(path,".hml");
	Con_Print("Load material '%s' from '%s'",name,path);
	mat->LoadFromFile(path);
	return mat;
}

