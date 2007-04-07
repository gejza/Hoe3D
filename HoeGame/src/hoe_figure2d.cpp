
#include "StdAfx.h"
#include "../include/hoe_figure2d.h"
#include "../include/hoe_gui.h"
#include "../include/hoe_app.h"
#include <locale.h>

void yyfigurerestart ( FILE *input_file );
int yyfigurelex();
extern char *yyfiguretext;
extern int yyfigureleng;
extern int yyfigurelineno;

BEGIN_HOEGAME

using namespace Gui;

Hoe2DFigureBase::Hoe2DFigureBase()
{
}

Hoe2DFigureBase::~Hoe2DFigureBase()
{
}

bool Hoe2DFigureBase::Load(const char * fname)
{
	setlocale(LC_NUMERIC, "C");

	FILE * f = fopen(fname, "rt");
	if (!f)
		return false;

	yyfigurerestart(f);

	int type = 0;
	char name[256];
	try {
	while ((type=yyfigurelex())==257)
	{
		// na zacatku musi byt jmeno vytvarene
		HoeGame::Gui::Base * gui = CreateGUI(yyfiguretext);		
		if (!gui)
			throw "syntax error, name requied";
		
		if ((type=yyfigurelex())!=258) 
		{
			throw "syntax error, `{' requied";
		}
		while ((type=yyfigurelex())!=260)
		{
			// read params
			if (type != 257)
				throw "syntax error, name requied";
			strncpy(name, yyfiguretext, 255);
			if (yyfigurelex()!=259 || yyfiguretext[0] != '=')
				throw "syntax error, `=' requied";
			// preskocit '=' a bile znaky
			char * p = yyfiguretext+1;
			char * e = yyfiguretext+yyfigureleng;
			while (*p==' ' || *p=='\t') p++;
			// pokud je to string, odstringovat
			if (*p == '\'' || *p == '\"')
			{
				while (*e != *p) e--;
				if (e==p)
					throw "syntax error, unclosed `\"' or `''";
				*e = '\0';
				p++;
			}
			gui->Set(name,p);
		}
	}
	} catch (const char * str)
	{
		fclose(f);
		BaseConsole::Printf("%s(%d): error: %s", fname, yyfigurelineno, str);
		return false;
	}

	fclose(f);
	if (type!=0)
	{
		BaseConsole::Printf("%s(%d): error: syntax error on end of file", fname, yyfigurelineno);
		return false;
	}
	return true;

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
		throw "syntax error, name requied";

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
		const char * n = m_list.Get(i)->GetName();
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
		char err[1024];
		sprintf(err,"Missing 2D object '%s'", name);
		HoeApp::GetApp<HoeApp>()->ShowMsg("2DFigure Error",err);
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


