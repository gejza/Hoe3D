
/**
   @file    figure.h
   @date    Aug 2006
   @version 1.0
   @brief   Soubor s definici tridy pro zobrazovani 2d.
*/

#include "StdAfx.h"
#include "figure.h"
IHoePicture * hud;
bool FigureEdit::Create(IHoe3DEngine * eng)
{
	m_scene = eng->CreateScene(HOETS_2D);
	eng->SetActiveScene(m_scene);
	m_scene->Set2DCallback(this);
	hud = (IHoePicture * )eng->Create("picture hud");
	return true;
}

void HOEAPI FigureEdit::_Paint(IHoe2D * h2d)
{
	h2d->SetRect(100,100);
	h2d->PaintRect(10,50,10,50,0x80ffff00,true);
	h2d->BltFast(0,100,0,100,hud);
}

