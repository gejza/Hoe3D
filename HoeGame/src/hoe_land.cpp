
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

bool Land::Create(int width, int height)
{
    m_tile = new GP[width*height];
    m_width = width;
    m_height = height;
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
			TILE(x,y).souseds = m_lists.GetBasePointer((uint)TILE(x,y).souseds);
		}

	return true;
}


bool Land::Find(uint fx, uint fy, uint tx, uint ty, LandPath * out)
{
	//memset(g_cesta, 0, sizeof(g_cesta));
	if (!this->Process(&TILE(fx,fy), &TILE(tx,ty)))
		return false;
	GP * p = &TILE(tx,ty);
	do {
		out->Insert(hiword(p->pos),loword(p->pos),false);
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



