
#include "StdAfx.h"
#include "../include/hoe_figure2d.h"
#include "../include/hoe_gui.h"

BEGIN_HOEGAME

Hoe2DFigure::Hoe2DFigure()
{
}

Hoe2DFigure::~Hoe2DFigure()
{
}

void HOEAPI Hoe2DFigure::_Paint(IHoe2D *)
{
}


bool Hoe2DFigure::Load(const char * fname)
{
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

END_HOEGAME


