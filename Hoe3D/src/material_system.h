
#ifndef _MATERIAL_SYSTEM_H_
#define _MATERIAL_SYSTEM_H_

#include "../include/hoeinterfaces.h"
#include "hoe_material.h"

class MaterialSystem : public IHoeMaterialSystem
{
	HoeCore::String m_dir;
public:
	MaterialSystem();
	~MaterialSystem();
	HoeMaterial * GetMaterial(const char * name);
	virtual void HOEAPI SetMaterialDir(const char * dir);
};

#endif // _MATERIAL_SYSTEM_H_

