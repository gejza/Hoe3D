
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

class Land : private HoeCore::Algorythm::Dajkrs
{
	HoeCore::Algorythm::Dajkrs::TGraphPoint m_tile[g_width][g_height];
	HoeCore::List<HoeCore::Algorythm::Dajkrs::TGraphPoint*> m_lists;
	virtual float FCALL w(const TGraphPoint * from, const TGraphPoint * to);
	virtual float FCALL Heuristic(const TGraphPoint * from, const TGraphPoint * to);
public:
	bool Preprocess(HoeGame::AI::MapFindPath & map);
	bool Find(uint fx,uint fy, uint tx, uint ty);
};

bool Land::Preprocess(HoeGame::AI::MapFindPath &map)
{
	// nejdriv vytvorit, pak promazat
	// vytvoreni sousedu
	// hotovo
	
	// promaznout
	memset(m_tile, 0, sizeof(m_tile));
	m_lists.SetCount(0);
	dword np = 0;
	for (int x=0;x < g_width;x++)
		for (int y=0;y < g_height;y++)
		{
            if (map.Get(x,y)==0)
                    continue;
			// vytvorit seznam sousedu
			m_tile[x][y].souseds = (HoeCore::Algorythm::Dajkrs::TGraphPoint**)(m_lists.Count());
			for (int px=-1;px < 2;px++)
            for (int py=-1;py < 2;py++)
            {
                if (px == 0 && py == 0)
                    continue;
                int nx = x+px;
                int ny = y+py;
                if (nx < 0 || nx >= g_width || ny < 0 || ny >= g_height)
                    continue;
                if (map.Get(nx,ny)==0)
                    continue;
				m_lists.Add(&(m_tile[nx][ny]));
			}
			m_lists.Add(NULL);

		}

	for (int x=0;x < g_width;x++)
		for (int y=0;y < g_height;y++)
		{
			// priradit base
			m_tile[x][y].souseds = m_lists.GetBasePointer((uint)m_tile[x][y].souseds);
		}

	return true;
}

bool Land::Find(uint fx, uint fy, uint tx, uint ty)
{
	memset(g_cesta, 0, sizeof(g_cesta));
	if (!this->Process(&m_tile[fx][fy], &m_tile[tx][ty]))
		return false;
	HoeCore::Algorythm::Dajkrs::TGraphPoint * p = &m_tile[tx][ty];
	do {
		// pp
		// zjistit pointer
		dword a = ((dword)p-(dword)m_tile)/sizeof(HoeCore::Algorythm::Dajkrs::TGraphPoint);
		g_cesta[a/g_height][a%g_height] = true;
	} while (p=p->prev); // hack
	return true;
}

float Land::w(const HoeCore::Algorythm::Dajkrs::TGraphPoint *from, const HoeCore::Algorythm::Dajkrs::TGraphPoint *to)
{
	dword a = ((dword)from-(dword)m_tile)/sizeof(HoeCore::Algorythm::Dajkrs::TGraphPoint);
	dword b = ((dword)to-(dword)m_tile)/sizeof(HoeCore::Algorythm::Dajkrs::TGraphPoint);
	if (a/g_height==b/g_height || a%g_height == b%g_height)
		return 1;
	return 1.4142135623730950488016887242097f;
}

float Land::Heuristic(const TGraphPoint * from, const TGraphPoint * to)
{
	dword a = ((dword)from-(dword)m_tile)/sizeof(HoeCore::Algorythm::Dajkrs::TGraphPoint);
	dword b = ((dword)to-(dword)m_tile)/sizeof(HoeCore::Algorythm::Dajkrs::TGraphPoint);
	return (HoeMath::Abs(a/g_height-b/g_height) + HoeMath::Abs(a%g_height-b%g_height)) / 5.f;
}

void PathFindApp::Process()
{
	Land land;
	float start = SysFloatTime();
	land.Preprocess(m_map);
    int numtest = 0;
	if (land.Find(0,0,g_width-1, g_height-1))
		HoeGame::Console::Printf("Cesta existuje! (%d) %f",numtest,SysFloatTime()-start);
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



