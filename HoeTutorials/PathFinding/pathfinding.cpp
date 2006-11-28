
/**
Demo pro hledani nejkratsi cesty
pouzity algoritmus bude A*, ktery lze pouzit i v hledani grafu
volitelna heuristicka funkce

Prioritní fronta     Open
seznam               Closed

   trida GroupPath
   - obsahuje optimalizace


*/

#include "pathfinding.h"

const char * g_TutorialName = "pathfind";

const uint g_width = 40;
const uint g_height = 30;

int _hoemain(HOE_INSTANCE instance, HoeGame::Console * con)
{
	PathFindApp app(instance,con);

	if (!app.InitTutorial(HOETS_2D))
	{
		app.HandleError();
		return 0;
	}

	if (app.LoadScene())
		app.Run();

	app.Destroy();

	return 0;
}

/*
  nejdriv skupiny
  pak vnitrek polygonu
  obejit polygony
  rozdelit na hrany
*/

/////////////////////////////////////////////////////////

PathFindApp::PathFindApp(HOE_INSTANCE instance, HoeGame::Console * con) : HoeTutorial(instance, con)
{
}

dword colors[] = { 0xff00ff00, 0xff0000ff };

void PathFindApp::_Paint(IHoe2D *h)
{
	h->SetRect((float)g_width,(float)g_height);

	// vykreslit start
	h->PaintRect(m_from.x-0.5f,m_from.x+0.5f,m_from.y-0.5f,m_from.y+0.5f, 0xffff0000, true);
	h->PaintRect(m_to.x-0.5f,m_to.x+0.5f,m_to.y-0.5f,m_to.y+0.5f, 0xff00ff00, true);

	const dword colors[] = { 0xff000000, 0xffff0000,0xff00ff00,0xff0000ff,0xffffffff };
	for (int y=0;y<g_height;y++)
		for (int x=0;x < g_width;x++)
		{
			register word c = m_map.Get(x,y);
			if (!(0xff&c))
				h->PaintRect(x,x+1,y,y+1, colors[((c>>8)-1)%4], true);
		}
	h->SetRect(g_width*20.f,g_height*14.f);
}

bool PathFindApp::LoadScene()
{
	/*GetFS()->AddResourceFile("../data/tex.hx");

	pic[0] = (IHoePicture *)GetEngine()->Create("picture samo");
	pic[1] = (IHoePicture *)GetEngine()->Create("picture earth");
	//pic[2] = (IHoePicture *)GetEngine()->Create("picture OLDMETAL");
	//pic[3] = (IHoePicture *)GetEngine()->Create("picture MetalGrate");
	if (f == NULL)
		return false;
	f2 = (IHoeFont *)GetEngine()->Create("font '../data/Kidsn.ttf' 24");
	if (f2 == NULL)
		return false;*/
	font = (IHoeFont *)GetEngine()->Create("font '../data/font.ttf' 12");
	m_map.Create(g_width, g_height);
	m_map.Clear(1);

	HoeGetRef(GetEngine())->SetBackgroundColor(0xffff);
	GetEngine()->GetActiveScene()->Set2DCallback(this);
	HoeGetInput(GetEngine())->RegisterMouse(IHoeInput::MT_Background, this);
	HoeGetInput(GetEngine())->RegisterKeyboard(this);
	HoeGetInput(GetEngine())->SetWindowRect(g_width, g_height);

	m_from.Set(0,0);
	m_to.Set(g_width, g_height);

	return true;
}

void PathFindApp::OnMouseMove(float X, float Y)
{
	if (this->IsLeftButtonDown() && !IsKeyDown(HK_LCONTROL) && !IsKeyDown(HK_RCONTROL)) 
	{
		m_map.Set(X,Y,this->IsShiftKeyDown() ? 1:0);
	}
}

void PathFindApp::OnLeftButtonUp()
{
	if (IsKeyDown(HK_LCONTROL))
		m_from.Set(GetMouseX(),GetMouseY());
	else if (IsKeyDown(HK_RCONTROL)) 
		m_to.Set(GetMouseX(),GetMouseY());
	else
		m_map.Set(GetMouseX(),GetMouseY(),this->IsShiftKeyDown() ? 1:0);
}

void PathFindApp::OnRightButtonUp()
{
	m_map.FloodFill(GetMouseX(),GetMouseY(), 0);
}

void PathFindApp::Preprocess()
{
	m_map.FloodFillPotencial();
	// ted najit vsechny oblasti
	// nejdrive okoli
	for (uint x=0;x < g_width;x++)
	{
		if (m_map.Get(x,0)==0)
			m_map.FloodFill(x,0,0x0100);
		if (m_map.Get(x,g_height-1)==0)
			m_map.FloodFill(x,g_height-1,0x0100);

	}
	for (uint y=0;y < g_height;y++)
	{
		if (m_map.Get(0,y)==0)
			m_map.FloodFill(0,y,0x0100);
		if (m_map.Get(g_width-1,y)==0)
			m_map.FloodFill(g_width-1,y,0x0100);
	}
	word c = 0x0200;
	for (uint y=0;y < g_height;y++)
		for (uint x=0;x < g_width;x++)
		{
		 	if (m_map.Get(x,y)==0)
			{
				m_map.FloodFill(x,y,c);	
				c += 0x0100;
			}
		}
	for (uint y=0;y<g_height;y++)
		for (uint x=0;x < g_width;x++)
		{
			if ((m_map.Get(x,y) & 0xff) == 0)
				continue;
			// nejaky bod, a udelat cestu
			uint px = x;
			uint py = y;
			while ((m_map.Get(px,py)&0xff) > 1)
			{
				if ((m_map.Get(px-1,py)&0xff) < (m_map.Get(px,py)&0xff))
					px--;
				else if ((m_map.Get(px,py-1)&0xff) < (m_map.Get(px,py)&0xff))
					py--;
				else if ((m_map.Get(px+1,py)&0xff) < (m_map.Get(px,py)&0xff))
					px++;
				else if ((m_map.Get(px,py+1)&0xff) < (m_map.Get(px,py)&0xff))
					py++;
				else
					hoe_assert(!"error in map");
			}
			// uz je to na kraji, staci jen zjisti na kterem
			if (px ==0 || py==0 || px==g_width-1 || py==g_height-1)
			{
				m_map.Set(x,y, 0x0100 | m_map.Get(x,y));
			}
			else
			{
				word c=0;
				if ((m_map.Get(px-1,py)&0xff) == 0)
					c=m_map.Get(px-1,py);
				else if ((m_map.Get(px,py-1)&0xff) == 0)
					c=m_map.Get(px,py-1);
				else if ((m_map.Get(px+1,py)&0xff) == 0)
					c=m_map.Get(px+1,py);
				else if ((m_map.Get(px,py+1)&0xff) == 0)
					c=m_map.Get(px,py+1);
				else
					hoe_assert(!"error in map");
				m_map.Set(x,y,(c&0xff00)|(m_map.Get(x,y)&0xff));
			}
		}
	// najit pak uzke prulezy (kolem 1)

}

void PathFindApp::OnKeyDown(int key)
{
	switch (key)
	{
	case HK_R: // zpatky na start
		for (int y=0;y<g_height;y++)
			for (int x=0;x < g_width;x++)
			{
				register word c = m_map.Get(x,y);
				m_map.Set(x,y, c&0xff ? 1:0);
			}
		break;
	case HK_SPACE:
		Preprocess();
		/*if (m_poly.Count() > 0)
		{
			HoeCore::TimeMeter tm;
			tm.Begin();
			m_path.Find(m_from, m_to, m_poly[0]);
			tm.End();
			int a=0;
		}*/
		break;
	};
}



