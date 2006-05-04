
#include "StdAfx.h"
#include "hoe_figure2d.h"

BEGIN_HOEGAME

Hoe2DFigure::Hoe2DFigure()
{
	m_scene = NULL;
	m_hoe2d = NULL;
}

Hoe2DFigure::~Hoe2DFigure()
{
}

void Hoe2DFigure::Show(IHoeScene * scene)
{
	assert(scene);
	scene->Set2DCallback(this);
	m_scene = scene;
}

void Hoe2DFigure::Hide()
{
	if (m_scene)
	{
		m_scene->Set2DCallback(NULL);
	}
}

void Hoe2DFigure::_Paint(IHoe2D * e)
{
	m_hoe2d = e;

	OnPaint();

	//return 0;
}

void Hoe2DFigure::OnPaint()
{
}

END_HOEGAME


