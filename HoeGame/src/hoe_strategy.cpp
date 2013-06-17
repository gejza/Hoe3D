
#include "StdAfx.h"
#include "../include/HoeGame/hoe_strategy.h"
#include "../include/HoeGame/hoe_stratmap.h"

namespace HoeGame {
namespace Strategy {

StgObject::StgObject(IHoeScene * scn) : BaseObject(scn)
{
}

StgObject::~StgObject()
{
}


SelectObjContainer::SelectObjContainer()
{ 
	m_object = NULL;
}

bool SelectObjContainer::IsSelectMode() 
{ 
	return m_object != NULL;
}

void SelectObjContainer::SetPosition(float _x, float _y)
{
	x = _x; y = _y;
	if (m_object)
		m_object->SetPosition(x,0,y);
}

void SelectObjContainer::Show()
{
	if (m_object) m_object->Show(true);
}
void SelectObjContainer::Hide()
{
	if (m_object) m_object->Show(false);
}

StgObject * SelectObjContainer::Build()
{
	StgObject * o = m_object;
	/*if (m_object)
        m_level->AddObject(x,y,m_object);*/
	m_object = NULL;
	return o;
}

void SelectObjContainer::Cancel()
{
	if (m_object)
		delete m_object;
	m_object = NULL;
}

void SelectObjContainer::Set(StgObject * obj)
{
	m_object = obj;
}

};
} // namespace HoeGame

