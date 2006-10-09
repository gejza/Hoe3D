
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "texture_system.h"
#include "material_system.h"

MaterialSystem::MaterialSystem()
{
	SET_SHARED_PTR(material);
	m_dir = 0;
}

MaterialSystem::~MaterialSystem()
{
	UNSET_SHARED_PTR(material);
	if (m_dir) free((void*)m_dir);
}

void HOEAPI MaterialSystem::SetMaterialDir(const char * dir)
{
	m_dir = strdup(dir);
}

HoeMaterial * MaterialSystem::GetMaterial(const char * name)
{
	HoeMaterial * mat = new HoeMaterial();
	char path[1024];
	if (m_dir)
	{
		strcpy(path,m_dir);
		size_t l = strlen(m_dir);
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

