
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

const uint g_width = 120;
const uint g_height = 90;

bool g_cesta[g_width][g_height];
   
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

	const dword colors[] = { 0xff000000, 0xffff0000,0xff00ff00,0xff0000ff,0xffffffff };
	for (int y=0;y<g_height;y++)
		for (int x=0;x < g_width;x++)
		{
			register word c = m_map.Get(x,y);
			if (!c)
				h->PaintRect(x,x+1,y,y+1, 0xff000000, true);
            else if (g_cesta[x][y])
                h->PaintRect(x,x+1,y,y+1, 0xffffffff, true);
		}

    
	// vykreslit start
	h->PaintRect(m_from.x-0.5f,m_from.x+0.5f,m_from.y-0.5f,m_from.y+0.5f, 0xffff0000, true);
	h->PaintRect(m_to.x-0.5f,m_to.x+0.5f,m_to.y-0.5f,m_to.y+0.5f, 0xff00ff00, true);

    // zobrazit cestu
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
	memset(g_cesta, 0, sizeof(g_cesta));

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

void PathFindApp::Process()
{
    HoeGame::Land land;
	float start = SysFloatTime();
    land.Create(g_width, g_height);
	land.Preprocess(m_map);
    int numtest = 0;
    HoeGame::LandPath result;
    memset(g_cesta, 0, sizeof(g_cesta));
	if (land.Find(0,0,g_width-1, g_height-1, &result))
    {
        // prekopirovat na result
        while (result.m_stack.Count() > 0)
        {
            HoeGame::LandPath::Point p = result.m_stack.Pop();
            g_cesta[(int)p.pos.x][(int)p.pos.y] = true;
        }
		HoeGame::Console::Printf("Cesta existuje! (%d) %f",numtest,SysFloatTime()-start);
    }
    else
		HoeGame::Console::Printf("Cesta neexistuje");
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
		Process();
		break;
	};
}



