
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

const uint g_width = 400;
const uint g_height = 300;

struct TGraphPoint
{
    float g;
    float f;
    bool inopen;
    bool inclosed;
    int prevx,prevy;
    bool ispath;
};
    
TGraphPoint tile[g_width][g_height];

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
            else if (tile[x][y].ispath)
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
    memset(tile, 0, sizeof(tile));

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


struct PGraphPoint
{
    int x,y;
    bool operator < (const PGraphPoint & n) const
    {
        return tile[x][y].f < tile[n.x][n.y].f;
    }
    bool operator > (const PGraphPoint & n) const
    {
        return tile[x][y].f > tile[n.x][n.y].f;
    }
};

void PathFindApp::Process()
{
    float start = SysFloatTime();
    int numtest = 0;
    HoeCore::Heap<PGraphPoint> open;
    memset(tile,0,sizeof(tile));
    // hledat z from na to
    PGraphPoint p = {0,0};
    tile[0][0].inopen = true;
    open.Insert(p);
    while (open.Count() > 0)
    {
        numtest++;
        const PGraphPoint n = open.GetHeap();
        open.RemoveHeap();
        tile[n.x][n.y].inopen = false;
   
        if (n.x==g_width-1 && n.y==g_height-1)
        {
            printf("Cesta existuje! (%d) %f\n",numtest,SysFloatTime()-start);
            // projet vsechny a nastavit jim is path
            int x=n.x;
            int y=n.y;
            while (x!=0||y!=0)
            {
                TGraphPoint & pp = tile[x][y];
                pp.ispath = true;
                x = pp.prevx;y = pp.prevy;
            }
            return;
        }
        for (int px=-1;px < 2;px++)
            for (int py=-1;py < 2;py++)
            {
                if (px == 0 && py == 0)
                    continue;
                int nx = n.x+px;
                int ny = n.y+py;
                if (nx < 0 || nx >= g_width || ny < 0 || ny >= g_height)
                    continue;
                if (m_map.Get(nx,ny)==0)
                    continue;
                
                float g = tile[n.x][n.y].g + ((n.x==nx||n.y==ny)?1:1.41f); // g = n.g + w(n,nn)
                if ((tile[nx][ny].inclosed || tile[nx][ny].inopen) && tile[nx][ny].g <= g)
                    continue;
                tile[nx][ny].prevx = n.x;
                tile[nx][ny].prevy = n.y;
                tile[nx][ny].g = g;
                tile[nx][ny].f = g + ((g_width-nx+g_height-ny) / 5);
                tile[nx][ny].inclosed = false;
                if (!tile[nx][ny].inopen)
                {
                    tile[nx][ny].inopen = true;
                    p.x = nx;
                    p.y = ny;
                    open.Insert(p);
                }
            }
        tile[n.x][n.y].inclosed = true;
    }
    // cesta neexistuje
    printf("Cesta neexistuje\n");
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



