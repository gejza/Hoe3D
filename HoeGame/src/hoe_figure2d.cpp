
#include "StdAfx.h"
#include "../include/hoe_figure2d.h"
#include "../include/hoe_gui.h"
#include <locale.h>


BEGIN_HOEGAME

Hoe2DFigureBase::Hoe2DFigureBase()
{
}

Hoe2DFigureBase::~Hoe2DFigureBase()
{
}

void HOEAPI Hoe2DFigureBase::_Paint(IHoe2D *)
{
}


bool Hoe2DFigureBase::Load(const char * fname)
{
	setlocale(LC_NUMERIC, "C");

	FigureFile ff;
	if (!ff.Open(fname))
		return false;

	try
	{

		char buff[2048];
		while (ff.GetLine(buff, sizeof(buff)))
		{
			// vytvorit base
			HoeGame::BaseGui * gui = CreateGUI(buff);		
			if (!gui)
				throw "chyba";
			if (!ff.GetLine(buff, sizeof(buff)) || strcmp(buff,"{") != 0)
				throw "chyba";
			while (1)
			{
				if (!ff.GetLine(buff, sizeof(buff)))
					throw "chyba";
				if (strcmp(buff,"}") == 0)
					break;
				char * p = buff;
				while (*p == '\t' || *p == ' ') p++;
				const char * stname = p;
				while (*p > ' ' && *p != '=') p++;
				if (*p == 0)
					throw "chyba";
				if (*p != '=')
					*p++ = '\0';
				while (*p != '=')
				{
					if (*p != ' ' && *p != '\t')
						throw "chyba";
					p++;
				}
				*p++ = '\0';
				while (*p == ' ' || *p == '\t') p++;
				if (*p == '\"')
				{
					p++;
					const char *val = p;
					while (*p) p++;
					p--;
					while (*p == ' ' || *p == '\t') p--;
					if (*p != '\"')
						throw "chyba";
					*p = '\0';
					gui->Set(stname, val);
				}
				else
					gui->Set(stname,p);
			}
		}
	} catch (...)
	{
		return false;
	}
	
	return true;
}

BaseGui * Hoe2DFigure::CreateGUI(const char *type)
{
	GuiItem * g = NULL;
#define IS(t) (strcmp(type,t)==0)
	if (IS("static"))
		g = new StaticPicture;
	else if (IS("colorrect"))
		g = new ColorRect;
	else if (IS("button"))
		g = new Button;
	else if (IS("digicounter"))
		g = new DigiCounter;
	else if (IS("infopanel"))
		g = new InfoPanel;
	else if (IS("font"))
		g = new Font;

	m_list.Add(g);
	return g;
}

void Hoe2DFigure::Draw(IHoe2D *hoe2d)
{
	hoe2d->SetRect(800,600);
	for (uint i=0;i<m_list.Count();i++)
		m_list.Get(i)->Draw(hoe2d);
}

GuiItem * Hoe2DFigure::GetItem(const char * name)
{
	for (uint i=0;i<m_list.Count();i++)
	{
		const char * n = m_list.Get(i)->GetName();
		if (n && strcmp(n,name)==0)
			return m_list.Get(i);
	}
	return NULL;
}

GuiItem * Hoe2DFigure::ReqItem(const char * name)
{
	GuiItem * i = GetItem(name);
	if (!i)
	{
		char err[1024];
		sprintf(err,"Missing 2D object '%s'", name);
		MessageBox(NULL,err,"2DFigure Error",0);
		exit(1);
	}
	return i;
}

END_HOEGAME


