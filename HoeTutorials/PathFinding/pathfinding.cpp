
#include "pathfinding.h"

const char * g_TutorialName = "pathfind";

const int g_width = 40;
const int g_height = 30;

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

HoeMath::Polygon2 * CreatePolygon(HoeCore::Set<HoeMath::VLine2Int> &lines)
{
	// vytvorit polygon
	// vzit prvni lajnu
	HoeMath::Polygon2 * p = new HoeMath::Polygon2;
	hoe_assert(lines.Count() > 0);
	HoeMath::Vector2Int first,act,next,last;
	last = first = act = lines[0].a;
	next = lines[0].b;
	lines.RemoveIndex(0);
	// musi skoncit kde to zacalo
	// polygon add first
	//FILE * f = fopen("lines.txt","wt");
	bool bsave = true;
	while (1)
	{
		// polygon add  act
		// 
		if (bsave)
			//p->points.Add(HoeMath::Vector2((act.x * m_distX)-m_sizeX*0.5f,(act.y * m_distY)-m_sizeY*0.5f));
			p->points.Add(HoeMath::Vector2(act.x,act.y));
		act = next;

		if (lines.Count() > 0)
		{
			int i=0;
			for (;i < lines.Count();i++)
				if (lines[i].IfCanConnect(next))
					break;
			if (i == lines.Count())
				break;
			lines.RemoveIndex(i);
			if ((act.x == next.x && act.x == last.x)
				|| (act.y == next.y && act.y == last.y))
				bsave = false;
			else
			{
				bsave = true;
				last = act;
			}
		}
		else break;
	}
	//fclose(f);
	hoe_assert(first == act);
	// add to polygon
	//p->End();
	return p;
}

/////////////////////////////////////////////////////////

PathFindApp::PathFindApp(HOE_INSTANCE instance, HoeGame::Console * con) : HoeTutorial(instance, con)
{
	m_poly = NULL;
}

dword colors[] = { 0xff000000, 0xffffffff, 0xffff0000, 0xff00ff00, 0xff0000ff };

void PathFindApp::_Paint(IHoe2D *h)
{
	h->SetRect(g_width,g_height);

	if (m_poly)
	{
		uint i=0;
		for (i=0;i < m_poly->points.Count()-1;i++)
		{
			h->PaintLine(m_poly->points[i].x, m_poly->points[i].y,
				m_poly->points[i+1].x, m_poly->points[i+1].y, 
				colors[i%(sizeof(colors)/sizeof(dword))]);
		}
		h->PaintLine(m_poly->points[i].x, m_poly->points[i].y,
			m_poly->points[0].x, m_poly->points[0].y, 
				colors[i%(sizeof(colors)/sizeof(dword))]);

		return;
	}

	if (m_lines.Count() > 0)
	{
		for (uint i=0;i < m_lines.Count();i++)
		{
			h->PaintLine(m_lines[i].a.x, m_lines[i].a.y, m_lines[i].b.x, m_lines[i].b.y, colors[i%(sizeof(colors)/sizeof(dword))]);
		}
		return;
	}

	for (int y=0;y<g_height;y++)
		for (int x=0;x < g_width;x++)
		{
			register byte c = m_terrain.Get(x,y);
			if (c)
				h->PaintRect(x,x+1,y,y+1, 0xff000000, true);
		}
}

bool PathFindApp::LoadScene()
{
	/*GetFS()->AddResourceFile("../data/tex.hx");

	pic[0] = (IHoePicture *)GetEngine()->Create("picture samo");
	pic[1] = (IHoePicture *)GetEngine()->Create("picture earth");
	//pic[2] = (IHoePicture *)GetEngine()->Create("picture OLDMETAL");
	//pic[3] = (IHoePicture *)GetEngine()->Create("picture MetalGrate");
	f = (IHoeFont *)GetEngine()->Create("font '../data/font.ttf' 24");
	if (f == NULL)
		return false;
	f2 = (IHoeFont *)GetEngine()->Create("font '../data/Kidsn.ttf' 24");
	if (f2 == NULL)
		return false;*/
	m_terrain.Create(g_width, g_height);
	m_terrain.Clear(0);

	HoeGetRef(GetEngine())->SetBackgroundColor(0xffff);
	GetEngine()->GetActiveScene()->Set2DCallback(this);
	HoeGetInput(GetEngine())->RegisterMouse(IHoeInput::MT_Background, this);
	HoeGetInput(GetEngine())->RegisterKeyboard(this);
	HoeGetInput(GetEngine())->SetWindowRECT(g_width, g_height);

	return true;
}

void PathFindApp::OnMouseMove(float X, float Y)
{
	if (this->IsLeftButtonDown())
	{
		m_terrain.Set(X,Y,1);
	}
}

void PathFindApp::OnRightButtonUp()
{
	m_terrain.Pisek(GetMouseX(),GetMouseY(), 1, 0);
}

void PathFindApp::Preprocess()
{
	// optimize
	m_lines.Delete();

	m_terrain.GetLines(1, m_lines);


}

void PathFindApp::OnKeyDown(int key)
{
	if (key == HK_2)
		Preprocess();
	if (key == HK_1)
	{
		m_poly = NULL;
		m_lines.Delete();
	}
	if (key == HK_3)
	{
		m_poly = CreatePolygon(m_lines);
	}
}

