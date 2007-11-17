
#include "StdAfx.h"
#include "../include/hoe_figure2d.h"
#include "../include/hoe_gui.h"
#include "../include/hoe_app.h"
#include "../include/hoe_structfile.h"

BEGIN_HOEGAME

using namespace Gui;

Hoe2DFigureBase::Hoe2DFigureBase()
{
}

Hoe2DFigureBase::~Hoe2DFigureBase()
{
}

bool Hoe2DFigureBase::Load(const tchar * fname)
{
	ObjectFileParser parser;
	HoeCore::String_s<1024> err;
	if (!parser.Open(fname))
		return false;
	
	while (parser.ParseObject())
	{
		HoeGame::Gui::Base * gui = CreateGUI(parser.GetTypeObject());		
		if (!gui)
		{
			err.printf("Object type %s is unknown.", parser.GetTypeObject());
			goto loaderror;
		}
		const Property * p;
		while (p = parser.GetNextProperty())
		{
			gui->Set(p->GetName(), p->GetValue());
		}
		if (parser.GetLastError())
		{
			err = parser.GetLastError();
			goto loaderror;
		}
	}
	if (parser.GetLastError())
	{
		err = parser.GetLastError();
		goto loaderror;
	}

	return true;
loaderror:
	HoeCore::String_s<1024> errf;
	errf.printf("%s(%d): error: %s", fname, parser.GetLineNum(), (const char*)err);
	BaseConsole::Printf("%s", (const char*)errf);
	// show error
	HoeApp::GetApp<HoeApp>()->ShowMsg("Script error", errf);

	return false;
}

Gui::Base * Hoe2DFigure::CreateGUI(const char *type)
{
	Gui::Item * g = NULL;
#define IS(t) (strcmp(type,t)==0)
	if (IS("static"))
		g = new StaticPicture;
	else if (IS("colorrect"))
		g = new ColorRect;
	else if (IS("button"))
		g = new Button;
	else if (IS("buttonusr"))
		g = new ButtonUsr;
	else if (IS("digicounter"))
		g = new DigiCounter;
	else if (IS("infopanel"))
		g = new InfoPanel;
	else if (IS("text"))
		g = new Font;
	else
		return NULL;

	m_list.Add(g);
	return g;
}

void Hoe2DFigure::Clear()
{
	for (uint i=0;i < m_list.Count();i++)
	{
		Gui::Item * item = m_list.Get(i);
		delete item;
	}
	m_list.SetCount(0);
}

void Hoe2DFigure::Draw(IHoe2D *hoe2d)
{
	hoe2d->SetRect(800,600);
	for (uint i=0;i<m_list.Count();i++)
	{
		if (m_list.Get(i)->GetShow())
			m_list.Get(i)->Draw(hoe2d);
	}
	for (uint i=0;i<m_list.Count();i++)
	{
		if (m_list.Get(i)->GetShow())
			m_list.Get(i)->Draw2(hoe2d);
	}
}

Gui::Item * Hoe2DFigure::GetItem(const char * name, Gui::EType type)
{
	for (uint i=0;i<m_list.Count();i++)
	{
		if (type != Gui::ENone && m_list.Get(i)->GetType() != type)
			continue;
		const tchar * n = m_list.Get(i)->GetName();
		if (n && strcmp(n,name)==0)
			return m_list.Get(i);
	}
	return NULL;
}

Gui::Item * Hoe2DFigure::ReqItem(const char * name, Gui::EType type)
{
	Gui::Item * i = GetItem(name, type);
	if (!i)
	{
		HoeCore::String_s<1024> err;
		err.printf(err,"Missing 2D object '%s'", name);
		HoeApp::GetApp<HoeApp>()->ShowMsg(T("2DFigure Error"),err);
		exit(1);
	}
	return i;
}

void Hoe2DFigure::Move(const float x, const float y, bool & act)
{
	for (int i=m_list.Count()-1;i>=0;i--)
	{
		if (m_list.Get(i)->GetType() == Gui::EButton)
			dynamic_cast<Gui::Button*>(m_list.Get(i))->Move(x,y, act);
	}
}

bool Hoe2DFigure::Click(const float x, const float y)
{
	for (int i=m_list.Count()-1;i>=0;i--)
	{
		if (m_list.Get(i)->GetType() == Gui::EButton
			&& dynamic_cast<Gui::Button*>(m_list.Get(i))->Click(x,y))
			return true;
	}
	return false;
}

END_HOEGAME


