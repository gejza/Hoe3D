
#include "StdAfx.h"
#include "../include/HoeGame/hoe_figure2d.h"
#include "../include/HoeGame/hoe_gui.h"
#include "../include/HoeGame/hoe_app.h"
#include "../include/HoeGame/hoe_structfile.h"

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
	errf.printf(T("%s(%d): error: %s"), fname, parser.GetLineNum(), (const tchar*)err);
	BaseConsole::Printf(T("%s"), (const tchar*)errf);
	// show error
	HoeApp::GetApp<HoeApp>()->ShowMsg(T("Script error"), errf);

	return false;
}



Gui::Base * Hoe2DFigure::CreateGUI(const char *type)
{
	Gui::Item * g = NULL;
#define IS(t) (HoeCore::string::cmp(type,t)==0)
	if (IS("static"))
		g = NewItem<StaticPicture>();
	else if (IS("colorrect"))
		g = NewItem<ColorRect>();
	else if (IS("button"))
		g = NewItem<Button>();
	else if (IS("buttonusr"))
		g = NewItem<ButtonUsr>();
	else if (IS("digicounter"))
		g = NewItem<DigiCounter>();
	else if (IS("infopanel"))
		g = NewItem<InfoPanel>();
	else if (IS("text"))
		g = NewItem<Font>();
	else
		return NULL;

	return g;
}

void Hoe2DFigure::Clear()
{
	m_list.Delete();
}

void Hoe2DFigure::Draw(IHoe2D *hoe2d)
{
	hoe2d->SetRect(800,600);
	for (ItemList::Iterator i(m_list);i;i++)
	{
		if (i->GetShow())
			i->Draw(hoe2d);
	}
	for (ItemList::Iterator i(m_list);i;i++)
	{
		if (i->GetShow())
			i->Draw2(hoe2d);
	}
}

Gui::Item * Hoe2DFigure::GetItem(const char * name, Gui::EType type)
{
	for (ItemList::Iterator i(m_list);i;i++)
	{
		if (type != Gui::ENone && i->GetType() != type)
			continue;
		const tchar * n = i->GetName();
		if (n && HoeCore::string::cmp(n,name)==0)
			return &(*i);
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
	for (ItemList::BackIterator i(m_list);i;i--)
	{
		if (i->GetType() == Gui::EButton)
			i.cast<Gui::Button>().Move(x,y, act);
	}
}

bool Hoe2DFigure::Click(const float x, const float y)
{
	for (ItemList::BackIterator i(m_list);i;i--)
	{
		if (i->GetType() == Gui::EButton
			&& i.cast<Gui::Button>().Click(x,y))
			return true;
	}
	return false;
}

END_HOEGAME


