
#include "StdAfx.h"
#include "../include/hoe_land.h"

BEGIN_HOEGAME

void LandPath::Insert(float x, float y, bool virt)
{
    Point p;
    p.pos = HoeMath::Vector2(x,y);
    p.virt = virt;
    m_stack.Push(p);
}

/////////////////////////////////////////
// Path
/*bool TrollPath::GetNextPos(float l,float &px, float &py)
{
	float ux = x - px;
	float uy = y - py;
	float mag = sqrt(ux * ux + uy * uy);
	ux /= mag;
	uy /= mag;
	if (l < mag)
	{
		px += ux * l;
		py += uy * l;
		return false;
	}
	px = x;
	py = y;

	// dalsi to
	if (points.IsEmpty())
		return true;
	HoeGame::AI::PathPoint next = points.Pop();
	x = next.to.x;
	y = next.to.y;

	return false;
}
*/


/////////////////////////////////////////////////////

Land::Land()
{
    m_tile = NULL;
}

Land::~Land()
{
    if (m_tile)
        delete [] m_tile;
}

#define TILE(x,y) (m_tile[y*m_width+x]) 

bool Land::Create(int width, int height, const THoeRect &rect)
{
    m_tile = new GP[width*height];
    m_width = width;
    m_height = height;
	m_rect = rect;
    return true;
}

bool Land::Preprocess(HoeCore::WordTileMap &map)
{
    hoe_assert(m_tile);
	memset(m_tile, 0, sizeof(GP)*m_width*m_height);
	m_lists.SetCount(0);
	dword np = 0;
	for (int x=0;x < m_width;x++)
		for (int y=0;y < m_height;y++)
		{
            if (map.Get(x,y)==0)
                    continue;
			// vytvorit seznam sousedu
			TILE(x,y).souseds = (HoeCore::Algorythm::Dajkrs::TGraphPoint**)(m_lists.Count());
			for (int px=-1;px < 2;px++)
            for (int py=-1;py < 2;py++)
            {
                if (px == 0 && py == 0)
                    continue;
                int nx = x+px;
                int ny = y+py;
                if (nx < 0 || nx >= m_width || ny < 0 || ny >= m_height)
                    continue;
                if (map.Get(nx,ny)==0)
                    continue;
				m_lists.Add(&(TILE(nx,ny)));
			}
			m_lists.Add(NULL);
            // ulozit cestu
            TILE(x,y).pos = makedword(x,y);
		}

	for (int x=0;x < m_width;x++)
		for (int y=0;y < m_height;y++)
		{
			// priradit base
			//todo 64 bit 
			//TILE(x,y).souseds = m_lists.GetBasePointer(0)+TILE(x,y).souseds;
			TILE(x,y).souseds = m_lists.GetBasePointer((unsigned long)TILE(x,y).souseds);
		}

	return true;
}


bool Land::Find(const HoeMath::Vector2 &from, const HoeMath::Vector2 &to, LandPath &out)
{
	const float dx = (m_rect.right - m_rect.left)/m_width;
	const float dy = (m_rect.bottom - m_rect.top)/m_height;
	// prepocitat na ctverce
	// zjisti ctverec
	int fx = (from.x - m_rect.left) / dx;
	int fy = (from.y - m_rect.top) / dy;
	int tx = (to.x - m_rect.left) / dx;
	int ty = (to.y - m_rect.top) / dy;

	//memset(g_cesta, 0, sizeof(g_cesta));
	for (int i=0;i < m_width*m_height;i++)
	{
		const GP p = m_tile[i];
		memset(&m_tile[i],0,sizeof(m_tile[i]));
		m_tile[i].pos = p.pos;
		m_tile[i].souseds = p.souseds;
	}

	if (!this->Process(&TILE(fx,fy), &TILE(tx,ty)))
		return false;
	GP * p = &TILE(tx,ty);
	do {

		out.Insert((hiword(p->pos)*dx)+m_rect.left,
			(loword(p->pos)*dy)+m_rect.top,false);
	} while (p=(GP*)p->prev); // hack
	return true;
}


float Land::w(const HoeCore::Algorythm::Dajkrs::TGraphPoint *from, const HoeCore::Algorythm::Dajkrs::TGraphPoint *to)
{
    int x1 = hiword(((GP*)from)->pos);
    int y1 = loword(((GP*)from)->pos);
    int x2 = hiword(((GP*)to)->pos);
    int y2 = loword(((GP*)to)->pos);
	if (x1==x2 || y1==y2)
		return 1;
	return 1.4142135623730950488016887242097f;
}

float Land::Heuristic(const TGraphPoint * from, const TGraphPoint * to)
{
    int dx = hiword(((GP*)from)->pos);
    int dy = loword(((GP*)from)->pos);
    dx -= hiword(((GP*)to)->pos);
    dy -= loword(((GP*)to)->pos);
    return sqrtf(dx*dx+dy*dy);
}

#undef TILE


END_HOEGAME



