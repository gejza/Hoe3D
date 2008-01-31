
#include "StdAfx.h"
#include "../include/HoeCore/hoe_core.h"
#include "../include/HoeCore/hoe_structures.h"
#include "../include/HoeCore/hoe_mem.h"
#include "../include/HoeCore/hoe_unicode.h"

#define INDEX(x,y) ((y)*m_width+(x))

HoeCore::WordTileMap::WordTileMap()
{
	m_map = NULL;
	m_width = 0;
	m_height = 0;
}

HoeCore::WordTileMap::WordTileMap(uint width, uint height)
{
	m_map = NULL;
	m_width = 0;
	m_height = 0;
	Create(width, height);
}

HoeCore::WordTileMap::~WordTileMap()
{
	if (m_map)
		delete [] m_map;
}

void HoeCore::WordTileMap::Create(uint width, uint height)
{
	if (m_map && width == m_width && height == m_height)
		return;
	if (m_map)
		delete [] m_map;
	m_map = new word[width*height];
	m_width = width;
	m_height = height;
}

void HoeCore::WordTileMap::Clear(word w)
{
	hoe_assert(m_map);
	for (uint y=0;y < m_height;y++)
	{
		for (uint x=0;x < m_width;x++)
			m_map[INDEX(x,y)] = w;
	}
}

bool HoeCore::WordTileMap::Find(word b, uint * x, uint * y)
{
	hoe_assert(m_map);
	for (;*y < m_height;(*y)++)
	{
		for (;*x < m_width;(*x)++)
			if (m_map[INDEX(*x,*y)] == b)
				return true;
		*x = 0;
	}
	return false;
}

// hack
struct Pnt
{
	uint x,y;
};

void HoeCore::WordTileMap::FloodFill(uint x, uint y, word w)
{
	hoe_assert(m_map);
	// s rekurzi jde rychle do prdele zasobnik

	// jestli je v okoli nejaka 
	dword find = m_map[INDEX(x,y)];

	HoeCore::Stack<Pnt> stk;
	Pnt p;
	p.x = x;p.y = y;
	stk.Push(p);
	m_map[INDEX(x,y)] = w;

	while (!stk.IsEmpty())
	{
		p = stk.Pop();
		x = p.x;
		y = p.y;
		if (x > 0 && m_map[INDEX(x-1,y)] == find)
		{
			m_map[INDEX(x-1,y)] = w;
			p.x = x-1;p.y = y;
			stk.Push(p);
		}
		if (y > 0 && m_map[INDEX(x,y-1)] == find)
		{
			m_map[INDEX(x,y-1)] = w;
			p.x = x;p.y = y-1;
			stk.Push(p);
		}
		if ((x+1) < m_width && m_map[INDEX(x+1,y)] == find)
		{
			m_map[INDEX(x+1,y)] = w;
			p.x = x+1;p.y = y;
			stk.Push(p);
		}
		if ((y+1) < m_height && m_map[INDEX(x,y+1)] == find)
		{
			m_map[INDEX(x,y+1)] = w;
			p.x = x;p.y = y+1;
			stk.Push(p);
		}
	}
}

void HoeCore::WordTileMap::FloodFillPotencial()
{
	bool needact;
	do {
		needact = false;
		for (uint y=0;y < m_height;y++)
			for (uint x=0;x < m_width;x++)
			{
				if (m_map[INDEX(x,y)] == 0)
					continue;

				// najit nejmensi okolni prvek
				word min = m_map[INDEX(x,y)];
				if (x > 0)
				{
					if (m_map[INDEX(x-1,y)] < min) min = m_map[INDEX(x-1,y)];
				}
				else
					min = 0;
				if (y > 0)
				{ 
					if (m_map[INDEX(x,y-1)] < min)
					min = m_map[INDEX(x,y-1)];
				}
				else
					min = 0;
				if ((x+1) < m_width)
				{
					if (m_map[INDEX(x+1,y)] < min)
					min = m_map[INDEX(x+1,y)];
				}
				else
					min = 0;
				if ((y+1) < m_height)
				{
					if (m_map[INDEX(x,y+1)] < min)
					min = m_map[INDEX(x,y+1)];
				}
				else
					min = 0;
				// mam minimum
				min++;
				if (min != m_map[INDEX(x,y)])
				{
					m_map[INDEX(x,y)] = min;
					needact = true;
				}
			}
	}
	while (needact);
}

void HoeCore::WordTileMap::Copy(const HoeCore::WordTileMap * map)
{
	Create(map->m_width, map->m_height);
	memcpy(m_map, map->m_map, m_width * m_height);
}

int HoeCore::WordTileMap::GetLines(word tile, HoeCore::Set<HoeMath::SegmentLine2i> &lines)
{
	hoe_assert(m_map);
	HoeMath::SegmentLine2i line;
	uint scount = lines.Count();
	for (uint y=0;y < m_height;y++)
		for (uint x=0;x<m_width;x++)
		{
			// pokud atd
			if (m_map[INDEX(x,y)] != tile)
				continue;
			if ((x+1) < m_width && m_map[INDEX(x+1,y)] != tile)
			{
				line.a = HoeMath::Vector2i(x+1,y);
				line.b = HoeMath::Vector2i(x+1,y+1);
				lines.Add(line);
			}
			if ((y+1) < m_height && m_map[INDEX(x,y+1)] != tile)
			{
				line.a = HoeMath::Vector2i(x,y+1);
				line.b = HoeMath::Vector2i(x+1,y+1);
				lines.Add(line);
			}
			if (x > 0 && m_map[INDEX(x-1,y)] != tile)
			{
				line.a = HoeMath::Vector2i(x,y);
				line.b = HoeMath::Vector2i(x,y+1);
				lines.Add(line);
			}
			if (y > 0 && m_map[INDEX(x,y-1)] != tile)
			{
				line.a = HoeMath::Vector2i(x,y);
				line.b = HoeMath::Vector2i(x+1,y);
				lines.Add(line);
			}
		}
	return lines.Count() - scount;
}

#undef INDEX
/////////////////

HoeCore::TimeMeter::TimeMeter()
{
	start = max = total = avarage = percent = pubmax = last = 0.f;
	num = 0;
}

void HoeCore::TimeMeter::Begin()
{
	start = SysFloatTime();
}

void HoeCore::TimeMeter::End()
{
	last = SysFloatTime() - start;
	total += last;
	if (last > max)
		max = last;
	num++;
}

////////////////////////////////////
inline dword MakeHash(dword h, char c)
{
	register dword g = h & 0xf0000000;
	h = h ^ (g >> 24);
	h = h ^ g;
	h = h << 4;
	return h + c;
}

dword HoeCore::HashString(const char * str)
{
	register dword hash = 0;
	while (*str)
	{
		hash = MakeHash(hash, *str++);
	}
	return hash;
}

dword HoeCore::HashString(const wchar_t * str)
{
	register dword hash = 0;
	char buff[8];
	while (*str)
	{
		char * p = buff;
		size_t n = string::w2utf(p, *str++, 8);
		for (size_t s = 0;s < n;s++)
			hash = MakeHash(hash, buff[s]);
	}
	return hash;
}

// algorithm
bool HoeCore::Algorythm::Dajkrs::Process(TGraphPoint * from, TGraphPoint * to)
{
	hoe_assert(from != NULL && to != NULL);
	m_open.SetCount(0);
	PGraphPoint p;
	p.p = from;
    p.p->inopen = true;
    m_open.Insert(p);
    while (m_open.Count() > 0)
    {
        const PGraphPoint n = m_open.GetHeap();
        m_open.RemoveHeap();
		n.p->inopen = false;
		// ono by se to dalo udelat tak, ze kazdy ten point, bude ukazovat na point
		// a dostane seznam sousedu
        if (n.p == to)
            return true;
		hoe_assert(n.p->souseds != NULL);
		for (TGraphPoint ** souseds = n.p->souseds; *souseds;souseds++)
		{
			register TGraphPoint * nn = *souseds;
			float g = n.p->g + w(n.p, nn);
            if ((nn->inclosed || nn->inopen) && nn->g <= g)
                    continue;
            nn->prev = n.p;
			nn->g = g;
			nn->f = g + Heuristic(nn,to);
			nn->inclosed = false;
			if (!nn->inopen)
			{
				nn->inopen = true;
				p.p = nn;
				m_open.Insert(p);
            }
		}
		n.p->inclosed = true;
    }
	return false;
}

/////////////////////////////////////////////
HoeCore::StringPool::ConstString::ConstString(const char * s) 
{
	hash = HoeCore::HashString(s);
	str = s;
}

HoeCore::StringPool::PoolIndex::PoolIndex(const char * s) : ConstString(s)
{
	ref = 1;
}

HoeCore::StringPool::StringPool() 
{
}

const char * HoeCore::StringPool::Strdup(const char * str)
{
	size_t l = string::len(str)+1;
	return reinterpret_cast<const char*>(Clone(str, l));
}

void HoeCore::StringPool::Remove(const char * str)
{
	// zatim vypnuto
	/*todo PoolIndex * ppi = m_keys.Find(str);
	hoe_assert(!ppi);
	if (ppi)
	{
		if (ppi <= 1) 
		{
			// odebrat ze seznamu
			free(ppi->str);
		}
		else
			ppi->ref--;
	}*/
}

const char * HoeCore::StringPool::Insert(const char * str)
{
	PoolIndex * ppi = m_keys.Find(str);
	if (ppi)
	{
		ppi->ref++;
		return ppi->str;
	}
	// vytvorit
	PoolIndex pi(this->Strdup(str));
	m_keys.Add(pi);
	return pi.str;
}

void HoeCore::StringPool::GetStats(StringStats* stat)
{
	memset(stat, 0, sizeof(StringStats));
	HoeCore::MemoryPool::GetStats(stat);
	stat->numstr = this->m_keys.Count();
	for (uint i=0;i < m_keys.Count();i++)
	{
		stat->refs += m_keys[i].ref;
	}
}

/////////////////////////////////////////////
HoeCore::Table::Table(HoeCore::StringPool & pool)
: m_pool(pool) 
{
}

bool HoeCore::Table::IfExist(const HoeCore::KeyString& key) const
{
	for (uint u=0;u < m_items.Count();u++)
		if (m_items[u].key == key)
			return true;
	return false;
}

HoeCore::Universal & HoeCore::Table::Get(const HoeCore::KeyString& key)
{
	for (uint u=0;u < m_items.Count();u++)
		if (m_items[u].key == key)
			return m_items[u].value;
	// vytvorit
	Item &i = m_items.Add();
	i.key = KeyString(m_pool.Insert(key.key));
	return i.value;
}





