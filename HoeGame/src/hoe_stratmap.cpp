
#include "StdAfx.h"
#include "../include/HoeGame/hoe_stratmap.h"

namespace HoeGame {
namespace Strategy {

////////////////////

Map::Map()
{
	m_scene = NULL;
}

bool Map::Create(IHoeScene * scn)
{
	m_scene = scn;
	m_view.Init(m_scene);
	m_view.SetTargetPosition(0,0);
	static float angle = 0;
	angle += 1.f;
	m_view.SetAngle(angle);
	m_view.SetDistance(120);

	return true;
}



/*void Level::SBEnter()
{
	if (m_sbobj)
		m_sbobj->Show(true);
}

void Level::SBLeave()
{
	if (m_sbobj)
		m_sbobj->Show(false);
}*/

void Map::OnSelectObject(StgObject * m_sbobj)
{
}

void Map::AddObject(float x, float y, StgObject * s)
{
	s->SetPosition(x,0,y);
	s->Show(true);
	OnAddObject(s);
}

void Map::OnAddObject(StgObject * obj)
{

}

};
} // namespace HoeGame

